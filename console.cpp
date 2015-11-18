#include "console.h"

#include "console.scanner.h"

#include "parser.h"
#include "state.h"
#include "colors.h"

#include <ace/Get_Opt.h>

#ifdef USE_READLINE
#   include <readline/readline.h>
#   include <readline/history.h>
#endif

#include <stdio.h>

#if !defined _WIN32
#   include <unistd.h>
#else
#   include <io.h>
#   define isatty(x) _isatty(x)
#endif

#include <math.h>

#include <iomanip>
#include <typeinfo>

using namespace parser;

CConsole::CConsole(int argc, char** argv): m_mode(EM_EXPRESSIONS),
    m_interactive(0 != isatty(fileno(stdin))),
    m_do_exit(false),
    m_variable(NULL)
{
    ACE_Get_Opt getopt(argc, argv, "hwi");
    getopt.long_option("where", 'w', ACE_Get_Opt::NO_ARG);
    getopt.long_option("help", 'h', ACE_Get_Opt::NO_ARG);
    getopt.long_option("non-interactive", 'i', ACE_Get_Opt::NO_ARG);

    int command;
    int nonopt;
    while (EOF != (command = getopt()))
    {
        switch (command)
        {
            case 'w':
                m_mode = EM_WHERE;
                break;

            case 'i':
                m_interactive = false;
                break;

            case 'h':
            default:
                usage(argv[0]);
                m_do_exit = true;
                return;
        }
    }

    nonopt = getopt.opt_ind();
    if (getopt.opt_ind() != argc)
    {
        std::cerr << "Wrong option '" << argv[nonopt] << "'" << std::endl;
        usage(argv[0]);
        m_do_exit = true;
        return;
    }

    if (!m_interactive)
    {
        colors::turn_off();
    }
    else
    {
        colors::turn_on();
    }

    prepare_variables();
}

int CConsole::run()
{
    if (m_do_exit)
    {
        return 0; // initialization was failed
    }

#ifdef USE_READLINE
    using_history();
#endif

    do
    {
        std::string line;
        if (get_line(line))
        {
            // means that we user closed input channel
            break;
        }            
               
        if (parse_command(line))
        {
            // means that we should do nothing anymore
            continue;
        }

        handle_command();
    } while (true);

    m_parser.reset();

    return 0;
}

void CConsole::prepare_variables()
{
    // nothing to do now.
}

bool CConsole::parse_command(const std::string& cmd)
{
    using namespace parser;
    using namespace grammar::console;
    using namespace scanner::console;

    CTreeNode::ptr_set_t temp_set;
    std::list<CParserNode*> nodes;
    grammar::SState state(0, 0, static_cast<void*>(this), &m_parser.root(), &temp_set);

    if (cmd.empty())
    {
        return true;    // Just ignore empty lines
    }

    void* parser = ParseAlloc(malloc);
    scanner::CToken t;
    scanner::CState ss;
    for (t = CScanner()(cmd, ss);
            scanner::CToken::ET_ERROR != t.token() && scanner::CToken::ET_EOF != t.token();
            t = CScanner()(cmd, ss))
    {
        CParserNode* node = CTreeNodeFactory::createParserNode(t.value());
        nodes.push_back(node);
        Parse(parser, t.token(), node, &state);
        if (state.syntax_error)
        {
            break;
        }
        state.offset = static_cast<int>(ss.offset());
    }
    if (!state.syntax_error && scanner::CToken::ET_ERROR != t.token())
    {
        state.offset = static_cast<int>(ss.offset());
        Parse(parser, scanner::CToken::ET_END, NULL, &state);
    }
    if (state.syntax_error || scanner::CToken::ET_ERROR == t.token())
    {
        if (state.syntax_error)
        {
            std::cerr << "CONSOLE: Grammar error at offset " << colors::green() << ss.offset() << colors::reset()
                << "; Unexpected token #" << t.token() << std::endl;
        }
        else
        {
            std::cerr << "CONSOLE: Syntax error at offset " << colors::green()<< state.offset << colors::reset() << std::endl;
        }
        std::string msg = cmd;
        msg.insert(state.offset, colors::blue());
        msg = "'" + msg;
        msg.append(colors::reset());
        msg.append("'");
        std::cerr << msg << std::endl;
        indent(std::cerr, state.offset, " ") << " ^ here" << std::endl;

        for (CTreeNode::ptr_set_t::const_iterator i = temp_set.begin(); i != temp_set.end(); i++)
        {
            delete *i;
        }
        temp_set.clear();
    }
    ParseFree(parser, free);

    while (!nodes.empty())
    {
        delete nodes.front();
        nodes.pop_front();
    }

    return false;
}

void CConsole::handle_command()
{
    if (".where" == m_command)
    {
        m_mode = EM_WHERE;
    }
    else if (".expr" == m_command)
    {
        m_mode = EM_EXPRESSIONS;
    }
    else if (".dump" == m_command)
    {
        m_parser.dump_tree(std::cout);
        m_parser.dump_variables(std::cout);
    }
    else if (".set" == m_command || ".exec" == m_command)
    {
        m_parser.clear_tree();
        switch (m_mode)
        {
            case EM_WHERE:
                m_parser.parse<types::Where>(m_expression);
                break;

            case EM_EXPRESSIONS:
                m_parser.parse<types::Expressions>(m_expression);
                break;
        }

        if (m_parser.ready())
        {
            try
            {
                CValue value = m_parser.evaluate();
                if (".set" == m_command)
                {
                    m_variable->get(true).value(value);
                    std::cout << "OK" << std::endl;
                }
                else if (".exec" == m_command)
                {
                    std::cout << (m_interactive ? "Value: " : "")
                        << colors::green()<< std::setprecision(16) << value << colors::reset()
                        << std::endl;
                }
                else
                {
                    std::cerr << "Unexpected command. Do nothing." << std::endl;
                }
            } catch (const CRuntimeException& e)
            {
                std::cerr << colors::red() << "Runtime error: " << e.what() << colors::reset() << std::endl;
            }
        }
    }
}

bool CConsole::get_line(std::string& result)
{
    std::string prompt = "";
    if (m_interactive)
    {
        switch (m_mode)
        {
            case EM_WHERE:
                prompt = "where> ";
                break;

            case EM_EXPRESSIONS:
                prompt = "expr> ";
                break;
        }
    }

#ifdef USE_READLINE
    if (m_interactive)
    {
        const char* l = readline(prompt.c_str());
        if (l)
        {
            result = l;
            add_history(l);
        }
        else
        {
            std::cout << std::endl;
            return true;;
        }
    }
    else
#endif
    {
        std::cout << prompt;
        std::getline(std::cin, result);
        if (std::cin.eof())
        {
            return true;
        }
    }
    return false;
}

void CConsole::usage(const char* cmd)
{
    std::cout << "Usage: " << cmd << " [-i] [-w] [-h]" << std::endl;
    std::cout
        <<       " --non-interactive Force non interactive mode [-i]\n"
        <<       " -where            Start from WHERE mode (by default is EXPRESSIONS) [-w]\n"
        <<       " --help            Show this help [-h]" << std::endl;
}


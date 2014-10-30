#include "parser.h"

#include "scanner.h"
#include "grammar.h"
#include "state.h"

#include <cstdlib>

extern "C"
{
    void *ParseAlloc(void *(*mallocProc)(size_t));
    void Parse(
            void *yyp,                   /* The parser */
            int yymajor,                 /* The major token code number */
            void* yyminor,               /* The value for the token */
            void*                        /* Optional %extra_argument parameter */
            );
    void ParseFree(
            void *p,                    /* The parser to be deleted */
            void (*freeProc)(void*)     /* Function used to reclaim memory */
            );
}

namespace parser
{
    static const char *TYPE_NAMES[] =
    {
#define TOKEN(x) #x,
        TYPES
#undef TOKEN
    };

long long int CValue::get_integer() const
{
    if (ETYPE_INTEGER != m_type)
    {
        throw CRuntimeException("Type of value is not integer");
    }

    return m_value.i;
}

long double CValue::get_float() const
{
    if (ETYPE_FLOAT != m_type)
    {
        throw CRuntimeException("Type of value is not float");
    }

    return m_value.f;
}

bool CValue::get_boolean() const
{
    if (ETYPE_BOOLEAN != m_type)
    {
        throw CRuntimeException("Type of value is not boolean");
    }

    return m_value.b;
}

const std::string& CValue::get_string() const
{
    if (ETYPE_STRING != m_type)
    {
        throw CRuntimeException("Type of value is not string");
    }

    return m_string;
}

CValue CValue::cast_to(EType type) const
{
    const char* from = TYPE_NAMES[m_type];
    const char* to = TYPE_NAMES[type];

    switch (m_type)
    {
        case ETYPE_INTEGER:
            switch (type)
            {
                case ETYPE_INTEGER:
                    return CValue(m_value.i);

                case ETYPE_FLOAT:
                    return CValue(static_cast<long double>(m_value.i));

                case ETYPE_BOOLEAN:
                    return CValue(0 != m_value.i);

                default:
                    throw CRuntimeException((std::string("Could not cast ") + from + " to type " + to).c_str());
            }

        case ETYPE_FLOAT:
            switch (type)
            {
                case ETYPE_FLOAT:
                    return CValue(m_value.f);

                default:
                    throw CRuntimeException((std::string("Could not cast ") + from + " to type " + to).c_str());
            }

        case ETYPE_BOOLEAN:
            switch (type)
            {
                case ETYPE_BOOLEAN:
                    return CValue(m_value.b);

                default:
                    throw CRuntimeException((std::string("Could not cast ") + from + " to type " + to).c_str());
            }

        case ETYPE_STRING:
            switch (type)
            {
                case ETYPE_STRING:
                    return CValue(m_value.s);

                default:
                    throw CRuntimeException((std::string("Could not cast ") + from + " to type " + to).c_str());
            }

        default:
            throw CRuntimeException((std::string("Could not cast ") + from + " to type " + to).c_str());
    }
}

CValue::EType CValue::top_type(CValue::EType a, CValue::EType b)
{
#define TOP(x, y, z) do { if (a == (x) && b == (y)) { return (z); } } while (false)
    TOP(ETYPE_INTEGER, ETYPE_INTEGER, ETYPE_INTEGER);
    TOP(ETYPE_FLOAT, ETYPE_INTEGER, ETYPE_FLOAT);
    TOP(ETYPE_INTEGER, ETYPE_FLOAT, ETYPE_FLOAT);
    TOP(ETYPE_FLOAT, ETYPE_FLOAT, ETYPE_FLOAT);
    TOP(ETYPE_BOOLEAN, ETYPE_BOOLEAN, ETYPE_BOOLEAN);
    TOP(ETYPE_STRING, ETYPE_STRING, ETYPE_STRING);
#undef TOP

    const char* from = TYPE_NAMES[a];
    const char* to = TYPE_NAMES[b];

    throw CRuntimeException((std::string("Could not choose common type between ") + from + " and " + to).c_str());
}

CValue operator<(const CValue& a, const CValue& b)
{
    const CValue::EType type = CValue::top_type(a.type(), b.type());
    const CValue& l = a.type() == type ? a : a.cast_to(type);
    const CValue& r = b.type() == type ? b : b.cast_to(type);

    switch (type)
    {
        case CValue::ETYPE_INTEGER:
            return l.get_integer() < r.get_integer();

        case CValue::ETYPE_FLOAT:
            return l.get_float() < r.get_float();

        case CValue::ETYPE_BOOLEAN:
            return l.get_boolean() < r.get_boolean();

        case CValue::ETYPE_STRING:
            return l.get_string() < r.get_string();

        default:
            throw CRuntimeException((std::string("Unexpected type") + TYPE_NAMES[type] + " for operation <").c_str());
    }
}

CValue operator<=(const CValue& a, const CValue& b)
{
    const CValue::EType type = CValue::top_type(a.type(), b.type());
    const CValue& l = a.type() == type ? a : a.cast_to(type);
    const CValue& r = b.type() == type ? b : b.cast_to(type);

    switch (type)
    {
        case CValue::ETYPE_INTEGER:
            return l.get_integer() <= r.get_integer();

        case CValue::ETYPE_FLOAT:
            return l.get_float() <= r.get_float();

        case CValue::ETYPE_BOOLEAN:
            return l.get_boolean() <= r.get_boolean();

        case CValue::ETYPE_STRING:
            return l.get_string() <= r.get_string();

        default:
            throw CRuntimeException((std::string("Unexpected type") + TYPE_NAMES[type] + " for operation <=").c_str());
    }
}

CValue operator==(const CValue& a, const CValue& b)
{
    const CValue::EType type = CValue::top_type(a.type(), b.type());
    const CValue& l = a.type() == type ? a : a.cast_to(type);
    const CValue& r = b.type() == type ? b : b.cast_to(type);

    switch (type)
    {
        case CValue::ETYPE_INTEGER:
            return l.get_integer() == r.get_integer();

        case CValue::ETYPE_FLOAT:
            return l.get_float() == r.get_float();

        case CValue::ETYPE_BOOLEAN:
            return l.get_boolean() == r.get_boolean();

        case CValue::ETYPE_STRING:
            return l.get_string() == r.get_string();

        default:
            throw CRuntimeException((std::string("Unexpected type") + TYPE_NAMES[type] + " for operation ==").c_str());
    }
}

CValue operator!=(const CValue& a, const CValue& b)
{
    const CValue::EType type = CValue::top_type(a.type(), b.type());
    const CValue& l = a.type() == type ? a : a.cast_to(type);
    const CValue& r = b.type() == type ? b : b.cast_to(type);

    switch (type)
    {
        case CValue::ETYPE_INTEGER:
            return l.get_integer() != r.get_integer();

        case CValue::ETYPE_FLOAT:
            return l.get_float() != r.get_float();

        case CValue::ETYPE_BOOLEAN:
            return l.get_boolean() != r.get_boolean();

        case CValue::ETYPE_STRING:
            return l.get_string() != r.get_string();

        default:
            throw CRuntimeException((std::string("Unexpected type") + TYPE_NAMES[type] + " for operation !=").c_str());
    }
}

CValue operator>(const CValue& a, const CValue& b)
{
    const CValue::EType type = CValue::top_type(a.type(), b.type());
    const CValue& l = a.type() == type ? a : a.cast_to(type);
    const CValue& r = b.type() == type ? b : b.cast_to(type);

    switch (type)
    {
        case CValue::ETYPE_INTEGER:
            return l.get_integer() > r.get_integer();

        case CValue::ETYPE_FLOAT:
            return l.get_float() > r.get_float();

        case CValue::ETYPE_BOOLEAN:
            return l.get_boolean() > r.get_boolean();

        case CValue::ETYPE_STRING:
            return l.get_string() > r.get_string();

        default:
            throw CRuntimeException((std::string("Unexpected type") + TYPE_NAMES[type] + " for operation >").c_str());
    }
}

CValue operator>=(const CValue& a, const CValue& b)
{
    const CValue::EType type = CValue::top_type(a.type(), b.type());
    const CValue& l = a.type() == type ? a : a.cast_to(type);
    const CValue& r = b.type() == type ? b : b.cast_to(type);

    switch (type)
    {
        case CValue::ETYPE_INTEGER:
            return l.get_integer() >= r.get_integer();

        case CValue::ETYPE_FLOAT:
            return l.get_float() >= r.get_float();

        case CValue::ETYPE_BOOLEAN:
            return l.get_boolean() >= r.get_boolean();

        case CValue::ETYPE_STRING:
            return l.get_string() >= r.get_string();

        default:
            throw CRuntimeException((std::string("Unexpected type") + TYPE_NAMES[type] + " for operation >=").c_str());
    }
}

CValue operator+(const CValue& a, const CValue& b)
{
    const CValue::EType type = CValue::top_type(a.type(), b.type());
    const CValue& l = a.type() == type ? a : a.cast_to(type);
    const CValue& r = b.type() == type ? b : b.cast_to(type);

    switch (type)
    {
        case CValue::ETYPE_INTEGER:
            return l.get_integer() + r.get_integer();

        case CValue::ETYPE_FLOAT:
            return l.get_float() + r.get_float();

        default:
            throw CRuntimeException((std::string("Unexpected type") + TYPE_NAMES[type] + " for operation +").c_str());
    }
}

CValue operator-(const CValue& a, const CValue& b)
{
    const CValue::EType type = CValue::top_type(a.type(), b.type());
    const CValue& l = a.type() == type ? a : a.cast_to(type);
    const CValue& r = b.type() == type ? b : b.cast_to(type);

    switch (type)
    {
        case CValue::ETYPE_INTEGER:
            return l.get_integer() - r.get_integer();

        case CValue::ETYPE_FLOAT:
            return l.get_float() - r.get_float();

        default:
            throw CRuntimeException((std::string("Unexpected type") + TYPE_NAMES[type] + " for operation -").c_str());
    }
}

CValue operator-(const CValue& a)
{
    switch (a.type())
    {
        case CValue::ETYPE_INTEGER:
            return -a.get_integer();

        case CValue::ETYPE_FLOAT:
            return -a.get_float();

        default:
            throw CRuntimeException((std::string("Unexpected type") + TYPE_NAMES[a.type()] + " for operation -").c_str());
    }
}

CValue operator/(const CValue& a, const CValue& b)
{
    const CValue::EType type = CValue::top_type(a.type(), b.type());
    const CValue& l = a.type() == type ? a : a.cast_to(type);
    const CValue& r = b.type() == type ? b : b.cast_to(type);

    switch (type)
    {
        case CValue::ETYPE_INTEGER:
            return l.get_integer() / r.get_integer();

        case CValue::ETYPE_FLOAT:
            return l.get_float() / r.get_float();

        default:
            throw CRuntimeException((std::string("Unexpected type") + TYPE_NAMES[type] + " for operation /").c_str());
    }
}

CValue operator%(const CValue& a, const CValue& b)
{
    const CValue::EType type = CValue::top_type(a.type(), b.type());
    const CValue& l = a.type() == type ? a : a.cast_to(type);
    const CValue& r = b.type() == type ? b : b.cast_to(type);

    switch (type)
    {
        case CValue::ETYPE_INTEGER:
            return l.get_integer() % r.get_integer();

        default:
            throw CRuntimeException((std::string("Unexpected type") + TYPE_NAMES[type] + " for operation /").c_str());
    }
}

CValue operator*(const CValue& a, const CValue& b)
{
    const CValue::EType type = CValue::top_type(a.type(), b.type());
    const CValue& l = a.type() == type ? a : a.cast_to(type);
    const CValue& r = b.type() == type ? b : b.cast_to(type);

    switch (type)
    {
        case CValue::ETYPE_INTEGER:
            return l.get_integer() * r.get_integer();

        case CValue::ETYPE_FLOAT:
            return l.get_float() * r.get_float();

        default:
            throw CRuntimeException((std::string("Unexpected type") + TYPE_NAMES[type] + " for operation *").c_str());
    }
}

CValue& CValue::operator=(const CValue& right)
{
    if (this != &right)
    {
        m_type = right.m_type;
        switch (m_type)
        {
            case ETYPE_INTEGER:
                m_value.i = right.m_value.i;
                break;

            case ETYPE_FLOAT:
                m_value.f = right.m_value.f;
                break;

            case ETYPE_BOOLEAN:
                m_value.b = right.m_value.b;
                break;

            case ETYPE_STRING:
                m_string = right.m_string;
                m_value.s = m_string.c_str();
                break;

            case ETYPE_UNDEFINED:
                break;

            default:
                throw CParserException("Unexpected type of value");
        }
    }

    return *this;
}

std::ostream& operator<<(std::ostream& os, const CValue& v)
{
    switch (v.type())
    {
        case CValue::ETYPE_INTEGER:
            return os << v.get_integer();
        case CValue::ETYPE_FLOAT:
            return os << v.get_float();
        case CValue::ETYPE_BOOLEAN:
            return os << v.get_boolean();
        case CValue::ETYPE_STRING:
            return os << "\"" << v.get_string() << "\"";
        case CValue::ETYPE_UNDEFINED:
            return os << "<Undefined value>";
    }
}

CValue::CValue(const CValue& from)
{
    *this = from;
}

void CVariables::add_variable(const std::string& name, const CValue& value)
{
    m_variables[name] = CVariable(name, value);
}

const CVariable& CVariables::get_variable(const std::string& name) const
{
    variables_map_t::const_iterator v = m_variables.find(name);
    if (m_variables.end() == v)
    {
        throw CParserException("Variable is not defined");
    }
    return v->second;
}

class CParserImpl
{
    public:
        typedef std::list<scanner::CToken> tokens_list_t;

        CParserImpl() {}
        bool parse(const std::string& expression);
        CValue evaluate() const { return m_tree.value(); }

    private:
        CTree m_tree;
};

CParserImpl* CParser::impl()
{
    if (!m_impl)
    {
        m_impl = new CParserImpl();
        if (NULL == m_impl)
        {
            throw CParserException("Cannot create instance of CParserImpl()");
        }
    }
    return m_impl;
}

bool CParserImpl::parse(const std::string& line)
{
    void* parser;
    size_t offset = 0;
    SState state = {offset: 0, syntax_error: 0, tree: &m_tree};

    if (line.empty())
    {
        std::cerr << "\033[1;31mLine may not be empty\033[0m" << std::endl;
        return false;
    }

    parser = ParseAlloc(malloc);
    std::cout << "start parsing '" << line << "'" << std::endl;
    std::list<CParserNode*> nodes;
    for (scanner::CToken t = scanner::scan(line, offset);
            ET_ERROR != t.token() && ET_EOF != t.token();
            t = scanner::scan(line, offset))
    {
        CParserNode* node = CTreeNodeFactory::createParserNode(t.value());
        Parse(parser, t.token(), node, &state);
        if (state.syntax_error)
        {
            break;
        }
        state.offset = offset;
        nodes.push_back(node);
    }
    if (!state.syntax_error)
    {
        Parse(parser, ET_END, ET_END, &state);
    }
    if (state.syntax_error)
    {
        std::cerr << "\033[0;32mSyntax error at offset " << state.offset << ":\033[0m" << std::endl;
        std::string msg = line;
        msg.insert(state.offset, "\033[1;34m");
        msg = "'" + msg;
        msg.append("\033[0m'");
        std::cerr << msg << std::endl;
    }
    ParseFree(parser, free);

    while (!nodes.empty())
    {
        delete nodes.front();
        nodes.pop_front();
    }

    return false;
}

CValue CParser::evaluate()
{
    return impl()->evaluate();
}

bool CParser::parse(const std::string& expression)
{
    try
    {
        bool result = impl()->parse(expression);
        m_state = result ? EPS_READY : EPS_ERROR;
    } catch (const CParserException& e)
    {
        m_state = EPS_UNDEFINED;
    }

    return EPS_READY == m_state;
}
}

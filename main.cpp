#include "parser.h"
#include "colors.h"

#ifdef USE_READLINE
#   include <readline/readline.h>
#endif

#include <cmath>
#include <iostream>
#include <string>
#include <iomanip>

int main(int argc, char **argv)
{
    parser::CParser parser;
    parser.add_variable("pi", static_cast<long double>(M_PI));

    do
    {
        std::string line;
#ifdef USE_READLINE
        const char* l = readline("expression> ");
        if (l)
        {
            line = l;
        }
        else
        {
            std::cout << std::endl;
            break;
        }
#else
        std::getline(std::cin, line);
        if (std::cin.eof())
        {
            break;
        }
#endif

        parser.clear_tree();
        parser.parse(line);
#ifdef DUMP_TREE
        parser.dump_tree(std::cout);
#endif
        if (parser.ready())
        {
            try
            {
                std::cout << "Value: " GREEN << std::setprecision(16) << parser.evaluate() << RESET << std::endl;
            } catch (const parser::CRuntimeException& e)
            {
                std::cerr << RED "Runtime error: " << e.what() << RESET << std::endl;
            }
        }
    } while (true);

    return 0;
}

#include "parser.h"
#include "colors.h"

#include <iostream>
#include <string>

int main(int argc, char **argv)
{
    parser::CParser parser;

    do
    {
        std::string line;
        std::getline(std::cin, line);
        if (std::cin.eof())
        {
            break;
        }

        parser.clear_tree();
        parser.parse(line);
#ifdef DUMP_TREE
        parser.dump_tree(std::cout);
#endif
        if (parser.ready())
        {
            try
            {
                std::cout << "Value: " GREEN << parser.evaluate() << RESET << std::endl;
            } catch (const parser::CRuntimeException& e)
            {
                std::cerr << RED "Runtime error: " << e.what() << RESET << std::endl;
            }
        }
    } while (true);

    return 0;
}

#include "parser.h"
#include "colors.h"

#include <iostream>
#include <string>

int main(int argc, char **argv)
{
    parser::CParser parser;
    parser.add_variable("i", 123ll);
    parser.add_variable("n", 3ll);
    parser.add_variable("f", 123.456l);
    parser.add_variable("bt", true);
    parser.add_variable("bf", false);
    parser.add_variable("s", std::string("hello world!!!"));

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

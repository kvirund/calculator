#include "parser.h"

#include <iostream>
#include <string>

int main(int argc, char **argv)
{
    do
    {
        std::string line;
        std::getline(std::cin, line);
        if (std::cin.eof())
        {
            break;
        }
        parser::CParser parser;

        parser.add_variable("i", 123ll);
        parser.add_variable("n", 3ll);
        parser.add_variable("f", 123.456l);
        parser.add_variable("bt", true);
        parser.add_variable("bf", false);
        parser.add_variable("s", "hello world!!!");

        parser.parse(line);
#ifdef DUMP_TREE
        parser.dump_tree(std::cerr);
#endif
        if (parser.ready())
        {
            try
            {
                std::cerr << "Value: " << parser.evaluate() << std::endl;
            } catch (const parser::CRuntimeException& e)
            {
                std::cerr << "\033[1;31mRuntime error: " << e.what() << "\033[0m" << std::endl;
            }
        }
    } while (true);

    return 0;
}

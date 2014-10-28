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
        parser.parse(line);
    } while (true);

    return 0;
}

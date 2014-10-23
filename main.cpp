#include "scanner.h"

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
        std::cerr << line << std::endl;

        EToken t;
        const char *p = line.c_str();
        const char *begin = p;
        const char *s;
        do
        {
            t = scan(&p, &s);
            size_t length = p - s;
            size_t offset = s - begin;

            switch (t)
            {
                case ET_IDENTIFIER:
                    std::cerr << "IDENTIFIER";
                    std::cerr << '(' << line.substr(offset, length) << ") ";
                    break;

                case ET_OPERATION:
                    std::cerr << "OPERATION";
                    std::cerr << '(' << line.substr(offset, length) << ") ";
                    break;

                case ET_BOOLEAN:
                    std::cerr << "BOOLEAN";
                    std::cerr << '(' << line.substr(offset, length) << ") ";
                    break;

                case ET_INTEGER:
                    std::cerr << "INTEGER";
                    std::cerr << '(' << line.substr(offset, length) << ") ";
                    break;

                case ET_FLOAT:
                    std::cerr << "FLOAT";
                    std::cerr << '(' << line.substr(offset, length) << ") ";
                    break;

                case ET_STRING:
                    std::cerr << "STRING";
                    std::cerr << '(' << line.substr(offset, length) << ") ";
                    break;

                case ET_LEFT_PARENTHESIS:
                    std::cerr << "LEFT_PARENTHESIS ";
                    break;

                case ET_RIGHT_PARENTHESIS:
                    std::cerr << "RIGHT_PARENTHESIS ";
                    break;

                case ET_SPACE:
                    // do nothing
                    break;

                case ET_EOL:
                    std::cerr << "EOL ";
                    break;

                case ET_END:
                    std::cerr << "END" << std::endl;
                    break;

                default:
                    std::cerr << "Unexpected token" << static_cast<int>(t) << std::endl;
            }
        } while (ET_ERROR != t && ET_END != t);
    } while (true);

    return 0;
}

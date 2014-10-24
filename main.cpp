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

        scanner::CToken t;
        size_t offset = 0;
        do
        {
            t = scanner::scan(line, offset);
            
            switch (t.token())
            {
                case scanner::ET_IDENTIFIER:
                    std::cerr << "IDENTIFIER";
                    std::cerr << '(' << t.value() << ") ";
                    break;

                case scanner::ET_OPERATION:
                    std::cerr << "OPERATION";
                    std::cerr << '(' << t.value() << ") ";
                    break;

                case scanner::ET_BOOLEAN:
                    std::cerr << "BOOLEAN";
                    std::cerr << '(' << t.value() << ") ";
                    break;

                case scanner::ET_INTEGER:
                    std::cerr << "INTEGER";
                    std::cerr << '(' << t.value() << ") ";
                    break;

                case scanner::ET_FLOAT:
                    std::cerr << "FLOAT";
                    std::cerr << '(' << t.value() << ") ";
                    break;

                case scanner::ET_STRING:
                    std::cerr << "STRING";
                    std::cerr << '(' << t.value() << ") ";
                    break;

                case scanner::ET_LEFT_PARENTHESIS:
                    std::cerr << "LEFT_PARENTHESIS ";
                    break;

                case scanner::ET_RIGHT_PARENTHESIS:
                    std::cerr << "RIGHT_PARENTHESIS ";
                    break;

                case scanner::ET_SPACE:
                    // do nothing
                    break;

                case scanner::ET_EOL:
                    std::cerr << "EOL ";
                    break;

                case scanner::ET_END:
                    std::cerr << "END" << std::endl;
                    break;

                default:
                    std::cerr << "Unexpected token" << static_cast<int>(t.token()) << std::endl;
            }
        } while (scanner::ET_ERROR != t.token() && scanner::ET_END != t.token());
    } while (true);

    return 0;
}

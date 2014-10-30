#include "parser.h"

#include <iostream>
#include <string>

int main(int argc, char **argv)
{
    parser::CTreeNode* node1 = parser::CTreeNodeFactory::createVariableNode("asdf");
    parser::CTreeNode* node2 = parser::CTreeNodeFactory::createVariableNode("asdf");
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
        std::cout << "Value: " << parser.evaluate() << std::endl;
    } while (true);

    return 0;
}

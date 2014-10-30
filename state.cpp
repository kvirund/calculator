#include "state.h"

#include "parser.h"

typedef parser::CTreeNodeFactory parser_factory;

extern "C"
{
void* create_identifier(void* n, void* vp)
{
    parser::CParserNode* node = static_cast<parser::CParserNode*>(n);
    const std::string name = node->as_string();
    const parser::CVariables* vpool = static_cast<parser::CVariables*>(vp);
    parser::CVariableTreeNode* new_node = parser_factory::createVariableNode(name, vpool);
    return new_node;
}

void* create_integer(void* n)
{
    parser::CParserNode* node = static_cast<parser::CParserNode*>(n);
    parser::CConstantTreeNode* new_node = parser_factory::createConstantNode(node->as_integer());
    return new_node;
}

void* create_float(void* n)
{
    parser::CParserNode* node = static_cast<parser::CParserNode*>(n);
    parser::CConstantTreeNode* new_node = parser_factory::createConstantNode(static_cast<long double>(node->as_float()));
    return new_node;
}

void* create_boolean(void* n)
{
    parser::CParserNode* node = static_cast<parser::CParserNode*>(n);
    parser::CConstantTreeNode* new_node = parser_factory::createConstantNode(node->as_boolean());
    return new_node;
}

void* create_string(void* n)
{
    parser::CParserNode* node = static_cast<parser::CParserNode*>(n);
    parser::CConstantTreeNode* new_node = parser_factory::createConstantNode(node->as_string());
    return new_node;
}

void* create_less_operator(void* a, void* b)
{
    parser::CTreeNode* node_a = static_cast<parser::CTreeNode*>(a);
    parser::CTreeNode* node_b = static_cast<parser::CTreeNode*>(b);
    parser::CTreeNode* new_node = parser_factory::createLessOperator(node_a, node_b);
    return new_node;
}

void* create_less_or_equal_operator(void* a, void* b)
{
    parser::CTreeNode* node_a = static_cast<parser::CTreeNode*>(a);
    parser::CTreeNode* node_b = static_cast<parser::CTreeNode*>(b);
    parser::CTreeNode* new_node = parser_factory::createLessOrEqualOperator(node_a, node_b);
    return new_node;
}

void* create_greater_operator(void* a, void* b)
{
    parser::CTreeNode* node_a = static_cast<parser::CTreeNode*>(a);
    parser::CTreeNode* node_b = static_cast<parser::CTreeNode*>(b);
    parser::CTreeNode* new_node = parser_factory::createGreaterOperator(node_a, node_b);
    return new_node;
}

void* create_greater_or_equal_operator(void* a, void* b)
{
    parser::CTreeNode* node_a = static_cast<parser::CTreeNode*>(a);
    parser::CTreeNode* node_b = static_cast<parser::CTreeNode*>(b);
    parser::CTreeNode* new_node = parser_factory::createGreaterOrEqualOperator(node_a, node_b);
    return new_node;
}

void* create_equal_operator(void* a, void* b)
{
    parser::CTreeNode* node_a = static_cast<parser::CTreeNode*>(a);
    parser::CTreeNode* node_b = static_cast<parser::CTreeNode*>(b);
    parser::CTreeNode* new_node = parser_factory::createEqualOperator(node_a, node_b);
    return new_node;
}

void* create_not_equal_operator(void* a, void* b)
{
    parser::CTreeNode* node_a = static_cast<parser::CTreeNode*>(a);
    parser::CTreeNode* node_b = static_cast<parser::CTreeNode*>(b);
    parser::CTreeNode* new_node = parser_factory::createNotEqualOperator(node_a, node_b);
    return new_node;
}

void* create_add_operator(void* a, void* b)
{
    parser::CTreeNode* node_a = static_cast<parser::CTreeNode*>(a);
    parser::CTreeNode* node_b = static_cast<parser::CTreeNode*>(b);
    parser::CTreeNode* new_node = parser_factory::createAddOperator(node_a, node_b);
    return new_node;
}

void* create_sub_operator(void* a, void* b)
{
    parser::CTreeNode* node_a = static_cast<parser::CTreeNode*>(a);
    parser::CTreeNode* node_b = static_cast<parser::CTreeNode*>(b);
    parser::CTreeNode* new_node = parser_factory::createSubOperator(node_a, node_b);
    return new_node;
}

void* create_div_operator(void* a, void* b)
{
    parser::CTreeNode* node_a = static_cast<parser::CTreeNode*>(a);
    parser::CTreeNode* node_b = static_cast<parser::CTreeNode*>(b);
    parser::CTreeNode* new_node = parser_factory::createDivOperator(node_a, node_b);
    return new_node;
}

void* create_mod_operator(void* a, void* b)
{
    parser::CTreeNode* node_a = static_cast<parser::CTreeNode*>(a);
    parser::CTreeNode* node_b = static_cast<parser::CTreeNode*>(b);
    parser::CTreeNode* new_node = parser_factory::createModOperator(node_a, node_b);
    return new_node;
}

void* create_times_operator(void* a, void* b)
{
    parser::CTreeNode* node_a = static_cast<parser::CTreeNode*>(a);
    parser::CTreeNode* node_b = static_cast<parser::CTreeNode*>(b);
    parser::CTreeNode* new_node = parser_factory::createTimesOperator(node_a, node_b);
    return new_node;
}

void* create_minus_operator(void* n)
{
    parser::CTreeNode* node = static_cast<parser::CTreeNode*>(n);
    parser::CTreeNode* new_node = parser_factory::createNegativeOperator(node);
    return new_node;
}

void set_root(void* tree, void* root)
{
    static_cast<parser::CTree*>(tree)->root(static_cast<parser::CTreeNode*>(root));
}

void delete_node(void* node)
{
    delete static_cast<parser::CTreeNode*>(node);
}
}

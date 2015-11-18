#include "state.h"

#include "parser.h"

typedef parser::CTreeNodeFactory parser_factory;

namespace grammar 
{
void add_to_temp_set(void *n, void* ts)
{
    parser::CTreeNode* node = static_cast<parser::CTreeNode*>(n);
    parser::CTreeNode::ptr_set_t* set = static_cast<parser::CTreeNode::ptr_set_t*>(ts);
    set->insert(node);
}

void remove_from_temp_set(void* n, void* ts)
{
    parser::CTreeNode* node = static_cast<parser::CTreeNode*>(n);
    parser::CTreeNode::ptr_set_t* set = static_cast<parser::CTreeNode::ptr_set_t*>(ts);
    set->erase(node);
}

void* create_identifier(void* n, void* vp)
{
    parser::CParserNode* node = static_cast<parser::CParserNode*>(n);
    const std::string name = node->as_string();
    parser::CVariable* vpool = static_cast<parser::CVariable*>(vp);
    parser::CVariableTreeNode* new_node = parser_factory::createVariableNode(name, *vpool);
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
    parser::CConstantTreeNode* new_node = parser_factory::createConstantNode(static_cast<long double>(node->as_double()));
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

void* create_null()
{
    return parser_factory::createConstantNode(static_cast<void*>(NULL));
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

void* create_and_operator(void* a, void* b)
{
    parser::CTreeNode* node_a = static_cast<parser::CTreeNode*>(a);
    parser::CTreeNode* node_b = static_cast<parser::CTreeNode*>(b);
    parser::CTreeNode* new_node = parser_factory::createAndOperator(node_a, node_b);
    return new_node;
}

void* create_or_operator(void* a, void* b)
{
    parser::CTreeNode* node_a = static_cast<parser::CTreeNode*>(a);
    parser::CTreeNode* node_b = static_cast<parser::CTreeNode*>(b);
    parser::CTreeNode* new_node = parser_factory::createOrOperator(node_a, node_b);
    return new_node;
}

void* create_access_operator(void* a, void* b)
{
    parser::CObjectTreeNode* node_a = static_cast<parser::CObjectTreeNode*>(a);
    parser::CParserNode* node = static_cast<parser::CParserNode*>(b);
    const std::string name = node->value().get_string();
    parser::CTreeNode* new_node = parser_factory::createAccessOperator(node_a, name);
    return new_node;
}

void* create_like_operator(void* a, void* b)
{
    parser::CTreeNode* node_a = static_cast<parser::CTreeNode*>(a);
    parser::CTreeNode* node_b = static_cast<parser::CTreeNode*>(b);
    parser::CTreeNode* new_node = parser_factory::createLikeOperator(node_a, node_b);
    return new_node;
}

void* create_is_operator(void* a, void* b)
{
    parser::CTreeNode* node_a = static_cast<parser::CTreeNode*>(a);
    parser::CTreeNode* node_b = static_cast<parser::CTreeNode*>(b);
    parser::CTreeNode* new_node = parser_factory::createIsOperator(node_a, node_b);
    return new_node;
}

void* create_not_operator(void* n)
{
    parser::CTreeNode* node = static_cast<parser::CTreeNode*>(n);
    parser::CTreeNode* new_node = parser_factory::createNotOperator(node);
    return new_node;
}

void delete_node(void* node)
{
    delete static_cast<parser::CTreeNode*>(node);
}

void* create_assignment_operator(void* a, void* b)
{
    parser::CObjectTreeNode* var = static_cast<parser::CObjectTreeNode*>(a);
    parser::CTreeNode* expr = static_cast<parser::CTreeNode*>(b);
    return parser_factory::createAssignmentOperator(var, expr);
}

}
/* vim: set ts=4 sw=4 tw=0 et syntax=cpp :*/

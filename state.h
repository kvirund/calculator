#ifndef __STATE_H__
#define __STATE_H__

struct SState
{
    int offset;
    int syntax_error;
    void* tree;
};

#ifdef __cplusplus
extern "C" {
#endif
void* create_identifier(void* node);
void* create_integer(void* node);
void* create_float(void* node);
void* create_boolean(void* node);
void* create_string(void* node);
void* create_less_operator(void* a, void* b);
void* create_less_or_equal_operator(void* a, void* b);
void* create_greater_operator(void* a, void* b);
void* create_greater_or_equal_operator(void* a, void* b);
void* create_equal_operator(void* a, void* b);
void* create_not_equal_operator(void* a, void* b);
void* create_add_operator(void* a, void* b);
void* create_add_operator(void* a, void* b);
void* create_sub_operator(void* a, void* b);
void* create_div_operator(void* a, void* b);
void* create_mod_operator(void* a, void* b);
void* create_times_operator(void* a, void* b);
void* create_minus_operator(void* n);
void set_root(void* tree, void* root);
void delete_node(void* node);
#ifdef __cplusplus
}
#endif

#endif

#ifndef __PARSER_H__
#define __PARSER_H__

#include <strings.h>

#include <cstdlib>
#include <iostream>
#include <string>
#include <list>
#include <map>
#include <set>

namespace parser
{
inline std::ostream& indent(std::ostream& os, int l)
{
    while (0 < l)
    {
        os << "\t";
        l--;
    }
    return os;
}

class CCommonException: public std::exception
{
    public:
        CCommonException(const char* message): m_what(message) {}
        const char* what() const throw() { return m_what; }

    private:
        const char* m_what;
};

class CParserException: public CCommonException
{
    public:
        CParserException(const char* message): CCommonException(message) {}
};

class CRuntimeException: public CCommonException
{
    public:
        CRuntimeException(const char* message): CCommonException(message) {}
};

enum EParserState
{
    EPS_UNDEFINED,
    EPS_ERROR,
    EPS_READY
};

#define TYPES   TOKEN(ETYPE_UNDEFINED) \
    TOKEN(ETYPE_INTEGER) \
    TOKEN(ETYPE_FLOAT) \
    TOKEN(ETYPE_BOOLEAN) \
    TOKEN(ETYPE_STRING)

class CValue
{
    public:
        enum EType
        {
#define TOKEN(x) x,
            TYPES
#undef TOKEN
        };

        CValue(): m_type(ETYPE_UNDEFINED) {}
        CValue(long long int value): m_type(ETYPE_INTEGER) { m_value.i = value; }
        CValue(long double value): m_type(ETYPE_FLOAT) { m_value.f = value; }
        CValue(bool value): m_type(ETYPE_BOOLEAN) { m_value.b = value; }
        CValue(const std::string& value): m_type(ETYPE_STRING), m_string(value) { m_value.s = m_string.c_str(); }
        CValue& operator=(const CValue& right);
        CValue(const CValue& from);

        EType type() const { return m_type; }
        long long int get_integer() const;
        long double get_float() const;
        bool get_boolean() const;
        const std::string& get_string() const;
        CValue cast_to(EType type) const;
        void dump(std::ostream& os) const;

        static EType top_type(EType a, EType b);

    private:
        union UValue
        {
            long long int i;
            long double f;
            bool b;
            const char* s;
        };

        EType m_type;
        UValue m_value;
        std::string m_string;
};

CValue operator<(const CValue& a, const CValue& b);
CValue operator<=(const CValue& a, const CValue& b);
CValue operator==(const CValue& a, const CValue& b);
CValue operator!=(const CValue& a, const CValue& b);
CValue operator>(const CValue& a, const CValue& b);
CValue operator>=(const CValue& a, const CValue& b);
CValue operator+(const CValue& a, const CValue& b);
CValue operator-(const CValue& a, const CValue& b);
CValue operator-(const CValue& a);
CValue operator/(const CValue& a, const CValue& b);
CValue operator%(const CValue& a, const CValue& b);
CValue operator*(const CValue& a, const CValue& b);

std::ostream& operator<<(std::ostream& os, const CValue& v);

class CVariable
{
    public:
        CVariable(): m_name("") {}
        CVariable(const std::string& name): m_name(name) {}
        CVariable(const std::string& name, const CValue& value): m_name(name), m_value(value) {}
        const CValue& value() const { return m_value; }
        void dump(std::ostream& os) const
        {
            os << m_name << "=";
            m_value.dump(os);
        }

    private:
        std::string m_name;
        CValue m_value;
};

class CVariables
{
    public:
        CVariables() {}
        void add_variable(const std::string& name, const CValue& value);
        const CVariable& get_variable(const std::string& name) const;
        bool is_variable_set(const std::string& name) const { return m_variables.end() != m_variables.find(name); }
        void clear() { m_variables.clear(); }

    private:
        typedef std::map<std::string, CVariable> variables_map_t;

        variables_map_t m_variables;
};

class CTreeNode
{
    public:
        typedef std::set<CTreeNode*> ptr_set_t;
        virtual CValue value() const = 0;
        virtual void dump(std::ostream& os, int indent = 0) const = 0;
        virtual ~CTreeNode() {};
};

class CParserNode: public CTreeNode
{
    public:
        CParserNode(const std::string& value): m_value(value) {}
        CValue value() const { return m_value; }
        long long int as_integer() const { return atoi(m_value.get_string().c_str()); }
        float as_float() const { return atof(m_value.get_string().c_str()); }
        bool as_boolean() const { return 0 == strcasecmp("true", m_value.get_string().c_str()); }
        const std::string& as_string() const { return m_value.get_string(); }
        void dump(std::ostream& os, int i = 0) const { indent(os, i) << "CParserNode: "; m_value.dump(os); os << std::endl; }

    private:
        CValue m_value;
};

class CLeafTreeNode: public CTreeNode
{
};

class CConstantTreeNode: public CLeafTreeNode
{
    public:
        explicit CConstantTreeNode(long long int value): m_value(value) {}
        explicit CConstantTreeNode(long double value): m_value(value) {}
        explicit CConstantTreeNode(bool value): m_value(value) {}
        explicit CConstantTreeNode(const std::string& value): m_value(value) {}
        CValue value() const { return m_value; }
        void dump(std::ostream& os, int i = 0) const { indent(os, i) << "CConstantTreeNode: "; m_value.dump(os); os << std::endl; }

    private:
        CValue m_value;
};

class CVariableTreeNode: public CLeafTreeNode
{
    public:
        CVariableTreeNode(const std::string& name, const CVariables* vpool): m_name(name), m_variables_pool(vpool) {}
        CValue value() const { return m_variables_pool->get_variable(m_name).value(); }
        void dump(std::ostream& os, int i = 0) const;

    private:
        const std::string m_name;
        const CVariables* m_variables_pool;
};

class COperatorTreeNode: public CTreeNode
{
    public:
        virtual void add_argument(const CTreeNode* node) = 0;
        void dump(std::ostream& os, int i = 0) const;
        ~COperatorTreeNode();

    protected:
        typedef std::list<const CTreeNode*> arguments_list_t;

        arguments_list_t m_arguments;
};

class CUnaryOperator: public COperatorTreeNode
{
    public:
        void add_argument(const CTreeNode* node)
        {
            if (!m_arguments.empty())
            {
                throw CParserException("So much arguments for unary operator");
            }
            m_arguments.push_back(node);
        }
};

class CNegativeOperator: public CUnaryOperator
{
    public:
        CValue value() const
        {
            if (m_arguments.empty())
            {
                throw CRuntimeException("Not enough arguments");
            }
            const CTreeNode* v = m_arguments.front();
            return -(v->value());
        }
        void dump(std::ostream& os, int i = 0) const
        {
            indent(os, i) << "CNegativeOperator: " << m_arguments.size() << " operands" << std::endl;
            COperatorTreeNode::dump(os, 1 + i);
        }
};

class CBinaryOperator: public COperatorTreeNode
{
    public:
        void add_argument(const CTreeNode* node)
        {
            if (2 < m_arguments.size())
            {
                throw CParserException("So much arguments for binary operator");
            }
            m_arguments.push_back(node);
        }
};

class CAddOperator: public CBinaryOperator
{
    public:
        CValue value() const
        {
            if (2 != m_arguments.size())
            {
                throw CRuntimeException("Not enough arguments");
            }
            return m_arguments.front()->value() + m_arguments.back()->value();
        }
        void dump(std::ostream& os, int i = 0) const
        {
            indent(os, i) << "CAddOperator: " << m_arguments.size() << " operands" << std::endl;
            COperatorTreeNode::dump(os, 1 + i);
        }
};

class CSubOperator: public CBinaryOperator
{
    public:
        CValue value() const
        {
            if (2 != m_arguments.size())
            {
                throw CRuntimeException("Not enough arguments");
            }
            return m_arguments.front()->value() - m_arguments.back()->value();
        }
        void dump(std::ostream& os, int i = 0) const
        {
            indent(os, i) << "CSubOperator: " << m_arguments.size() << " operands" << std::endl;
            COperatorTreeNode::dump(os, 1 + i);
        }
};

class CDivOperator: public CBinaryOperator
{
    public:
        CValue value() const
        {
            if (2 != m_arguments.size())
            {
                throw CRuntimeException("Not enough arguments");
            }
            return m_arguments.front()->value() / m_arguments.back()->value();
        }
        void dump(std::ostream& os, int i = 0) const
        {
            indent(os, i) << "CDivOperator: " << m_arguments.size() << " operands" << std::endl;
            COperatorTreeNode::dump(os, 1 + i);
        }
};

class CModOperator: public CBinaryOperator
{
    public:
        CValue value() const
        {
            if (2 != m_arguments.size())
            {
                throw CRuntimeException("Not enough arguments");
            }
            return m_arguments.front()->value() % m_arguments.back()->value();
        }
        void dump(std::ostream& os, int i = 0) const
        {
            indent(os, i) << "CModOperator: " << m_arguments.size() << " operands" << std::endl;
            COperatorTreeNode::dump(os, 1 + i);
        }
};

class CTimesOperator: public CBinaryOperator
{
    public:
        CValue value() const
        {
            if (2 != m_arguments.size())
            {
                throw CRuntimeException("Not enough arguments");
            }
            return m_arguments.front()->value() * m_arguments.back()->value();
        }
        void dump(std::ostream& os, int i = 0) const
        {
            indent(os, i) << "CTimesOperator: " << m_arguments.size() << " operands" << std::endl;
            COperatorTreeNode::dump(os, 1 + i);
        }
};

class CLessOperator: public CBinaryOperator
{
    public:
        CValue value() const
        {
            if (2 != m_arguments.size())
            {
                throw CRuntimeException("Not enough arguments");
            }
            return m_arguments.front()->value() < m_arguments.back()->value();
        }
        void dump(std::ostream& os, int i = 0) const
        {
            indent(os, i) << "CLessOperator: " << m_arguments.size() << " operands" << std::endl;
            COperatorTreeNode::dump(os, 1 + i);
        }
};

class CLessOrEqualOperator: public CBinaryOperator
{
    public:
        CValue value() const
        {
            if (2 != m_arguments.size())
            {
                throw CRuntimeException("Not enough arguments");
            }
            return m_arguments.front()->value() <= m_arguments.back()->value();
        }
        void dump(std::ostream& os, int i = 0) const
        {
            indent(os, i) << "CLessOrEqualOperator: " << m_arguments.size() << " operands" << std::endl;
            COperatorTreeNode::dump(os, 1 + i);
        }
};

class CEqualOperator: public CBinaryOperator
{
    public:
        CValue value() const
        {
            if (2 != m_arguments.size())
            {
                throw CRuntimeException("Not enough arguments");
            }
            return m_arguments.front()->value() == m_arguments.back()->value();
        }
        void dump(std::ostream& os, int i = 0) const
        {
            indent(os, i) << "CEqualOperator: " << m_arguments.size() << " operands" << std::endl;
            COperatorTreeNode::dump(os, 1 + i);
        }
};

class CNotEqualOperator: public CBinaryOperator
{
    public:
        CValue value() const
        {
            if (2 != m_arguments.size())
            {
                throw CRuntimeException("Not enough arguments");
            }
            return m_arguments.front()->value() != m_arguments.back()->value();
        }
        void dump(std::ostream& os, int i = 0) const
        {
            indent(os, i) << "CNotEqualOperator: " << m_arguments.size() << " operands" << std::endl;
            COperatorTreeNode::dump(os, 1 + i);
        }
};

class CGreaterOperator: public CBinaryOperator
{
    public:
        CValue value() const
        {
            if (2 != m_arguments.size())
            {
                throw CRuntimeException("Not enough arguments");
            }
            return m_arguments.front()->value() > m_arguments.back()->value();
        }
        void dump(std::ostream& os, int i = 0) const
        {
            indent(os, i) << "CGreaterOperator: " << m_arguments.size() << " operands" << std::endl;
            COperatorTreeNode::dump(os, 1 + i);
        }
};

class CGreaterOrEqualOperator: public CBinaryOperator
{
    public:
        CValue value() const
        {
            if (2 != m_arguments.size())
            {
                throw CRuntimeException("Not enough arguments");
            }
            return m_arguments.front()->value() >= m_arguments.back()->value();
        }
        void dump(std::ostream& os, int i = 0) const
        {
            indent(os, i) << "CGreaterOrEqualOperator: " << m_arguments.size() << " operands" << std::endl;
            COperatorTreeNode::dump(os, 1 + i);
        }
};

class CTree
{
    public:
        CTree(): m_root(NULL) {}

        void root(CTreeNode* node) { m_root = node; }
        const CTreeNode* root() const { return m_root; }
        CValue value() const;
        void dump(std::ostream& os, int i = 0) const;
        void clear();

        ~CTree()
        {
            if (m_root)
            {
                delete m_root;
            }
            m_root = NULL;
        }

    private:
        CTree(const CTree&);
        CTree& operator=(const CTree&);
        const CTree& operator=(const CTree&) const;

        CTreeNode* m_root;
};

class CTreeNodeFactory
{
    public:
        static CParserNode* createParserNode(const std::string& value) { return new CParserNode(value); }
        static CConstantTreeNode* createConstantNode(long long int value) { return new CConstantTreeNode(value); }
        static CConstantTreeNode* createConstantNode(long double value) { return new CConstantTreeNode(value); }
        static CConstantTreeNode* createConstantNode(bool value) { return new CConstantTreeNode(value); }
        static CConstantTreeNode* createConstantNode(const std::string& value) { return new CConstantTreeNode(value); }
        static CVariableTreeNode* createVariableNode(const std::string& name, const CVariables* vpool) { return new CVariableTreeNode(name, vpool); }
        static CLessOperator* createLessOperator(const CTreeNode* left, const CTreeNode* right) { return createBinaryOperator<CLessOperator>(left, right); }
        static CLessOrEqualOperator* createLessOrEqualOperator(const CTreeNode* left, const CTreeNode* right) { return createBinaryOperator<CLessOrEqualOperator>(left, right); }
        static CEqualOperator* createEqualOperator(const CTreeNode* left, const CTreeNode* right) { return createBinaryOperator<CEqualOperator>(left, right); }
        static CNotEqualOperator* createNotEqualOperator(const CTreeNode* left, const CTreeNode* right) { return createBinaryOperator<CNotEqualOperator>(left, right); }
        static CGreaterOperator* createGreaterOperator(const CTreeNode* left, const CTreeNode* right) { return createBinaryOperator<CGreaterOperator>(left, right); }
        static CGreaterOrEqualOperator* createGreaterOrEqualOperator(const CTreeNode* left, const CTreeNode* right) { return createBinaryOperator<CGreaterOrEqualOperator>(left, right); }
        static CAddOperator* createAddOperator(const CTreeNode* left, const CTreeNode* right) { return createBinaryOperator<CAddOperator>(left, right); }
        static CSubOperator* createSubOperator(const CTreeNode* left, const CTreeNode* right) { return createBinaryOperator<CSubOperator>(left, right); }
        static CNegativeOperator* createNegativeOperator(const CTreeNode* node) { CNegativeOperator* o = new CNegativeOperator(); o->add_argument(node); return o; }
        static CDivOperator* createDivOperator(const CTreeNode* left, const CTreeNode* right) { return createBinaryOperator<CDivOperator>(left, right); }
        static CModOperator* createModOperator(const CTreeNode* left, const CTreeNode* right) { return createBinaryOperator<CModOperator>(left, right); }
        static CTimesOperator* createTimesOperator(const CTreeNode* left, const CTreeNode* right) { return createBinaryOperator<CTimesOperator>(left, right); }

    private:
        template <class T>
        static T* createBinaryOperator(const CTreeNode* left, const CTreeNode* right)
        {
            T* node = new T();
            node->add_argument(left);
            node->add_argument(right);
            return node;
        }
};

class CParser
{
    public:
        CParser(): m_state(EPS_UNDEFINED) {}
        bool parse(const std::string& expression);
        void dump_tree(std::ostream& os) const;
        CValue evaluate() const { return m_tree.value(); };
        void add_variable(const std::string& name, const CValue& value) { m_variables_pool.add_variable(name, value); };
        void clear_variables() { m_variables_pool.clear(); }
        void clear_tree() { m_tree.clear(); }
        void reset();
        bool ready() const { return EPS_READY == m_state; }
        ~CParser() { reset(); }

    private:
        EParserState m_state;
        CTree m_tree;
        CVariables m_variables_pool;
};

inline void CParser::reset()
{
    clear_variables();
    clear_tree();
}
}

#endif

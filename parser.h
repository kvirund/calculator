#ifndef __PARSER_H__
#define __PARSER_H__

#include "expressions.scanner.h"
#include "where.scanner.h"
#include "parser_utils.h"

#include <string.h>

#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <string>
#include <list>
#include <map>
#include <set>

namespace scanner
{
    namespace expressions
    {
        class CToken;
    }
    namespace where
    {
        class CToken;
    }
}

namespace grammar
{
    struct SState;

    namespace expressions
    {
        void *ParseAlloc(void *(*mallocProc)(size_t));
        void Parse( void *yyp, int yymajor, void* yyminor, SState*);
        void ParseFree( void *p, void (*freeProc)(void*));
    }
    namespace where
    {
        void *ParseAlloc(void *(*mallocProc)(size_t));
        void Parse( void *yyp, int yymajor, void* yyminor, SState*);
        void ParseFree( void *p, void (*freeProc)(void*));
    }
}

namespace parser
{
inline std::ostream& indent(std::ostream& os, int l, const char* ind = "\t")
{
    while (0 < l)
    {
        os << ind;
        l--;
    }
    return os;
}

class CCommonException: public std::exception
{
    public:
        CCommonException(const char* message): m_what(message) {}
        const char* what() const throw() { return m_what.c_str(); }
        ~CCommonException() throw() {}

    private:
        const std::string m_what;
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

#define TYPES \
    TOKEN(ETYPE_UNDEFINED) \
    TOKEN(ETYPE_INTEGER) \
    TOKEN(ETYPE_FLOAT) \
    TOKEN(ETYPE_BOOLEAN) \
    TOKEN(ETYPE_STRING) \
    TOKEN(ETYPE_NULL)

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
        CValue(const void*): m_type(ETYPE_NULL) { }
        CValue& operator=(const CValue& right);
        CValue(const CValue& from);

        EType type() const { return m_type; }
        long long int get_integer() const;
        long double get_float() const;
        bool get_boolean() const;
        bool is_null() const { return ETYPE_NULL == m_type; }
        bool is_defined() const { return ETYPE_UNDEFINED != m_type; }
        const std::string& get_string() const;
        CValue cast_to(EType type) const;
        void dump(std::ostream& os) const;

        static EType top_type(EType a, EType b) { return top_type(a, b, true); };
        static EType top_type(EType a, EType b, const bool throw_exception);
        CValue equal_to(const CValue& right, const bool throw_exception) const;

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
CValue operator&&(const CValue& a, const CValue& b);
CValue operator||(const CValue& a, const CValue& b);
CValue operator!(const CValue& a);

std::ostream& operator<<(std::ostream& os, const CValue& v);

class CVariable
{
    public:
        CVariable(): m_name("") {}
        CVariable(const std::string& name): m_name(name) {}
        CVariable(const std::string& name, const CValue& value): m_name(name), m_value(value) {}
        const CValue& value() const { return m_value; }
        const std::string& name() const { return m_name; }
        void value(const CValue& value) { m_value = value; }
        void add_field(const std::string& name, const CValue& value) { m_fields[name] = CVariable(name, value); }
        void add_field(const std::string& name, const CVariable& value) { m_fields[name] = value; }
        CVariable& add_field(const std::string& name) { return m_fields[name]; }
        bool has_field(const std::string& name) const { return m_fields.end() != m_fields.find(name); }
        const CVariable& get_field(const std::string& name) const;
        CVariable& get_field(const std::string& name, const bool create);
        void clear() { m_fields.clear(); }
        void dump(std::ostream& os) const;

    private:
        typedef std::map<std::string, CVariable> fields_t;

        std::string m_name;
        CValue m_value;
        fields_t m_fields;
};

inline
const CVariable& CVariable::get_field(const std::string& name) const
{
    if (!has_field(name))
    {
        std::stringstream ss;
        ss << "Field '" << name << "' does not exist in variable '" << this->name() << "'";
        throw CRuntimeException(ss.str().c_str());
    }
    fields_t::const_iterator i = m_fields.find(name);
    return i->second;
}

inline
CVariable& CVariable::get_field(const std::string& name, const bool create)
{
    if (!has_field(name))
    {
        if (!create)
        {
        std::stringstream ss;
        ss << "Field '" << name << "' does not exist in variable '" << this->name() << "'";
        throw CRuntimeException(ss.str().c_str());
        }
        else
        {
            add_field(name, CVariable(name));
        }
    }
    return m_fields[name];
}

inline
void CVariable::dump(std::ostream& os) const
{
    os << m_name << "=";
    m_value.dump(os);
    os << " fields: ";
    if (m_fields.empty())
    {
        os << "<no fields>";
    }
    else
    {
        os << "(";
        for (fields_t::const_iterator i = m_fields.begin(); i != m_fields.end(); ++i)
        {
            if (m_fields.begin() != i)
            {
                os << ", ";
            }
            os << i->first << ": ";
            i->second.dump(os);
        }
        os << ")";
    }
}

class CTreeNode
{
    public:
        typedef std::set<CTreeNode*> ptr_set_t;
        virtual CValue value() const = 0;
        virtual void dump(std::ostream& os, int indent = 0) const = 0;
        virtual ~CTreeNode() {};
};

inline std::string to_lower(const std::string& s)
{
    std::string var = s;
    std::transform(var.begin(), var.end(), var.begin(), ::tolower);
    return var;
}

class CParserNode: public CTreeNode
{
    public:
        CParserNode(const std::string& value): m_value(value) {}
        CValue value() const { return m_value; }
        long long int as_integer() const { return atoi(m_value.get_string().c_str()); }
        long double as_double() const { return static_cast<long double>( atof(m_value.get_string().c_str()) ); }
        bool as_boolean() const { return to_lower(m_value.get_string()) == "true"; }
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
        explicit CConstantTreeNode(const void* null): m_value(null) {}
        CValue value() const { return m_value; }
        void dump(std::ostream& os, int i = 0) const { indent(os, i) << "CConstantTreeNode: "; m_value.dump(os); os << std::endl; }

    private:
        CValue m_value;
};

class CObjectTreeNode: public CTreeNode
{
    protected:
        CObjectTreeNode() {}

    public:
        virtual void dump(std::ostream& os, int i = 0) const = 0;
        virtual const CVariable& get() const = 0;
        virtual CVariable& get(const bool create) = 0;
};

class CVariableTreeNode: public CObjectTreeNode
{
    public:
        CVariableTreeNode(const std::string& name, CVariable& owner): m_name(name), m_owner(owner) {}
        CValue value() const;
        const CVariable& get() const { return m_owner.get_field(m_name); }
        CVariable& get(const bool create) { return m_owner.get_field(m_name, create); }
        void dump(std::ostream& os, int i = 0) const;
        const std::string& name() const { return m_name; }

    private:
        const std::string m_name;
        CVariable& m_owner;
};

class CAccessOperator: public CObjectTreeNode
{
    public:
        CAccessOperator(CObjectTreeNode* object, const std::string& field): m_name(field), m_object(object) {}
        CValue value() const { return m_object->get().get_field(m_name).value(); }
        void dump(std::ostream& os, int i = 0) const;
        const CVariable& get() const { return m_object->get().get_field(m_name); }
        CVariable& get(const bool create) { return m_object->get(create).get_field(m_name, create); }
        ~CAccessOperator() { delete m_object; }

    private:
        const std::string m_name;
        CObjectTreeNode* m_object;
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
            return -v->value();
        }
        void dump(std::ostream& os, int i = 0) const
        {
            indent(os, i) << "CNegativeOperator: " << m_arguments.size() << " operands" << std::endl;
            COperatorTreeNode::dump(os, 1 + i);
        }
};

class CNotOperator: public CUnaryOperator
{
    public:
        CValue value() const
        {
            if (m_arguments.empty())
            {
                throw CRuntimeException("Not enough arguments");
            }
            const CTreeNode* v = m_arguments.front();
            return !v->value();
        }
        void dump(std::ostream& os, int i = 0) const
        {
            indent(os, i) << "CNotOperator: " << m_arguments.size() << " operands" << std::endl;
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

class CAndOperator: public CBinaryOperator
{
    public:
        CValue value() const
        {
            if (2 != m_arguments.size())
            {
                throw CRuntimeException("Not enough arguments");
            }
            return m_arguments.front()->value() && m_arguments.back()->value();
        }
        void dump(std::ostream& os, int i = 0) const
        {
            indent(os, i) << "CAndOperator: " << m_arguments.size() << " operands" << std::endl;
            COperatorTreeNode::dump(os, 1 + i);
        }
};

class COrOperator: public CBinaryOperator
{
    public:
        CValue value() const
        {
            if (2 != m_arguments.size())
            {
                throw CRuntimeException("Not enough arguments");
            }
            return m_arguments.front()->value() || m_arguments.back()->value();
        }
        void dump(std::ostream& os, int i = 0) const
        {
            indent(os, i) << "COrOperator: " << m_arguments.size() << " operands" << std::endl;
            COperatorTreeNode::dump(os, 1 + i);
        }
};

class CLikeOperator: public CBinaryOperator
{
    private:
        static bool like(const std::string& s, const std::string& p);

    public:
        CValue value() const
        {
            if (2 != m_arguments.size())
            {
                throw CRuntimeException("Not enough arguments");
            }
            const CValue& str = m_arguments.front()->value();
            const CValue& pattern = m_arguments.back()->value();

            if (str.is_null() || pattern.is_null())
            {
                return CValue(static_cast<void*>(NULL));
            }

            return CValue(static_cast<bool>(like(str.cast_to(CValue::ETYPE_STRING).get_string(),
                            pattern.cast_to(CValue::ETYPE_STRING).get_string())));
        }
        void dump(std::ostream& os, int i = 0) const
        {
            indent(os, i) << "CLikeOperator: " << m_arguments.size() << " operands" << std::endl;
            COperatorTreeNode::dump(os, 1 + i);
        }
};

class CIsOperator: public CBinaryOperator
{
    public:
        CValue value() const
        {
            if (2 != m_arguments.size())
            {
                throw CRuntimeException("Not enough arguments");
            }
            const CValue& left = m_arguments.front()->value();
            const CValue& right = m_arguments.back()->value();

            if (left.is_null() && right.is_null())
            {
                return CValue(static_cast<bool>(true));
            }
            CValue result = left.equal_to(right, false);
            if (result.is_defined())
            {
                return result;
            }
            return CValue(static_cast<bool>(false));
        }
        void dump(std::ostream& os, int i = 0) const
        {
            indent(os, i) << "CIsOperator: " << m_arguments.size() << " operands" << std::endl;
            COperatorTreeNode::dump(os, 1 + i);
        }
};

class CAssignmentOperator: public CTreeNode
{
    public:
        CAssignmentOperator(CObjectTreeNode* var, const CTreeNode* expr): m_variable(var), m_expression(expr) {}
        CValue value() const
        {
            CValue value = m_expression->value();
            m_variable->get(true).value(value);
            return value;
        }
        void dump(std::ostream& os, int i = 0) const
        {
            indent(os, i) << "CAssignmentOperator: " << std::endl;
            m_variable->dump(os, 1 + i);
            m_variable->dump(os, 1 + i);
        }
    private:
        CObjectTreeNode* m_variable;
        const CTreeNode* m_expression;
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
        CTree& operator=(const CTree&);  // Disable assignment operator.

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
        static CConstantTreeNode* createConstantNode(const void* null) { return new CConstantTreeNode(null); }
        static CVariableTreeNode* createVariableNode(const std::string& name, CVariable& owner) { return new CVariableTreeNode(name, owner); }
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
        static CAndOperator* createAndOperator(const CTreeNode* left, const CTreeNode* right) { return createBinaryOperator<CAndOperator>(left, right); }
        static COrOperator* createOrOperator(const CTreeNode* left, const CTreeNode* right) { return createBinaryOperator<COrOperator>(left, right); }
        static CLikeOperator* createLikeOperator(const CTreeNode* left, const CTreeNode* right) { return createBinaryOperator<CLikeOperator>(left, right); }
        static CIsOperator* createIsOperator(const CTreeNode* left, const CTreeNode* right) { return createBinaryOperator<CIsOperator>(left, right); }
        static CAccessOperator* createAccessOperator(CObjectTreeNode* object, const std::string& field) { return new CAccessOperator(object, field); }
        static CNotOperator* createNotOperator(const CTreeNode* node) { CNotOperator* o = new CNotOperator(); o->add_argument(node); return o; }
        static CAssignmentOperator* createAssignmentOperator(CObjectTreeNode* object, const CTreeNode* expr) { return new CAssignmentOperator(object, expr); }

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

namespace types
{
    class ParserType
    {
        public:
            virtual ~ParserType() {}
            virtual scanner::CToken scan(const std::string& expression, scanner::CState& state) const = 0;
            virtual void *ParseAlloc(void *(*mallocProc)(size_t)) const = 0;
            virtual void Parse( void *yyp, int yymajor, void* yyminor, grammar::SState*) const = 0;
            virtual void ParseFree( void *p, void (*freeProc)(void*)) const = 0;

    };

    class Expressions: public ParserType
    {
        public:
            static const Expressions& instance() { return s_i; }
            virtual scanner::CToken scan(const std::string& expression, scanner::CState& state) const { return scanner::expressions::CScanner()(expression, state); }
            virtual void *ParseAlloc(void *(*mallocProc)(size_t)) const { return grammar::expressions::ParseAlloc(mallocProc); }
            virtual void Parse( void *yyp, int yymajor, void* yyminor, grammar::SState* p) const { grammar::expressions::Parse(yyp, yymajor, yyminor, p); }
            virtual void ParseFree( void *p, void (*freeProc)(void*)) const { grammar::expressions::ParseFree(p, freeProc); }

        private:
            const static Expressions s_i;
    };

    class Where: public ParserType
    {
        public:
            static const Where& instance() { return s_i; }
            virtual scanner::CToken scan(const std::string& expression, scanner::CState& state) const { return scanner::where::CScanner()(expression, state); }
            virtual void *ParseAlloc(void *(*mallocProc)(size_t)) const { return grammar::where::ParseAlloc(mallocProc); }
            virtual void Parse( void *yyp, int yymajor, void* yyminor, grammar::SState* p) const { grammar::where::Parse(yyp, yymajor, yyminor, p); }
            virtual void ParseFree( void *p, void (*freeProc)(void*)) const { grammar::where::ParseFree(p, freeProc); }

        private:
            const static Where s_i;
    };
}

class CParser
{
    public:
        CParser(): m_state(EPS_UNDEFINED) {}
        template <typename ParserType>
        bool parse(const std::string& expression);
        void dump_tree(std::ostream& os) const;
        void dump_variables(std::ostream& os) const
        {
            os << "Root variable: ";
            m_vroot.dump(os);
            os << std::endl;
        };
        CValue evaluate() const { return m_tree.value(); };
        void add_variable(const std::string& name, const CValue& value) { m_vroot.add_field(name, value); };
        void add_variable(const std::string& name, const CVariable& value) { m_vroot.add_field(name, value); };
        CVariable& root() { return m_vroot; }
        void clear_variables() { m_vroot.clear(); }
        void clear_tree() { m_tree.clear(); }
        void reset();
        bool ready() const { return EPS_READY == m_state; }
        ~CParser() { reset(); }

    private:
        EParserState m_state;
        CTree m_tree;
        CVariable m_vroot;

        bool parse(const types::ParserType* ptype, const std::string& expression);
};

inline void CParser::reset()
{
    clear_variables();
    clear_tree();
}

template <typename T>
class implement_me {};

template <typename T>
bool CParser::parse(const std::string& line)
{
    return implement_me<T>::FAIL;
}

template <> inline
bool CParser::parse<types::Expressions>(const std::string& line)
{
    return parse(&types::Expressions::instance(), line);
}

template <> inline
bool CParser::parse<types::Where>(const std::string& line)
{
    return parse(&types::Where::instance(), line);
}

}

#endif
/* vim: set ts=4 sw=4 tw=0 et syntax=cpp :*/

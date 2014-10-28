#include "parser.h"

#include "scanner.h"

#include <list>
#include <map>

#define TYPES   TOKEN(ET_UNDEFINED) \
    TOKEN(ET_INTEGER) \
    TOKEN(ET_FLOAT) \
    TOKEN(ET_BOOLEAN) \
    TOKEN(ET_STRING)

namespace parser
{
    static const char *TYPE_NAMES[] =
    {
#define TOKEN(x) #x,
        TYPES
#undef TOKEN
    };

class CValue
{
    public:
        enum EType
        {
#define TOKEN(x) x,
            TYPES
#undef TOKEN
        };

        CValue(): m_type(ET_UNDEFINED) {}
        CValue(long long int value): m_type(ET_INTEGER) { m_value.i = value; }
        CValue(long double value): m_type(ET_FLOAT) { m_value.f = value; }
        CValue(bool value): m_type(ET_BOOLEAN) { m_value.b = value; }
        CValue(const std::string& value): m_type(ET_STRING), m_string(value) { m_value.s = m_string.c_str(); }
        CValue& operator=(const CValue& right);
        CValue(const CValue& from);

        EType type() const { return m_type; }
        long long int get_integer() const;
        long double get_float() const;
        bool get_boolean() const;
        const std::string& get_string() const;
        CValue cast_to(EType type) const;

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

long long int CValue::get_integer() const
{
    if (ET_INTEGER != m_type)
    {
        throw CRuntimeException("Type of value is not integer");
    }

    return m_value.i;
}

long double CValue::get_float() const
{
    if (ET_FLOAT != m_type)
    {
        throw CRuntimeException("Type of value is not float");
    }

    return m_value.f;
}

bool CValue::get_boolean() const
{
    if (ET_BOOLEAN != m_type)
    {
        throw CRuntimeException("Type of value is not boolean");
    }

    return m_value.b;
}

const std::string& CValue::get_string() const
{
    if (ET_STRING != m_type)
    {
        throw CRuntimeException("Type of value is not string");
    }

    return m_string;
}

CValue CValue::cast_to(EType type) const
{
    const char* from = TYPE_NAMES[m_type];
    const char* to = TYPE_NAMES[type];

    switch (m_type)
    {
        case ET_INTEGER:
            switch (type)
            {
                case ET_INTEGER:
                    return CValue(m_value.i);

                case ET_FLOAT:
                    return CValue(static_cast<long double>(m_value.i));

                case ET_BOOLEAN:
                    return CValue(0 != m_value.i);

                default:
                    throw CRuntimeException((std::string("Could not cast ") + from + " to type " + to).c_str());
            }

        case ET_FLOAT:
            switch (type)
            {
                case ET_FLOAT:
                    return CValue(m_value.f);

                default:
                    throw CRuntimeException((std::string("Could not cast ") + from + " to type " + to).c_str());
            }

        case ET_BOOLEAN:
            switch (type)
            {
                case ET_BOOLEAN:
                    return CValue(m_value.b);

                default:
                    throw CRuntimeException((std::string("Could not cast ") + from + " to type " + to).c_str());
            }

        case ET_STRING:
            switch (type)
            {
                case ET_STRING:
                    return CValue(m_value.s);

                default:
                    throw CRuntimeException((std::string("Could not cast ") + from + " to type " + to).c_str());
            }

        default:
            throw CRuntimeException((std::string("Could not cast ") + from + " to type " + to).c_str());
    }
}

CValue::EType CValue::top_type(CValue::EType a, CValue::EType b)
{
#define TOP(x, y, z) do { if (a == (x) && b == (y)) { return (z); } } while (false)
    TOP(ET_INTEGER, ET_INTEGER, ET_INTEGER);
    TOP(ET_FLOAT, ET_INTEGER, ET_FLOAT);
    TOP(ET_INTEGER, ET_FLOAT, ET_FLOAT);
    TOP(ET_FLOAT, ET_FLOAT, ET_FLOAT);
    TOP(ET_BOOLEAN, ET_BOOLEAN, ET_BOOLEAN);
    TOP(ET_STRING, ET_STRING, ET_STRING);
#undef TOP

    const char* from = TYPE_NAMES[a];
    const char* to = TYPE_NAMES[b];

    throw CRuntimeException((std::string("Could not choose common type between ") + from + " and " + to).c_str());
}

CValue operator<(const CValue& a, const CValue& b)
{
    const CValue::EType type = CValue::top_type(a.type(), b.type());
    const CValue& l = a.type() == type ? a : a.cast_to(type);
    const CValue& r = b.type() == type ? b : b.cast_to(type);

    switch (type)
    {
        case CValue::ET_INTEGER:
            return l.get_integer() < r.get_integer();

        case CValue::ET_FLOAT:
            return l.get_float() < r.get_float();

        case CValue::ET_BOOLEAN:
            return l.get_boolean() < r.get_boolean();

        case CValue::ET_STRING:
            return l.get_string() < r.get_string();

        default:
            throw CRuntimeException((std::string("Unexpected type") + TYPE_NAMES[type] + " for operation <").c_str());
    }
}

CValue operator<=(const CValue& a, const CValue& b)
{
    const CValue::EType type = CValue::top_type(a.type(), b.type());
    const CValue& l = a.type() == type ? a : a.cast_to(type);
    const CValue& r = b.type() == type ? b : b.cast_to(type);

    switch (type)
    {
        case CValue::ET_INTEGER:
            return l.get_integer() <= r.get_integer();

        case CValue::ET_FLOAT:
            return l.get_float() <= r.get_float();

        case CValue::ET_BOOLEAN:
            return l.get_boolean() <= r.get_boolean();

        case CValue::ET_STRING:
            return l.get_string() <= r.get_string();

        default:
            throw CRuntimeException((std::string("Unexpected type") + TYPE_NAMES[type] + " for operation <=").c_str());
    }
}

CValue operator==(const CValue& a, const CValue& b)
{
    const CValue::EType type = CValue::top_type(a.type(), b.type());
    const CValue& l = a.type() == type ? a : a.cast_to(type);
    const CValue& r = b.type() == type ? b : b.cast_to(type);

    switch (type)
    {
        case CValue::ET_INTEGER:
            return l.get_integer() == r.get_integer();

        case CValue::ET_FLOAT:
            return l.get_float() == r.get_float();

        case CValue::ET_BOOLEAN:
            return l.get_boolean() == r.get_boolean();

        case CValue::ET_STRING:
            return l.get_string() == r.get_string();

        default:
            throw CRuntimeException((std::string("Unexpected type") + TYPE_NAMES[type] + " for operation ==").c_str());
    }
}

CValue operator!=(const CValue& a, const CValue& b)
{
    const CValue::EType type = CValue::top_type(a.type(), b.type());
    const CValue& l = a.type() == type ? a : a.cast_to(type);
    const CValue& r = b.type() == type ? b : b.cast_to(type);

    switch (type)
    {
        case CValue::ET_INTEGER:
            return l.get_integer() != r.get_integer();

        case CValue::ET_FLOAT:
            return l.get_float() != r.get_float();

        case CValue::ET_BOOLEAN:
            return l.get_boolean() != r.get_boolean();

        case CValue::ET_STRING:
            return l.get_string() != r.get_string();

        default:
            throw CRuntimeException((std::string("Unexpected type") + TYPE_NAMES[type] + " for operation !=").c_str());
    }
}

CValue operator>(const CValue& a, const CValue& b)
{
    const CValue::EType type = CValue::top_type(a.type(), b.type());
    const CValue& l = a.type() == type ? a : a.cast_to(type);
    const CValue& r = b.type() == type ? b : b.cast_to(type);

    switch (type)
    {
        case CValue::ET_INTEGER:
            return l.get_integer() > r.get_integer();

        case CValue::ET_FLOAT:
            return l.get_float() > r.get_float();

        case CValue::ET_BOOLEAN:
            return l.get_boolean() > r.get_boolean();

        case CValue::ET_STRING:
            return l.get_string() > r.get_string();

        default:
            throw CRuntimeException((std::string("Unexpected type") + TYPE_NAMES[type] + " for operation >").c_str());
    }
}

CValue operator>=(const CValue& a, const CValue& b)
{
    const CValue::EType type = CValue::top_type(a.type(), b.type());
    const CValue& l = a.type() == type ? a : a.cast_to(type);
    const CValue& r = b.type() == type ? b : b.cast_to(type);

    switch (type)
    {
        case CValue::ET_INTEGER:
            return l.get_integer() >= r.get_integer();

        case CValue::ET_FLOAT:
            return l.get_float() >= r.get_float();

        case CValue::ET_BOOLEAN:
            return l.get_boolean() >= r.get_boolean();

        case CValue::ET_STRING:
            return l.get_string() >= r.get_string();

        default:
            throw CRuntimeException((std::string("Unexpected type") + TYPE_NAMES[type] + " for operation >=").c_str());
    }
}

CValue operator+(const CValue& a, const CValue& b)
{
    const CValue::EType type = CValue::top_type(a.type(), b.type());
    const CValue& l = a.type() == type ? a : a.cast_to(type);
    const CValue& r = b.type() == type ? b : b.cast_to(type);

    switch (type)
    {
        case CValue::ET_INTEGER:
            return l.get_integer() + r.get_integer();

        case CValue::ET_FLOAT:
            return l.get_float() + r.get_float();

        default:
            throw CRuntimeException((std::string("Unexpected type") + TYPE_NAMES[type] + " for operation +").c_str());
    }
}

CValue operator-(const CValue& a, const CValue& b)
{
    const CValue::EType type = CValue::top_type(a.type(), b.type());
    const CValue& l = a.type() == type ? a : a.cast_to(type);
    const CValue& r = b.type() == type ? b : b.cast_to(type);

    switch (type)
    {
        case CValue::ET_INTEGER:
            return l.get_integer() - r.get_integer();

        case CValue::ET_FLOAT:
            return l.get_float() - r.get_float();

        default:
            throw CRuntimeException((std::string("Unexpected type") + TYPE_NAMES[type] + " for operation -").c_str());
    }
}

CValue operator/(const CValue& a, const CValue& b)
{
    const CValue::EType type = CValue::top_type(a.type(), b.type());
    const CValue& l = a.type() == type ? a : a.cast_to(type);
    const CValue& r = b.type() == type ? b : b.cast_to(type);

    switch (type)
    {
        case CValue::ET_INTEGER:
            return l.get_integer() - r.get_integer();

        case CValue::ET_FLOAT:
            return l.get_float() - r.get_float();

        default:
            throw CRuntimeException((std::string("Unexpected type") + TYPE_NAMES[type] + " for operation /").c_str());
    }
}

CValue operator*(const CValue& a, const CValue& b)
{
    const CValue::EType type = CValue::top_type(a.type(), b.type());
    const CValue& l = a.type() == type ? a : a.cast_to(type);
    const CValue& r = b.type() == type ? b : b.cast_to(type);

    switch (type)
    {
        case CValue::ET_INTEGER:
            return l.get_integer() - r.get_integer();

        case CValue::ET_FLOAT:
            return l.get_float() - r.get_float();

        default:
            throw CRuntimeException((std::string("Unexpected type") + TYPE_NAMES[type] + " for operation *").c_str());
    }
}


CValue& CValue::operator=(const CValue& right)
{
    if (this != &right)
    {
        m_type = right.m_type;
        switch (m_type)
        {
            case ET_INTEGER:
                m_value.i = right.m_value.i;
                break;

            case ET_FLOAT:
                m_value.f = right.m_value.f;
                break;

            case ET_BOOLEAN:
                m_value.b = right.m_value.b;
                break;

            case ET_STRING:
                m_string = right.m_string;
                m_value.s = m_string.c_str();
                break;

            case ET_UNDEFINED:
                break;

            default:
                throw CParserException("Unexpected type of value");
        }
    }

    return *this;
}

CValue::CValue(const CValue& from)
{
    *this = from;
}

class CVariable
{
    public:
        CVariable(): m_name(""), m_value() {}
        CVariable(const std::string& name): m_name(name) {}
        CVariable(const std::string& name, const CValue& value): m_name(name), m_value(value) {}
        const CValue& value() const { return m_value; }

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

    private:
        typedef std::map<std::string, CVariable> variables_map_t;

        variables_map_t m_variables;
};

void CVariables::add_variable(const std::string& name, const CValue& value)
{
    m_variables[name] = CVariable(name, value);
}

const CVariable& CVariables::get_variable(const std::string& name) const
{
    variables_map_t::const_iterator v = m_variables.find(name);
    if (m_variables.end() == v)
    {
        throw CParserException("Variable is not defined");
    }
    return v->second;
}

class CTreeNode
{
    public:
        virtual CValue value() const = 0;
};

class CLeafTreeNode: public CTreeNode
{
};

class CConstantTreeNode: public CLeafTreeNode
{
    public:
        CConstantTreeNode(long long int value): m_value(value) {}
        CConstantTreeNode(long double value): m_value(value) {}
        CConstantTreeNode(bool value): m_value(value) {}
        CConstantTreeNode(const std::string& value): m_value(value) {}
        CValue value() const { return m_value; }

    private:
        CValue m_value;
};

class CVariableTreeNode: public CLeafTreeNode
{
    public:
        CVariableTreeNode(const std::string& name): m_variable(name) {}
        CValue value() const { return m_variable.value(); }

    private:
        CVariable m_variable;
};

class COperatorTreeNode: public CTreeNode
{
    public:
        virtual void add_argument(const CTreeNode* node) = 0;

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
            return m_arguments.front()->value() + m_arguments.back()->value();
        }
};

class CSubOperator: public CBinaryOperator
{
    public:
        CValue value() const
        {
            return m_arguments.front()->value() - m_arguments.back()->value();
        }
};

class CDivOperator: public CBinaryOperator
{
    public:
        CValue value() const
        {
            return m_arguments.front()->value() / m_arguments.back()->value();
        }
};

class CMulOperator: public CBinaryOperator
{
    public:
        CValue value() const
        {
            return m_arguments.front()->value() * m_arguments.back()->value();
        }
};

class CLessOperator: public CBinaryOperator
{
    public:
        CValue value() const
        {
            return m_arguments.front()->value() < m_arguments.back()->value();
        }
};

class CLessOrEqualOperator: public CBinaryOperator
{
    public:
        CValue value() const
        {
            return m_arguments.front()->value() <= m_arguments.back()->value();
        }
};

class CEqualOperator: public CBinaryOperator
{
    public:
        CValue value() const
        {
            return m_arguments.front()->value() == m_arguments.back()->value();
        }
};

class CNotEqualOperator: public CBinaryOperator
{
    public:
        CValue value() const
        {
            return m_arguments.front()->value() != m_arguments.back()->value();
        }
};

class CGreaterOperator: public CBinaryOperator
{
    public:
        CValue value() const
        {
            return m_arguments.front()->value() > m_arguments.back()->value();
        }
};

class CGreaterOrEqualOperator: public CBinaryOperator
{
    public:
        CValue value() const
        {
            return m_arguments.front()->value() >= m_arguments.back()->value();
        }
};

class CTreeNodeFactory
{
    public:
        static CConstantTreeNode* createConstantNode(long long int value) { return new CConstantTreeNode(value); }
        static CConstantTreeNode* createConstantNode(long double value) { return new CConstantTreeNode(value); }
        static CConstantTreeNode* createConstantNode(bool value) { return new CConstantTreeNode(value); }
        static CConstantTreeNode* createConstantNode(const std::string& value) { return new CConstantTreeNode(value); }
        static CVariableTreeNode* createVariableNode(const std::string& name) { return new CVariableTreeNode(name); }
        static CLessOperator* createLessOperator(const CTreeNode* left, const CTreeNode* right) { return createBinaryOperator<CLessOperator>(left, right); }
        static CLessOrEqualOperator* createLessOrEqualOperator(const CTreeNode* left, const CTreeNode* right) { return createBinaryOperator<CLessOrEqualOperator>(left, right); }
        static CEqualOperator* createEqualOperator(const CTreeNode* left, const CTreeNode* right) { return createBinaryOperator<CEqualOperator>(left, right); }
        static CNotEqualOperator* createNotEqualOperator(const CTreeNode* left, const CTreeNode* right) { return createBinaryOperator<CNotEqualOperator>(left, right); }
        static CGreaterOperator* createGreaterOperator(const CTreeNode* left, const CTreeNode* right) { return createBinaryOperator<CGreaterOperator>(left, right); }
        static CGreaterOrEqualOperator* createGreaterOrEqualOperator(const CTreeNode* left, const CTreeNode* right) { return createBinaryOperator<CGreaterOrEqualOperator>(left, right); }
        static CAddOperator* createAddOperator(const CTreeNode* left, const CTreeNode* right) { return createBinaryOperator<CAddOperator>(left, right); }
        static CSubOperator* createSubOperator(const CTreeNode* left, const CTreeNode* right) { return createBinaryOperator<CSubOperator>(left, right); }
        static CMulOperator* createMulOperator(const CTreeNode* left, const CTreeNode* right) { return createBinaryOperator<CMulOperator>(left, right); }
        static CDivOperator* createDivOperator(const CTreeNode* left, const CTreeNode* right) { return createBinaryOperator<CDivOperator>(left, right); }

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

class CTree
{
    public:
        CTree(): m_root(NULL) {}

    private:
        CTreeNode* m_root;
};

class CParserImpl
{
    public:
        typedef std::list<scanner::CToken> tokens_list_t;

        CParserImpl() {}
        bool parse(const std::string& expression);
        bool build_tree(const tokens_list_t& tokens);
};

bool CParserImpl::build_tree(const tokens_list_t& tokens)
{
    for (tokens_list_t::const_iterator i = tokens.begin(); i != tokens.end(); i++)
    {
        std::cout << scanner::TOKEN_NAMES[i->token()] << " ";
    }
    std::cout << std::endl;
    return false;
}

CParserImpl* CParser::impl()
{
    if (!m_impl)
    {
        m_impl = new CParserImpl();
        if (NULL == m_impl)
        {
            throw CParserException("Cannot create instance of CParserImpl()");
        }
    }
    return m_impl;
}

bool CParserImpl::parse(const std::string& line)
{
    tokens_list_t tokens;
    size_t offset = 0;
    int balance = 0;
    scanner::EToken prev = scanner::ET_ERROR;

    scanner::CToken t;
    do
    {
        t = scanner::scan(line, offset);

        if (scanner::ET_LEFT_PARENTHESIS == t.token())
        {
            balance++;
        }
        if (scanner::ET_RIGHT_PARENTHESIS == t.token() && 0 < balance)
        {
            balance--;
        }
        else
        {
            throw CParserException("Parenthesis is not balanced");
        }

        if (scanner::ET_OPERATION == t.token())
        {
            switch (prev)
            {
                case scanner::ET_RIGHT_PARENTHESIS:
                case scanner::ET_INTEGER:
                case scanner::ET_FLOAT:
                case scanner::ET_BOOLEAN:
                case scanner::ET_STRING:
                case scanner::ET_IDENTIFIER:
                    break;

                default:
                    throw CParserException("Syntax error");
            }
        }

        if (scanner::ET_SPACE != t.token())
        {
            tokens.push_back(t);
        }
        prev = t.token();
    } while (scanner::ET_ERROR != t.token() && scanner::ET_END != t.token());

    build_tree(tokens);

    return scanner::ET_END == t.token();
}

bool CParser::parse(const std::string& expression)
{
    try
    {
        bool result = impl()->parse(expression);
        m_state = result ? EPS_READY : EPS_ERROR;
    } catch (const CParserException& e)
    {
        m_state = EPS_UNDEFINED;
    }

    return EPS_READY == m_state;
}
}

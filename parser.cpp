#include "parser.h"

#include "scanner.h"
#include "grammar.h"
#include "grammar.c"

#include <cstdlib>

namespace parser
{
    static const char *TYPE_NAMES[] =
    {
#define TOKEN(x) #x,
        TYPES
#undef TOKEN
    };

long long int CValue::get_integer() const
{
    if (ETYPE_INTEGER != m_type)
    {
        throw CRuntimeException("Type of value is not integer");
    }

    return m_value.i;
}

long double CValue::get_float() const
{
    if (ETYPE_FLOAT != m_type)
    {
        throw CRuntimeException("Type of value is not float");
    }

    return m_value.f;
}

bool CValue::get_boolean() const
{
    if (ETYPE_BOOLEAN != m_type)
    {
        throw CRuntimeException("Type of value is not boolean");
    }

    return m_value.b;
}

const std::string& CValue::get_string() const
{
    if (ETYPE_STRING != m_type)
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
        case ETYPE_INTEGER:
            switch (type)
            {
                case ETYPE_INTEGER:
                    return CValue(m_value.i);

                case ETYPE_FLOAT:
                    return CValue(static_cast<long double>(m_value.i));

                case ETYPE_BOOLEAN:
                    return CValue(0 != m_value.i);

                default:
                    throw CRuntimeException((std::string("Could not cast ") + from + " to type " + to).c_str());
            }

        case ETYPE_FLOAT:
            switch (type)
            {
                case ETYPE_FLOAT:
                    return CValue(m_value.f);

                default:
                    throw CRuntimeException((std::string("Could not cast ") + from + " to type " + to).c_str());
            }

        case ETYPE_BOOLEAN:
            switch (type)
            {
                case ETYPE_BOOLEAN:
                    return CValue(m_value.b);

                default:
                    throw CRuntimeException((std::string("Could not cast ") + from + " to type " + to).c_str());
            }

        case ETYPE_STRING:
            switch (type)
            {
                case ETYPE_STRING:
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
    TOP(ETYPE_INTEGER, ETYPE_INTEGER, ETYPE_INTEGER);
    TOP(ETYPE_FLOAT, ETYPE_INTEGER, ETYPE_FLOAT);
    TOP(ETYPE_INTEGER, ETYPE_FLOAT, ETYPE_FLOAT);
    TOP(ETYPE_FLOAT, ETYPE_FLOAT, ETYPE_FLOAT);
    TOP(ETYPE_BOOLEAN, ETYPE_BOOLEAN, ETYPE_BOOLEAN);
    TOP(ETYPE_STRING, ETYPE_STRING, ETYPE_STRING);
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
        case CValue::ETYPE_INTEGER:
            return l.get_integer() < r.get_integer();

        case CValue::ETYPE_FLOAT:
            return l.get_float() < r.get_float();

        case CValue::ETYPE_BOOLEAN:
            return l.get_boolean() < r.get_boolean();

        case CValue::ETYPE_STRING:
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
        case CValue::ETYPE_INTEGER:
            return l.get_integer() <= r.get_integer();

        case CValue::ETYPE_FLOAT:
            return l.get_float() <= r.get_float();

        case CValue::ETYPE_BOOLEAN:
            return l.get_boolean() <= r.get_boolean();

        case CValue::ETYPE_STRING:
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
        case CValue::ETYPE_INTEGER:
            return l.get_integer() == r.get_integer();

        case CValue::ETYPE_FLOAT:
            return l.get_float() == r.get_float();

        case CValue::ETYPE_BOOLEAN:
            return l.get_boolean() == r.get_boolean();

        case CValue::ETYPE_STRING:
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
        case CValue::ETYPE_INTEGER:
            return l.get_integer() != r.get_integer();

        case CValue::ETYPE_FLOAT:
            return l.get_float() != r.get_float();

        case CValue::ETYPE_BOOLEAN:
            return l.get_boolean() != r.get_boolean();

        case CValue::ETYPE_STRING:
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
        case CValue::ETYPE_INTEGER:
            return l.get_integer() > r.get_integer();

        case CValue::ETYPE_FLOAT:
            return l.get_float() > r.get_float();

        case CValue::ETYPE_BOOLEAN:
            return l.get_boolean() > r.get_boolean();

        case CValue::ETYPE_STRING:
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
        case CValue::ETYPE_INTEGER:
            return l.get_integer() >= r.get_integer();

        case CValue::ETYPE_FLOAT:
            return l.get_float() >= r.get_float();

        case CValue::ETYPE_BOOLEAN:
            return l.get_boolean() >= r.get_boolean();

        case CValue::ETYPE_STRING:
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
        case CValue::ETYPE_INTEGER:
            return l.get_integer() + r.get_integer();

        case CValue::ETYPE_FLOAT:
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
        case CValue::ETYPE_INTEGER:
            return l.get_integer() - r.get_integer();

        case CValue::ETYPE_FLOAT:
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
        case CValue::ETYPE_INTEGER:
            return l.get_integer() - r.get_integer();

        case CValue::ETYPE_FLOAT:
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
        case CValue::ETYPE_INTEGER:
            return l.get_integer() - r.get_integer();

        case CValue::ETYPE_FLOAT:
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
            case ETYPE_INTEGER:
                m_value.i = right.m_value.i;
                break;

            case ETYPE_FLOAT:
                m_value.f = right.m_value.f;
                break;

            case ETYPE_BOOLEAN:
                m_value.b = right.m_value.b;
                break;

            case ETYPE_STRING:
                m_string = right.m_string;
                m_value.s = m_string.c_str();
                break;

            case ETYPE_UNDEFINED:
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
    void* parser;
    void* state = NULL;
    size_t offset = 0;

    parser = ParseAlloc(malloc);
    std::cout << "start parsing '" << line << "'" << std::endl;
    for (scanner::CToken t = scanner::scan(line, offset);
            ET_ERROR != t.token() && ET_EOF != t.token();
            t = scanner::scan(line, offset))
    {
        Parse(parser, t.token(), NULL, state);
    }
    std::cout << "last touch" << std::endl;
    Parse(parser, ET_END, ET_END, &state);
    std::cout << "end parsing" << std::endl;
    ParseFree(parser, free);

    return false;
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

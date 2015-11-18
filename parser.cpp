#include "parser.h"

#include "colors.h"
#include "expressions.scanner.h"
#include "expressions.grammar.h"
#include "where.scanner.h"
#include "where.grammar.h"
#include "state.h"

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

                case ETYPE_STRING:
                    {
                        std::stringstream ss;
                        ss << m_value.i;
                        return CValue(ss.str());
                    }

                default:
                    throw CRuntimeException((std::string("Could not cast ") + from + " to type " + to).c_str());
            }

        case ETYPE_FLOAT:
            switch (type)
            {
                case ETYPE_FLOAT:
                    return CValue(m_value.f);

                case ETYPE_STRING:
                    {
                        std::stringstream ss;
                        ss << m_value.f;
                        return CValue(ss.str());
                    }

                default:
                    throw CRuntimeException((std::string("Could not cast ") + from + " to type " + to).c_str());
            }

        case ETYPE_BOOLEAN:
            switch (type)
            {
                case ETYPE_BOOLEAN:
                    return CValue(m_value.b);

                case ETYPE_STRING:
                    return CValue(m_value.b ? "1" : "0");

                default:
                    throw CRuntimeException((std::string("Could not cast ") + from + " to type " + to).c_str());
            }

        case ETYPE_STRING:
            switch (type)
            {
                case ETYPE_STRING:
                    return CValue(std::string(m_value.s));

                default:
                    throw CRuntimeException((std::string("Could not cast ") + from + " to type " + to).c_str());
            }

        default:
            throw CRuntimeException((std::string("Could not cast ") + from + " to type " + to).c_str());
    }
}

void CValue::dump(std::ostream& os) const
{
    switch (m_type)
    {
        case ETYPE_INTEGER:
            os << "INTEGER(" << get_integer() << ")";
            break;

        case ETYPE_FLOAT:
            os << "FLOAT(" << get_float() << ")";
            break;

        case ETYPE_BOOLEAN:
            os << "BOOLEAN(" << (get_boolean() ? "true" : "false") << ")";
            break;

        case ETYPE_STRING:
            os << "STRING(\"" << get_string() << "\")";
            break;

        case ETYPE_NULL:
            os << "NULL";
            break;

        case ETYPE_UNDEFINED:
            os << "<Undefined value>";
            break;
    }
}

CValue::EType CValue::top_type(CValue::EType a, CValue::EType b, const bool throw_exception)
{
#ifdef TOP
#undef TOP // avoid a conflict with AIX 6.1 system include files
#endif
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

    if (throw_exception)
    {
        throw CRuntimeException((std::string("Could not choose common type between ") + from + " and " + to).c_str());
    }
    return ETYPE_UNDEFINED;
}

CValue operator<(const CValue& a, const CValue& b)
{
    if (a.is_null() || b.is_null())
    {
        return CValue(static_cast<bool>(false));
    }

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
            throw CRuntimeException((std::string("Unexpected type ") + TYPE_NAMES[type] + " for operation <").c_str());
    }
}

CValue operator<=(const CValue& a, const CValue& b)
{
    if (a.is_null() || b.is_null())
    {
        return CValue(static_cast<bool>(false));
    }

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
            throw CRuntimeException((std::string("Unexpected type ") + TYPE_NAMES[type] + " for operation <=").c_str());
    }
}

CValue CValue::equal_to(const CValue& right, const bool throw_exception) const
{
    if (this->is_null() || right.is_null())
    {
        return CValue(static_cast<bool>(false));
    }

    const CValue::EType type = CValue::top_type(this->type(), right.type(), throw_exception);
    if (ETYPE_UNDEFINED == type)
    {
        return CValue();
    }

    const CValue& l = this->type() == type ? *this : this->cast_to(type);
    const CValue& r = right.type() == type ? right : right.cast_to(type);

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
            throw CRuntimeException((std::string("Unexpected type ") + TYPE_NAMES[type] + " for operation ==").c_str());
    }
}

CValue operator==(const CValue& a, const CValue& b)
{
    return a.equal_to(b, true);
}

CValue operator!=(const CValue& a, const CValue& b)
{
    if (a.is_null() || b.is_null())
    {
        return CValue(static_cast<bool>(false));
    }

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
            throw CRuntimeException((std::string("Unexpected type ") + TYPE_NAMES[type] + " for operation !=").c_str());
    }
}

CValue operator>(const CValue& a, const CValue& b)
{
    if (a.is_null() || b.is_null())
    {
        return CValue(static_cast<bool>(false));
    }

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
            throw CRuntimeException((std::string("Unexpected type ") + TYPE_NAMES[type] + " for operation >").c_str());
    }
}

CValue operator>=(const CValue& a, const CValue& b)
{
    if (a.is_null() || b.is_null())
    {
        return CValue(static_cast<bool>(false));
    }

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
            throw CRuntimeException((std::string("Unexpected type ") + TYPE_NAMES[type] + " for operation >=").c_str());
    }
}

CValue operator+(const CValue& a, const CValue& b)
{
    if (a.is_null() || b.is_null())
    {
        return CValue(static_cast<void*>(NULL));
    }

    const CValue::EType type = CValue::top_type(a.type(), b.type());
    const CValue& l = a.type() == type ? a : a.cast_to(type);
    const CValue& r = b.type() == type ? b : b.cast_to(type);

    switch (type)
    {
        case CValue::ETYPE_INTEGER:
            return l.get_integer() + r.get_integer();

        case CValue::ETYPE_FLOAT:
            return l.get_float() + r.get_float();

        case CValue::ETYPE_STRING:
            return l.get_string() + r.get_string();

        default:
            throw CRuntimeException((std::string("Unexpected type ") + TYPE_NAMES[type] + " for operation +").c_str());
    }
}

CValue operator-(const CValue& a, const CValue& b)
{
    if (a.is_null() || b.is_null())
    {
        return CValue(static_cast<void*>(NULL));
    }

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
            throw CRuntimeException((std::string("Unexpected type ") + TYPE_NAMES[type] + " for operation -").c_str());
    }
}

CValue operator-(const CValue& a)
{
    if (a.is_null())
    {
        return CValue(static_cast<void*>(NULL));
    }

    switch (a.type())
    {
        case CValue::ETYPE_INTEGER:
            return -a.get_integer();

        case CValue::ETYPE_FLOAT:
            return -a.get_float();

        default:
            throw CRuntimeException((std::string("Unexpected type ") + TYPE_NAMES[a.type()] + " for operation -").c_str());
    }
}

CValue operator/(const CValue& a, const CValue& b)
{
    if (a.is_null() || b.is_null())
    {
        return CValue(static_cast<void*>(NULL));
    }

    const CValue::EType type = CValue::top_type(a.type(), b.type());
    const CValue& l = a.type() == type ? a : a.cast_to(type);
    const CValue& r = b.type() == type ? b : b.cast_to(type);

    switch (type)
    {
        case CValue::ETYPE_INTEGER:
            return l.get_integer() / r.get_integer();

        case CValue::ETYPE_FLOAT:
            return l.get_float() / r.get_float();

        default:
            throw CRuntimeException((std::string("Unexpected type ") + TYPE_NAMES[type] + " for operation /").c_str());
    }
}

CValue operator%(const CValue& a, const CValue& b)
{
    if (a.is_null() || b.is_null())
    {
        return CValue(static_cast<void*>(NULL));
    }

    const CValue::EType type = CValue::top_type(a.type(), b.type());
    const CValue& l = a.type() == type ? a : a.cast_to(type);
    const CValue& r = b.type() == type ? b : b.cast_to(type);

    switch (type)
    {
        case CValue::ETYPE_INTEGER:
            return l.get_integer() % r.get_integer();

        default:
            throw CRuntimeException((std::string("Unexpected type ") + TYPE_NAMES[type] + " for operation /").c_str());
    }
}

CValue operator*(const CValue& a, const CValue& b)
{
    if (a.is_null() || b.is_null())
    {
        return CValue(static_cast<void*>(NULL));
    }

    const CValue::EType type = CValue::top_type(a.type(), b.type());
    const CValue& l = a.type() == type ? a : a.cast_to(type);
    const CValue& r = b.type() == type ? b : b.cast_to(type);

    switch (type)
    {
        case CValue::ETYPE_INTEGER:
            return l.get_integer() * r.get_integer();

        case CValue::ETYPE_FLOAT:
            return l.get_float() * r.get_float();

        default:
            throw CRuntimeException((std::string("Unexpected type ") + TYPE_NAMES[type] + " for operation *").c_str());
    }
}

CValue operator&&(const CValue& a, const CValue& b)
{
    if (a.is_null() || b.is_null())
    {
        return CValue(static_cast<void*>(NULL));
    }

    const CValue::EType type = CValue::ETYPE_BOOLEAN;
    const CValue& l = a.type() == type ? a : a.cast_to(type);
    const CValue& r = b.type() == type ? b : b.cast_to(type);

    switch (type)
    {
        case CValue::ETYPE_BOOLEAN:
            return l.get_boolean() && r.get_boolean();

        default:
            throw CRuntimeException((std::string("Unexpected type ") + TYPE_NAMES[type] + " for operation *").c_str());
    }
}

CValue operator||(const CValue& a, const CValue& b)
{
    if (a.is_null() || b.is_null())
    {
        return CValue(static_cast<void*>(NULL));
    }

    const CValue::EType type = CValue::ETYPE_BOOLEAN;
    const CValue& l = a.type() == type ? a : a.cast_to(type);
    const CValue& r = b.type() == type ? b : b.cast_to(type);

    switch (type)
    {
        case CValue::ETYPE_BOOLEAN:
            return l.get_boolean() || r.get_boolean();

        default:
            throw CRuntimeException((std::string("Unexpected type ") + TYPE_NAMES[type] + " for operation *").c_str());
    }
}

CValue operator!(const CValue& a)
{
    if (a.is_null())
    {
        return CValue(static_cast<void*>(NULL));
    }

    switch (a.type())
    {
        case CValue::ETYPE_BOOLEAN:
            return !a.get_boolean();

        default:
            throw CRuntimeException((std::string("Unexpected type ") + TYPE_NAMES[a.type()] + " for operation !").c_str());
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

            case ETYPE_NULL:
            case ETYPE_UNDEFINED:
                break;

            default:
                throw CParserException("Unexpected type of value");
        }
    }

    return *this;
}

std::ostream& operator<<(std::ostream& os, const CValue& v)
{
    switch (v.type())
    {
        case CValue::ETYPE_INTEGER:
            return os << v.get_integer();

        case CValue::ETYPE_FLOAT:
            return os << v.get_float();

        case CValue::ETYPE_BOOLEAN:
            return os << (v.get_boolean() ? "true" : "false");

        case CValue::ETYPE_STRING:
            return os << "\"" << v.get_string() << "\"";

        case CValue::ETYPE_NULL:
            return os << "NULL";

        case CValue::ETYPE_UNDEFINED:
            return os << "<Undefined value>";
    }
    return os;
}

CValue::CValue(const CValue& from)
{
    *this = from;
}

CValue CVariableTreeNode::value() const
{
    return m_owner.get_field(m_name).value();
}

CValue CTree::value() const
{
    if (!m_root)
    {
        throw CRuntimeException("Root has not been set");
    }
    return m_root->value();
}

void CTree::clear()
{
    if (m_root)
    {
        delete m_root;
    }
    m_root = NULL;
}

void CTree::dump(std::ostream& os, int i) const
{
    if (m_root)
    {
        m_root->dump(os, i);
    }
    else
    {
        indent(os, i);
        os << "Tree is empty" << std::endl;
    }
}

void CVariableTreeNode::dump(std::ostream& os, int i) const
{
    indent(os, i) << "CVariableTreeNode: ";
    if (m_owner.has_field(m_name))
    {
        m_owner.get_field(m_name).dump(os);
    }
    else
    {
        os << "<variable is not defined>";
    }
    os << std::endl;
}

void CAccessOperator::dump(std::ostream& os, int i) const
{
    indent(os, i) << "CAccessOperator: get field called '" << m_name << "'" << std::endl;
    m_object->dump(os, 1 + i);
}

void COperatorTreeNode::dump(std::ostream& os, int i) const
{
    for (arguments_list_t::const_iterator j = m_arguments.begin(); j != m_arguments.end(); j++)
    {
        const CTreeNode* n = *j;
        n->dump(os, i);
    }
}

COperatorTreeNode::~COperatorTreeNode()
{
    for (arguments_list_t::const_iterator j = m_arguments.begin(); j != m_arguments.end(); j++)
    {
        delete *j;
    }
}

bool CLikeOperator::like(const std::string& s, const std::string& p)
{
    return parser::utils::WildTextCompare(s.c_str(), p.c_str());
}

bool CParser::parse(const types::ParserType* ptype, const std::string& line)
{
    void* parser;
    CTreeNode::ptr_set_t temp_set;
    std::list<CParserNode*> nodes;
    grammar::SState state(0, 0, NULL, &m_vroot, &temp_set);
    m_state = EPS_UNDEFINED;

    if (line.empty())
    {
        m_state = EPS_ERROR;
        std::cerr << "Line may not be empty" << std::endl;
        return false;
    }

    parser = ptype->ParseAlloc(malloc);
    scanner::CToken t;
    scanner::CState ss;
    for (t = ptype->scan(line, ss);
            scanner::CToken::ET_ERROR != t.token() && scanner::CToken::ET_EOF != t.token();
            t = ptype->scan(line, ss))
    {
        CParserNode* node = CTreeNodeFactory::createParserNode(t.value());
        nodes.push_back(node);
        ptype->Parse(parser, t.token(), node, &state);
        if (state.syntax_error)
        {
            break;
        }
        state.offset = static_cast<int>(ss.offset());
    }
    if (!state.syntax_error && scanner::CToken::ET_ERROR != t.token())
    {
        ptype->Parse(parser, scanner::CToken::ET_END, NULL, &state);
    }
    if (state.syntax_error || scanner::CToken::ET_ERROR == t.token())
    {
        m_state = EPS_ERROR;
        int offset = 0;
        if (state.syntax_error)
        {
            offset = ss.offset();
            std::cerr << "PARSER: Grammar error at offset " << colors::green() << ss.offset() << colors::reset()
                << "; Unexpected token #" << t.token() << std::endl;
        }
        else
        {
            offset = state.offset;
            std::cerr << "PARSER: Syntax error at offset " << colors::green()<< state.offset << colors::reset() << std::endl;
        }
        std::string msg = line;
        msg.insert(offset, colors::blue());
        msg = "'" + msg;
        msg.append(colors::reset());
        msg.append("'");
        std::cerr << msg << std::endl;
        indent(std::cerr, offset, " ") << " ^ here" << std::endl;

        for (CTreeNode::ptr_set_t::const_iterator i = temp_set.begin(); i != temp_set.end(); i++)
        {
            delete *i;
        }
        temp_set.clear();
    }
    else
    {
        m_state = EPS_READY;
        m_tree.root(static_cast<CTreeNode*>(state.root));
    }
    ptype->ParseFree(parser, free);

    while (!nodes.empty())
    {
        delete nodes.front();
        nodes.pop_front();
    }

    return EPS_READY == m_state;
}

void CParser::dump_tree(std::ostream& os) const
{
    os << "Parser state is " << (EPS_UNDEFINED == m_state ? "UNDEFINED" : (EPS_ERROR == m_state ? "ERROR" : "READY")) << std::endl;
    m_tree.dump(os);
};

namespace types
{
    const Expressions Expressions::s_i;
    const Where Where::s_i;
}
}
/* vim: set ts=4 sw=4 tw=0 et syntax=cpp :*/

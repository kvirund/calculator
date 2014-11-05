#include "parser.h"

#include "scanner.h"
#include "grammar.h"
#include "state.h"
#include "colors.h"

#include <cstdlib>

extern "C"
{
    void *ParseAlloc(void *(*mallocProc)(size_t));
    void Parse(
            void *yyp,                   /* The parser */
            int yymajor,                 /* The major token code number */
            void* yyminor,               /* The value for the token */
            void*                        /* Optional %extra_argument parameter */
            );
    void ParseFree(
            void *p,                    /* The parser to be deleted */
            void (*freeProc)(void*)     /* Function used to reclaim memory */
            );
}

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

        case ETYPE_UNDEFINED:
            os << "<Undefined value>";
            break;
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
            throw CRuntimeException((std::string("Unexpected type ") + TYPE_NAMES[type] + " for operation <").c_str());
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
            throw CRuntimeException((std::string("Unexpected type ") + TYPE_NAMES[type] + " for operation <=").c_str());
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
            throw CRuntimeException((std::string("Unexpected type ") + TYPE_NAMES[type] + " for operation ==").c_str());
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
            throw CRuntimeException((std::string("Unexpected type ") + TYPE_NAMES[type] + " for operation !=").c_str());
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
            throw CRuntimeException((std::string("Unexpected type ") + TYPE_NAMES[type] + " for operation >").c_str());
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
            throw CRuntimeException((std::string("Unexpected type ") + TYPE_NAMES[type] + " for operation >=").c_str());
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
            throw CRuntimeException((std::string("Unexpected type ") + TYPE_NAMES[type] + " for operation +").c_str());
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
            throw CRuntimeException((std::string("Unexpected type ") + TYPE_NAMES[type] + " for operation -").c_str());
    }
}

CValue operator-(const CValue& a)
{
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
    switch (a.type())
    {
        case CValue::ETYPE_BOOLEAN:
            return !a.get_boolean();

        default:
            throw CRuntimeException((std::string("Unexpected type ") + TYPE_NAMES[a.type()] + " for operation -").c_str());
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

        case CValue::ETYPE_UNDEFINED:
            return os << "<Undefined value>";
    }
    return os;
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
        throw CRuntimeException(("Variable '" + name + "' is not defined").c_str());
    }
    return v->second;
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
    if (m_variables_pool->is_variable_set(m_name))
    {
        m_variables_pool->get_variable(m_name).dump(os);
    }
    else
    {
        os << "<variable is not defined>";
    }
    os << std::endl;
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

bool CParser::parse(const std::string& line)
{
    void* parser;
    size_t offset = 0;
    CTreeNode::ptr_set_t temp_set;
    std::list<CParserNode*> nodes;
    SState state = {offset: 0, syntax_error: 0, root: NULL, vpool: &m_variables_pool, temp_set: &temp_set};
    m_state = EPS_UNDEFINED;

    if (line.empty())
    {
        m_state = EPS_ERROR;
        std::cerr << RED "Line may not be empty" RESET << std::endl;
        return false;
    }

    parser = ParseAlloc(malloc);
    std::cout << "start parsing '" << line << "'" << std::endl;
    scanner::CToken t;
    for (t = scanner::scan(line, offset);
            ET_ERROR != t.token() && ET_EOF != t.token();
            t = scanner::scan(line, offset))
    {
        CParserNode* node = CTreeNodeFactory::createParserNode(t.value());
        nodes.push_back(node);
        Parse(parser, t.token(), node, &state);
        if (state.syntax_error)
        {
            break;
        }
        state.offset = offset;
    }
    if (!state.syntax_error)
    {
        Parse(parser, ET_END, ET_END, &state);
    }
    if (state.syntax_error || ET_ERROR == t.token())
    {
        m_state = EPS_ERROR;
        std::cerr << GREEN "Syntax error at offset " << state.offset << RESET << std::endl;
        std::string msg = line;
        msg.insert(state.offset, BLUE);
        msg = "'" + msg;
        msg.append(RESET "'");
        std::cerr << msg << std::endl;

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
    ParseFree(parser, free);

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
}

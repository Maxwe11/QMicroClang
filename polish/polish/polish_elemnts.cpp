#include "polish_elemnts.h"

polish_element::polish_element(unsigned i)
{
    m_type  = polish::T_LABEL;
    m_id    = i;
    m_token = std::string("m");
    m_token += std::to_string(static_cast<unsigned long long>(m_id));
}

polish_element::polish_element(const std::string& s, polish::type t)
    : m_token(s)
    , m_type(t)
    , m_id()
{
}

polish_element::~polish_element()
{
}

std::string polish_element::toString() const
{
    return m_token;
}

polish::type polish_element::type() const
{
    return m_type;
}

unsigned polish_element::id() const
{
    return m_id;
}

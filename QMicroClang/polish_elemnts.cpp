#include "polish_elemnts.h"

polish_element::polish_element(uint i)
{
    m_type  = polish::T_LABEL;
    m_id    = i;
    m_token = QString("_m%1").arg(m_id);
}

polish_element::polish_element(const QString& s, polish::type t)
    : m_token(s)
    , m_type(t)
    , m_id()
{
}

polish_element::~polish_element()
{
}

QString polish_element::toString() const
{
    return m_token;
}

polish::type polish_element::type() const
{
    return m_type;
}

uint polish_element::id() const
{
    return m_id;
}
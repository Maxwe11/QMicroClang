#ifndef POLISH_ELEMNTS_H
#define POLISH_ELEMNTS_H

#include <boost/serialization/string.hpp>
#include <boost/serialization/access.hpp>

namespace polish {
    enum type { T_VARIABLE = 1, T_CONSTANT = 2, T_STRING = 3, T_UNARY_OP = 4, T_BINARY_OP = 5, T_READ = 6, T_WRITE = 7, T_JMP = 8, T_JIF = 9, T_LABEL = 10 };
}

class polish_element {
    friend class boost::serialization::access;

    template <typename Archive> 
    friend void serialize(Archive &ar, polish_element &p, const unsigned int version);
public:
    polish_element() {}
    polish_element(unsigned i);
    polish_element(const std::string& s, polish::type t);
    ~polish_element();
    std::string toString() const;
    polish::type type() const;
    unsigned id() const;
private:
    std::string     m_token;
    polish::type    m_type;
    unsigned        m_id;
};

template <typename Archive>
void serialize(Archive &ar, polish_element &p, const unsigned int version)
{
    ar & p.m_type;
    ar & p.m_id;
    ar & p.m_token.toStdString();
}

#endif // POLISH_ELEMNTS_H

#ifndef POLISH_ELEMNTS_H
#define POLISH_ELEMNTS_H

#include "utility.h"

namespace polish {
    enum type { T_VARIABLE = 1, T_CONSTANT = 2, T_STRING = 3, T_UNARY_OP = 4, T_BINARY_OP = 5, T_READ = 6, T_WRITE = 7, T_JMP = 8, T_JIF = 9, T_LABEL = 10 };
}

class polish_element {
public:
    polish_element(uint i);
    polish_element(const QString& s, polish::type t);
    ~polish_element();
    QString toString() const;
    polish::type type() const;
    uint id() const;
private:
    QString      m_token;
    polish::type m_type;
    uint         m_id;
};

#endif // POLISH_ELEMNTS_H

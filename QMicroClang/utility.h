#ifndef UTILITY_H
#define UTILITY_H

#include <map>
#include <vector>
#include <utility>
#include <QtCore\QString>

namespace lex {
    enum type {
        T_DOUBLE = 1, 
        T_READ = 2, 
        T_WRITE = 3, 
        T_IF = 4, 
        T_THEN = 5,
        T_ELSE = 6, 
        T_WHILE = 7, 
        T_DO = 8, 
        T_OR = 9, 
        T_AND = 10,
        T_NOT = 11, 
        T_BEGIN = 12, 
        T_END = 13, 
        T_END_OP = 14, 
        T_COMMA = 15,
        T_PLUS = 16, 
        T_MINUS = 17, 
        T_MULT = 18,
        T_DIV = 19,
        T_ASSIGM = 20,
        T_GREATER = 21, 
        T_GREATER_EQ = 22,
        T_LESS = 23, 
        T_LESS_EQ = 24, 
        T_EQUAL = 25,
        T_NOT_EQUAL = 26, 
        T_LBRACKET = 27,
        T_RBRACKET = 28,
        T_LOG_LBRACKET = 29, 
        T_LOG_RBRACKET = 30, 
        T_ID = 31, 
        T_NUMERIC = 32, 
        T_STRING = 33,
        T_NIL = 34
    };
}

struct lexeme {

    lexeme(uint line, lex::type type_, uint index = 0)
            : line_num(line)
            , type(type_)
            , table_indx(index)
        { }

        uint        line_num;
        lex::type   type;
        uint        table_indx;
};

typedef std::vector< QString >              value_table;
typedef std::vector< lexeme >               lexeme_list;
typedef std::map< QString, lex::type >    keyword_table;
typedef std::map< lex::type, uint >      priority_table;

#endif //UTILITY_H

#ifndef PARSER_H
#define PARSER_H

#include "compiler.h"

struct state {
        lex::type lexeme;
        uint      next_state;
        uint      stack_eq;

        state(lex::type lexe, uint nextState, uint Stack_eq = 0) 
            : lexeme(lexe)
            , next_state(nextState)
            , stack_eq(Stack_eq)
        {}
};

typedef std::multimap< uint, state> states_table;
typedef std::map<uint, const char*> error_map;

class QMicroClangParser {
public:
    QMicroClangParser(QMicroClangCompiler*);
    ~QMicroClangParser();
    bool parse();
private:
    void pushError(const QString&);
    states_table createStates();
    error_map createErrorMap();
private:
    QMicroClangCompiler* p;
    const states_table states;
    const error_map    error;
};

#endif //PARSER_H

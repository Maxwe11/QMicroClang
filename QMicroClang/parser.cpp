#include "parser.h"

#include <stack>
#include <cassert>
#include <QtCore\QDebug>

QMicroClangParser::QMicroClangParser(QMicroClangCompiler* parent) 
    : p(parent)
    , states(createStates())
    , error(createErrorMap())
{
}

QMicroClangParser::~QMicroClangParser()
{
}

void QMicroClangParser::pushError(const QString& error_msg)
{
    p->err_list << error_msg;
}

states_table QMicroClangParser::createStates()
{
    states_table table;

    table.emplace(std::make_pair(  1, state(lex::T_DOUBLE      ,   2,   0)));
    table.emplace(std::make_pair(  2, state(lex::T_ID          ,   3,   0)));
    table.emplace(std::make_pair(  3, state(lex::T_COMMA       ,   2,   0)));
    table.emplace(std::make_pair(  3, state(lex::T_END_OP      ,   4,   0)));
    table.emplace(std::make_pair(  4, state(lex::T_BEGIN       ,  11,   5)));
    table.emplace(std::make_pair(  4, state(lex::T_NIL         ,   1,   0)));
    table.emplace(std::make_pair(  5, state(lex::T_END         ,   0,   0)));
    table.emplace(std::make_pair(  5, state(lex::T_NIL         ,  11,   5)));

    table.emplace(std::make_pair( 11, state(lex::T_ID          ,  12,   0)));
    table.emplace(std::make_pair( 11, state(lex::T_READ        ,  14,   0)));
    table.emplace(std::make_pair( 11, state(lex::T_WRITE       ,  17,   0)));
    table.emplace(std::make_pair( 11, state(lex::T_WHILE       , 301,  20))); //while
    table.emplace(std::make_pair( 11, state(lex::T_IF          , 301,  23))); //if
    table.emplace(std::make_pair( 12, state(lex::T_ASSIGM      , 201,  13))); //assigment
    table.emplace(std::make_pair( 13, state(lex::T_END_OP      ,   0,   0))); //end_op
    table.emplace(std::make_pair( 14, state(lex::T_LBRACKET    ,  15,   0))); 
    table.emplace(std::make_pair( 15, state(lex::T_ID          ,  16,   0)));
    table.emplace(std::make_pair( 16, state(lex::T_COMMA       ,  15,   0)));
    table.emplace(std::make_pair( 16, state(lex::T_RBRACKET    ,  13,   0)));
    table.emplace(std::make_pair( 17, state(lex::T_LBRACKET    ,  18,   0)));
    table.emplace(std::make_pair( 18, state(lex::T_ID          ,  19,   0)));
    table.emplace(std::make_pair( 18, state(lex::T_NUMERIC     ,  19,   0)));
    table.emplace(std::make_pair( 18, state(lex::T_STRING      ,  19,   0)));
    table.emplace(std::make_pair( 19, state(lex::T_COMMA       ,  18,   0)));
    table.emplace(std::make_pair( 19, state(lex::T_RBRACKET    ,  13,   0)));
    table.emplace(std::make_pair( 20, state(lex::T_DO          ,  21,   0)));
    table.emplace(std::make_pair( 21, state(lex::T_BEGIN       ,  11,  22)));
    table.emplace(std::make_pair( 22, state(lex::T_END         ,  13,   0)));
    table.emplace(std::make_pair( 22, state(lex::T_NIL         ,  11,  22)));
    table.emplace(std::make_pair( 23, state(lex::T_THEN        ,  24,   0)));
    table.emplace(std::make_pair( 24, state(lex::T_BEGIN       ,  11,  25)));
    table.emplace(std::make_pair( 25, state(lex::T_END         ,  26,   0)));
    table.emplace(std::make_pair( 25, state(lex::T_NIL         ,  11,  25)));
    table.emplace(std::make_pair( 26, state(lex::T_ELSE        ,  27,   0)));
    table.emplace(std::make_pair( 27, state(lex::T_BEGIN       ,  11,  28)));
    table.emplace(std::make_pair( 28, state(lex::T_END         ,  13,   0)));
    table.emplace(std::make_pair( 28, state(lex::T_NIL         ,  11,  28)));

    table.emplace(std::make_pair(201, state(lex::T_MINUS       , 202,   0)));
    table.emplace(std::make_pair(201, state(lex::T_PLUS        , 202,   0)));
    table.emplace(std::make_pair(201, state(lex::T_NIL         , 202,   0)));
    table.emplace(std::make_pair(202, state(lex::T_ID          , 203,   0)));
    table.emplace(std::make_pair(202, state(lex::T_NUMERIC     , 203,   0)));
    table.emplace(std::make_pair(202, state(lex::T_LBRACKET    , 201, 204)));
    table.emplace(std::make_pair(203, state(lex::T_PLUS        , 202,   0)));//205 - exit
    table.emplace(std::make_pair(203, state(lex::T_MINUS       , 202,   0)));
    table.emplace(std::make_pair(203, state(lex::T_MULT        , 202,   0)));
    table.emplace(std::make_pair(203, state(lex::T_DIV         , 202,   0)));
    table.emplace(std::make_pair(203, state(lex::T_NIL         , 205,   0)));
    table.emplace(std::make_pair(204, state(lex::T_RBRACKET    , 203,   0)));

    table.emplace(std::make_pair(301, state(lex::T_NOT         , 301,   0)));
    table.emplace(std::make_pair(301, state(lex::T_LOG_LBRACKET, 301, 304)));
    table.emplace(std::make_pair(301, state(lex::T_NIL         , 201, 302)));
    table.emplace(std::make_pair(302, state(lex::T_LESS        , 201, 303)));
    table.emplace(std::make_pair(302, state(lex::T_LESS_EQ     , 201, 303)));
    table.emplace(std::make_pair(302, state(lex::T_GREATER     , 201, 303)));
    table.emplace(std::make_pair(302, state(lex::T_GREATER_EQ  , 201, 303)));
    table.emplace(std::make_pair(302, state(lex::T_EQUAL       , 201, 303)));
    table.emplace(std::make_pair(302, state(lex::T_NOT_EQUAL   , 201, 303)));
    table.emplace(std::make_pair(303, state(lex::T_OR          , 301,   0)));//305 - exit
    table.emplace(std::make_pair(303, state(lex::T_AND         , 301,   0)));
    table.emplace(std::make_pair(303, state(lex::T_NIL         , 305,   0)));
    table.emplace(std::make_pair(304, state(lex::T_LOG_RBRACKET, 303,   0)));

    return table;
}

error_map QMicroClangParser::createErrorMap()
{
    static const char* err[] = { 
                                    "%1: parser error: error in declaration part",
                                    "%1: parser error: identifier excepted",
                                    "%1: parser error: declaration error",
                                    "%1: parser error: unkown operator",
                                    "%1: parser error: excepted \'=\' after identifier",
                                    "%1: parser error: excepted \'(\' in operator \'read\'",
                                    "%1: parser error: excepted identifier",
                                    "%1: parser error: excepted \')\'' in operator \'read\'",
                                    "%1: parser error: excepted \'(\' in operator \'write\'",
                                    "%1: parser error: excepted (identifier | numeric constant | string constant)",
                                    "%1: parser error: excepted \')\'' in operator \'write\'",
                                    "%1: parser error: excepted \'do\' in operator \'while\'",
                                    "%1: parser error: excepted \'{\' in operator \'while\'",
                                    "%1: parser error: excepted \'then\' in operator \'if\'",
                                    "%1: parser error: excepted \'{\' in operator \'if\'",
                                    "%1: parser error: excepted \'else\' in operator \'if\'",
                                    "%1: parser error: excepted \'{\' in operator \'if\'(else part)",
                                    "%1: parser error: invalid arithmetic expression",
                                    "%1: parser error: missing \')\' in arithmetic expression",
                                    "%1: parser error: invalid logic expression",
                                    "%1: parser error: missing \']\' in logic expression"
                               };
    error_map tmp;
    tmp.emplace(std::make_pair(  1, err[ 0]));
    tmp.emplace(std::make_pair(  2, err[ 1]));
    tmp.emplace(std::make_pair(  3, err[ 2]));
    tmp.emplace(std::make_pair( 11, err[ 3]));
    tmp.emplace(std::make_pair( 12, err[ 4]));
    tmp.emplace(std::make_pair( 14, err[ 5]));
    tmp.emplace(std::make_pair( 15, err[ 6]));
    tmp.emplace(std::make_pair( 16, err[ 7]));
    tmp.emplace(std::make_pair( 17, err[ 8]));
    tmp.emplace(std::make_pair( 18, err[ 9]));
    tmp.emplace(std::make_pair( 19, err[10]));
    tmp.emplace(std::make_pair( 20, err[11]));
    tmp.emplace(std::make_pair( 21, err[12]));
    tmp.emplace(std::make_pair( 23, err[13]));
    tmp.emplace(std::make_pair( 24, err[14]));
    tmp.emplace(std::make_pair( 26, err[15]));
    tmp.emplace(std::make_pair( 27, err[16]));
    tmp.emplace(std::make_pair(202, err[17]));
    tmp.emplace(std::make_pair(204, err[18]));
    tmp.emplace(std::make_pair(302, err[19]));
    tmp.emplace(std::make_pair(304, err[20]));

    return tmp;
}

bool QMicroClangParser::parse()
{
    uint state_n = 1;
    bool read_from_stack = false;
    std::stack<uint> stack;
    value_table::size_type cur_lex, size;

    for (cur_lex = 0, size = p->res_table.size(); cur_lex < size;)
    {
        if (read_from_stack == true)
        {
            state_n = stack.top();
            stack.pop();
            read_from_stack = false;
        }

        if (state_n != 13)
        {
            auto state_ = states.equal_range(state_n);
            bool found = false;

#if _DEBUG
            assert(std::distance(state_.first, state_.second) != 0);
#endif
            
            auto it = state_.first;
            for (; it != state_.second; ++it)
            {
                if (it->second.lexeme == p->res_table[cur_lex].type || it->second.lexeme == lex::T_NIL)
                {
                    if (it->second.lexeme != lex::T_NIL)
                    {
                        ++cur_lex;
                        p->res_stack.emplace_back(std::vector<uint>(stack._Get_container().begin(), stack._Get_container().end()));
                    }
                    state_n = it->second.next_state;
                    if (it->second.stack_eq != 0)
                        stack.push(it->second.stack_eq);
                    found = true;
                    break;
                }
            }
    
            if (found == false)
            {
                auto err_it = error.find((--it)->first);
                pushError(QString((err_it != error.end() ? err_it->second : "%1: parse error")).arg(p->res_table[cur_lex].line_num));
                break;
            }
        }

        if (state_n == 13)
        {
            if (p->res_table[cur_lex++].type == lex::T_END_OP)
            {
                p->res_stack.emplace_back(std::vector<uint>(stack._Get_container().begin(), stack._Get_container().end()));
                read_from_stack = true;
                continue;
            }
            else
            {
                pushError(QString("%1: parser error: excepted \';\' after operator").arg(p->res_table[cur_lex - 1].line_num));
                break;
            }
        }

        if (state_n == 205 || state_n == 305)
        {
            read_from_stack = true;
            continue;
        }
    }
    
    if (stack.empty() == false)
        pushError(QString("%1: parser error: \'}\' excepted").arg(p->res_table[cur_lex - 1].line_num));

    return (cur_lex == size && stack.empty());
}

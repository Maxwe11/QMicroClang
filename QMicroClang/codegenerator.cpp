#include "codegenerator.h"

#include "compiler.h"
#include "polish_elemnts.h"

#include <stack>
#include <QtCore/QDebug>

QMicroClangCodeGenerator::QMicroClangCodeGenerator(QMicroClangCompiler* parent) 
    : p(parent)
    , priority(createPriorityTable())
{
}

QMicroClangCodeGenerator::~QMicroClangCodeGenerator()
{
}

priority_table QMicroClangCodeGenerator::createPriorityTable()
{
    priority_table tmp;

    tmp.emplace(std::make_pair(lex::T_READ        , 0));
    tmp.emplace(std::make_pair(lex::T_WRITE       , 0));
    tmp.emplace(std::make_pair(lex::T_IF          , 0));
    tmp.emplace(std::make_pair(lex::T_WHILE       , 0));
    tmp.emplace(std::make_pair(lex::T_BEGIN       , 0));
    tmp.emplace(std::make_pair(lex::T_LBRACKET    , 0));
    tmp.emplace(std::make_pair(lex::T_LOG_LBRACKET, 0));

    tmp.emplace(std::make_pair(lex::T_THEN        , 1));
    tmp.emplace(std::make_pair(lex::T_ELSE        , 1));
    tmp.emplace(std::make_pair(lex::T_DO          , 1));
    tmp.emplace(std::make_pair(lex::T_END         , 1));
    tmp.emplace(std::make_pair(lex::T_END_OP      , 1));
    tmp.emplace(std::make_pair(lex::T_RBRACKET    , 1));
    tmp.emplace(std::make_pair(lex::T_LOG_RBRACKET, 1));

    tmp.emplace(std::make_pair(lex::T_ASSIGM      , 2));

    tmp.emplace(std::make_pair(lex::T_OR          , 3));

    tmp.emplace(std::make_pair(lex::T_AND         , 4));

    tmp.emplace(std::make_pair(lex::T_NOT         , 5));

    tmp.emplace(std::make_pair(lex::T_LESS        , 6));
    tmp.emplace(std::make_pair(lex::T_LESS_EQ     , 6));
    tmp.emplace(std::make_pair(lex::T_GREATER     , 6));
    tmp.emplace(std::make_pair(lex::T_GREATER_EQ  , 6));
    tmp.emplace(std::make_pair(lex::T_EQUAL       , 6));
    tmp.emplace(std::make_pair(lex::T_NOT_EQUAL   , 6));

    tmp.emplace(std::make_pair(lex::T_PLUS        , 7));
    tmp.emplace(std::make_pair(lex::T_MINUS       , 7));

    tmp.emplace(std::make_pair(lex::T_MULT        , 8));
    tmp.emplace(std::make_pair(lex::T_DIV         , 8));

    return tmp;
}

void QMicroClangCodeGenerator::generate()
{
    lexeme_list::iterator it = p->res_table.begin();
    
    while (it->type != lex::T_BEGIN)
    {
        if (it->type == lex::T_ID)
            p->variables << p->id_table[it->table_indx - 1];
        ++it;
    }

    p->variables << "|";
    operator_(it);
}

void QMicroClangCodeGenerator::operator_(lexeme_list::iterator& it)
{
    std::stack<std::pair<lex::type, bool>> stack;
    bool unary_minus(true);
    uint args(0), labels(0), tmp;
    std::stack<uint> deepness;
    lexeme_list::iterator& end = p->res_table.end();

    while (it != end)
    {
        if (stack.empty() && it->type != lex::T_ID && it->type != lex::T_NUMERIC)
        {
            stack.push(std::make_pair(it->type, it->type == lex::T_MINUS && unary_minus));
            ++it;
            continue;
        }

        switch (it->type)
        {
        case lex::T_ID:
            unary_minus = false;
            p->polish_vec.push_back(polish_element(p->id_table[it->table_indx - 1], polish::T_VARIABLE));
            break;
        case lex::T_NUMERIC:
            unary_minus = false;
            p->polish_vec.push_back(polish_element(p->num_table[it->table_indx - 1], polish::T_CONSTANT));
            break;
        case lex::T_STRING:
            p->polish_vec.push_back(polish_element(QString("\"%1\"").arg(p->str_table[it->table_indx - 1]), polish::T_STRING));
            break;
        case lex::T_BEGIN:
            stack.push(std::make_pair(it->type, false));
            break;
        case lex::T_LBRACKET:
        case lex::T_LOG_LBRACKET:
            stack.push(std::make_pair(it->type, false));
            unary_minus = true;
            break;
        case lex::T_RBRACKET:
            args = 1;
            while (stack.top().first != lex::T_LBRACKET)
            {
                if (stack.top().first == lex::T_MINUS && stack.top().second == true)
                    p->polish_vec.push_back(polish_element("@", polish::T_UNARY_OP));
                else if (stack.top().first == lex::T_NOT)
                    p->polish_vec.push_back(polish_element("!", polish::T_UNARY_OP));
                else if (stack.top().first == lex::T_COMMA)
                    ++args;
                else
                    p->polish_vec.push_back(polish_element(p->lexemeByType(stack.top().first), polish::T_BINARY_OP));
                stack.pop();
            }
            stack.pop();
            if (stack.top().first == lex::T_READ || stack.top().first == lex::T_WRITE)
                p->polish_vec.push_back(polish_element(QString("%1").arg(args), polish::T_CONSTANT));
            break;
        case lex::T_LOG_RBRACKET:
            while (stack.top().first != lex::T_LOG_LBRACKET)
            {
                if (stack.top().first == lex::T_MINUS && stack.top().second == true)
                    p->polish_vec.push_back(polish_element("@", polish::T_UNARY_OP));
                else if (stack.top().first == lex::T_NOT)
                    p->polish_vec.push_back(polish_element("!", polish::T_UNARY_OP));
                else
                    p->polish_vec.push_back(polish_element(p->lexemeByType(stack.top().first), polish::T_BINARY_OP));
                stack.pop();
            }
            stack.pop();
            break;
        case lex::T_END:
            stack.pop();
            break;
        case lex::T_END_OP:
            if (!stack.empty() && stack.top().first == lex::T_WHILE)
            {
                p->polish_vec.push_back(polish_element(deepness.top()));
                deepness.pop();
                p->polish_vec.push_back(polish_element("JMP", polish::T_JMP));
                p->polish_vec.push_back(polish_element(deepness.top()));
                deepness.pop();
                stack.pop();
            }
            if (!stack.empty() && stack.top().first == lex::T_IF)
            {
                p->polish_vec.push_back(polish_element(deepness.top()));
                deepness.pop();
                stack.pop();
            }
            while (stack.top().first != lex::T_BEGIN)
            {
                if (stack.top().first == lex::T_MINUS && stack.top().second == true)
                    p->polish_vec.push_back(polish_element("@", polish::T_UNARY_OP));
                else if (stack.top().first == lex::T_NOT)
                    p->polish_vec.push_back(polish_element("!", polish::T_UNARY_OP));
                else if (stack.top().first == lex::T_WRITE)
                    p->polish_vec.push_back(polish_element("WRITE", polish::T_WRITE));
                else if (stack.top().first == lex::T_READ)
                    p->polish_vec.push_back(polish_element("READ", polish::T_READ));
                else
                    p->polish_vec.push_back(polish_element(p->lexemeByType(stack.top().first), polish::T_BINARY_OP));
                stack.pop();
            }
            break;
        case lex::T_WHILE:
            p->polish_vec.push_back(polish_element(++labels));
        case lex::T_IF:
        case lex::T_READ:
        case lex::T_WRITE:
        case lex::T_COMMA:
            stack.push(std::make_pair(it->type, false));
            break;
        case lex::T_DO:
            while (stack.top().first != lex::T_WHILE)
            {
                if (stack.top().first == lex::T_MINUS && stack.top().second == true)
                    p->polish_vec.push_back(polish_element("@", polish::T_UNARY_OP));
                else if (stack.top().first == lex::T_NOT)
                    p->polish_vec.push_back(polish_element("!", polish::T_UNARY_OP));
                else
                    p->polish_vec.push_back(polish_element(p->lexemeByType(stack.top().first), polish::T_BINARY_OP));
                stack.pop();
            }
            p->polish_vec.push_back(polish_element(++labels));
            p->polish_vec.push_back(polish_element("JIF", polish::T_JIF));
            deepness.push(labels);
            deepness.push(labels - 1);
            break;
        case lex::T_THEN:
            while (stack.top().first != lex::T_IF)
            {
                if (stack.top().first == lex::T_MINUS && stack.top().second == true)
                    p->polish_vec.push_back(polish_element("@", polish::T_UNARY_OP));
                else if (stack.top().first == lex::T_NOT)
                    p->polish_vec.push_back(polish_element("!", polish::T_UNARY_OP));
                else
                    p->polish_vec.push_back(polish_element(p->lexemeByType(stack.top().first), polish::T_BINARY_OP));
                stack.pop();
            }
            p->polish_vec.push_back(polish_element(++labels));
            p->polish_vec.push_back(polish_element("JIF", polish::T_JIF));
            deepness.push(labels);
            deepness.push(++labels);
            break;
        case lex::T_ELSE:
            while (stack.top().first != lex::T_IF)
            {
                if (stack.top().first == lex::T_MINUS && stack.top().second == true)
                    p->polish_vec.push_back(polish_element("@", polish::T_UNARY_OP));
                else if (stack.top().first == lex::T_NOT)
                    p->polish_vec.push_back(polish_element("!", polish::T_UNARY_OP));
                else
                    p->polish_vec.push_back(polish_element(p->lexemeByType(stack.top().first), polish::T_BINARY_OP));
                stack.pop();
            }
            tmp = deepness.top();
            p->polish_vec.push_back(polish_element(tmp));
            deepness.pop();
            p->polish_vec.push_back(polish_element("JMP", polish::T_JMP));
            p->polish_vec.push_back(polish_element(deepness.top()));
            deepness.pop();
            deepness.push(tmp);
            break;
        case lex::T_ASSIGM:
        case lex::T_OR:
        case lex::T_AND:
        case lex::T_NOT:
        case lex::T_LESS:
        case lex::T_LESS_EQ:
        case lex::T_GREATER:
        case lex::T_GREATER_EQ:
        case lex::T_EQUAL:
        case lex::T_NOT_EQUAL:
            unary_minus = true;
        case lex::T_PLUS:
        case lex::T_MULT:
        case lex::T_DIV:
            if (priority.find(stack.top().first)->second < priority.find(it->type)->second)
            {
                stack.push(std::make_pair(it->type, false));
            }
            else
            {
                if (stack.top().first == lex::T_MINUS && stack.top().second == true)
                    p->polish_vec.push_back(polish_element("@", polish::T_UNARY_OP));
                else if (stack.top().first == lex::T_NOT)
                    p->polish_vec.push_back(polish_element("!", polish::T_UNARY_OP));
                else
                    p->polish_vec.push_back(polish_element(p->lexemeByType(stack.top().first), polish::T_BINARY_OP));
                stack.pop();
                continue;
            }
            break;
        case lex::T_MINUS:
            if (unary_minus == true)
            {
                if (priority.find(stack.top().first)->second < 8)
                {
                    stack.push(std::make_pair(it->type, true)); 
                }
                else
                {
                    p->polish_vec.push_back(polish_element("@", polish::T_UNARY_OP));
                    stack.pop();
                    continue;
                }
            }
            else
            {
                if (priority.find(stack.top().first)->second < 7)
                {
                    stack.push(std::make_pair(it->type, false));
                }
                else
                {
                    if (stack.top().first == lex::T_MINUS && stack.top().second == true)
                        p->polish_vec.push_back(polish_element("@", polish::T_UNARY_OP));
                    else if (stack.top().first == lex::T_NOT)
                        p->polish_vec.push_back(polish_element("!", polish::T_UNARY_OP));
                    else
                        p->polish_vec.push_back(polish_element(p->lexemeByType(stack.top().first), polish::T_BINARY_OP));
                    stack.pop();
                    continue;
                }
            }
            break;
        }
        ++it;
    }
}
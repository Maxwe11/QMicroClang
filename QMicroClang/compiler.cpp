#include "compiler.h"

#include <QtCore\QDebug>

#include "scanner.h"
#include "parser.h"
#include "codegenerator.h"

QMicroClangCompiler::QMicroClangCompiler(QObject* parent)
    : QObject(parent)
    , keyword(createKeywordTable())
    , scanner(new QMicroClangScanner(this))
    , parser(new QMicroClangParser(this))
    , generator(new QMicroClangCodeGenerator(this))
{  
}

QMicroClangCompiler::~QMicroClangCompiler()
{
    delete scanner;
    delete parser;
    delete generator;
}

void QMicroClangCompiler::clearAllTables()
{
    variables.clear();
    res_stack.clear();
    res_table.clear();
    err_list.clear();
    id_table.clear();
    str_table.clear();
    num_table.clear();
    polish_vec.clear();
    polish_str.clear();
}

QString QMicroClangCompiler::lexemeByType(lex::type type)
{
    for (auto it = keyword.cbegin(), end = keyword.cend(); it != end; ++it)
    {
        if (it->second == type)
            return it->first;
    }
}

keyword_table QMicroClangCompiler::createKeywordTable()
{
    keyword_table tmp;
    
    tmp.emplace(std::make_pair("double" , lex::T_DOUBLE      ));
    tmp.emplace(std::make_pair("read"   , lex::T_READ        ));
    tmp.emplace(std::make_pair("write"  , lex::T_WRITE       ));
    tmp.emplace(std::make_pair("if"     , lex::T_IF          ));
    tmp.emplace(std::make_pair("then"   , lex::T_THEN        ));
    tmp.emplace(std::make_pair("else"   , lex::T_ELSE        ));
    tmp.emplace(std::make_pair("while"  , lex::T_WHILE       ));
    tmp.emplace(std::make_pair("do"     , lex::T_DO          ));
    tmp.emplace(std::make_pair("||"     , lex::T_OR          ));
    tmp.emplace(std::make_pair("&&"     , lex::T_AND         ));
    tmp.emplace(std::make_pair("!"      , lex::T_NOT         ));
    tmp.emplace(std::make_pair("{"      , lex::T_BEGIN       ));
    tmp.emplace(std::make_pair("}"      , lex::T_END         ));
    tmp.emplace(std::make_pair(";"      , lex::T_END_OP      ));
    tmp.emplace(std::make_pair(","      , lex::T_COMMA       ));
    tmp.emplace(std::make_pair("+"      , lex::T_PLUS        ));
    tmp.emplace(std::make_pair("-"      , lex::T_MINUS       ));
    tmp.emplace(std::make_pair("*"      , lex::T_MULT        ));
    tmp.emplace(std::make_pair("/"      , lex::T_DIV         ));
    tmp.emplace(std::make_pair("="      , lex::T_ASSIGM      ));
    tmp.emplace(std::make_pair(">"      , lex::T_GREATER     ));
    tmp.emplace(std::make_pair(">="     , lex::T_GREATER_EQ  ));
    tmp.emplace(std::make_pair("<"      , lex::T_LESS        ));
    tmp.emplace(std::make_pair("<="     , lex::T_LESS_EQ     ));
    tmp.emplace(std::make_pair("=="     , lex::T_EQUAL       ));
    tmp.emplace(std::make_pair("!="     , lex::T_NOT_EQUAL   ));
    tmp.emplace(std::make_pair("("      , lex::T_LBRACKET    ));
    tmp.emplace(std::make_pair(")"      , lex::T_RBRACKET    ));
    tmp.emplace(std::make_pair("["      , lex::T_LOG_LBRACKET));
    tmp.emplace(std::make_pair("]"      , lex::T_LOG_RBRACKET));
    tmp.emplace(std::make_pair("id"     , lex::T_ID          ));
    tmp.emplace(std::make_pair("numeric", lex::T_NUMERIC     ));
    tmp.emplace(std::make_pair("string" , lex::T_STRING      ));

    return tmp;
}

bool QMicroClangCompiler::run(const QStringList &list)
{
    clearAllTables();

    if (scanner->scan(list) && parser->parse())
    {
        generator->generate();
        std::for_each(polish_vec.begin(), polish_vec.end(), [this](const polish_element& t)
        {
            polish_str += QString("%1 ").arg(t.toString());
            variables << t.toString();
        });
    }
    
    if (err_list.isEmpty() == false)
        emit this->error(err_list);
    else
        emit this->noErrors();

    return err_list.isEmpty();
}

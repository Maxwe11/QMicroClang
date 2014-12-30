#include "qmicroclangparser.h"

QMicroClangParser::QMicroClangParser(QObject* parent)
    : QObject(parent)
{
    keywords["double" ] =  1;
    keywords["read"   ] =  2;
    keywords["write"  ] =  3;
    keywords["if"     ] =  4;
    keywords["then"   ] =  5;
    keywords["else"   ] =  6;
    keywords["while"  ] =  7;
    keywords["do"     ] =  8;
    keywords["||"     ] =  9;
    keywords["&&"     ] = 10;
    keywords["!"      ] = 11;
    keywords["{"      ] = 12;
    keywords["}"      ] = 13;
    keywords[";"      ] = 14;
    keywords[","      ] = 15;
    keywords["+"      ] = 16;
    keywords["-"      ] = 17;
    keywords["*"      ] = 18;
    keywords["/"      ] = 19;
    keywords["="      ] = 20;
    keywords[">"      ] = 21;
    keywords[">="     ] = 22;
    keywords["<"      ] = 23;
    keywords["<="     ] = 24;
    keywords["=="     ] = 25;
    keywords["!="     ] = 26;
    keywords["("      ] = 27;
    keywords[")"      ] = 28;
    keywords["["      ] = 29;
    keywords["]"      ] = 30;
    keywords["id"     ] = 31;
    keywords["numeric"] = 32;
    keywords["string" ] = 33;
}

bool QMicroClangParser::parse(const QStringList &list)
{
    res_table.clear();
    err_list.clear();
    id_table.clear();
    str_table.clear();
    num_table.clear();

    if (scan(list) == false)
    {
        emit this->error(err_list);
        return false;
    }

    uint cur_lex(0);
    bool res = false;

    if (declarationList(cur_lex) == true)
    {
        if (res_table[cur_lex].lex_code == 12) // 12 - {
        {
            if (operatorsList(++cur_lex) == true)
            {
                if (cur_lex < res_table.size() && res_table[cur_lex].lex_code == 13)
                    res = true;
                else
                    err_list << QString("%1: parser error: \'}\' excepted").arg(res_table[cur_lex - 1].line_num);
            }
        }
    }

    if (cur_lex < res_table.size() - 1 && res == true)
    {
        res = false;
        err_list << QString("%1: parser error: end of program found before end of file: \'{\' excepted").arg(res_table[cur_lex - 1].line_num);
    }

    if (res == false)
        emit this->error(err_list);
    else
        emit this->noErrors();

    return res;
}

bool QMicroClangParser::scan(QStringList code)
{
    bool declr_sect(true);
    uint line_number(0), count_id(0), count_str(0), count_const(0), lex_code(0), id_index(0);
    QString token;

    for (
            QStringList::iterator it = code.begin(), end = code.end();
            it != end && err_list.empty();
            ++it
        )
    {
        ++line_number;

        while ( it->isEmpty() == false && err_list.isEmpty() == true )
        {
            token.clear();
            lexeme_type lex_type;
            if (getToken(*it, token, lex_type, line_number) == true)
            {
                switch (lex_type)
                {
                case L_KEYW:
                    lex_code = keywords.find(token)->second;
                    res_table.push_back(lexeme(line_number, token, L_KEYW, lex_code, 0));
                    if (lex_code == 12) //12 - left figure parent {
                        declr_sect = false;
                    break;
                case L_ID:
                    id_index = findID(token);
                    if (declr_sect == true)
                    {
                        if (id_index != 0)
                        {
                            err_list << QString("%1: scan error: identifier is already existed: %2").arg(line_number).arg(token);
                        }
                        else
                        {
                            res_table.push_back(lexeme(line_number, token, L_ID, 31, ++count_id));
                            id_table.push_back(T_pair(token, count_id));
                        }
                    }
                    else
                    {
                        if (id_index != 0)
                            res_table.push_back(lexeme(line_number, token, L_ID, 31, id_index));
                        else
                            err_list << QString("%1: scan error: undeclared identifier: %2").arg(line_number).arg(token);
                    }
                    break;
                case L_NUMERIC:
                    res_table.push_back(lexeme(line_number, token, L_NUMERIC, 32, ++count_const));
                    num_table.push_back(T_pair(token, count_const));
                    break;
                case L_STRING:
                    res_table.push_back(lexeme(line_number, token, L_STRING, 33, ++count_str));
                    str_table.push_back(T_pair(token, count_str));
                    break;
                }
            }
        }
    }

    return err_list.isEmpty();
}

bool QMicroClangParser::getToken(QString &str, QString &token, lexeme_type &lex, uint line)
{
    uint cur(0), len = static_cast<uint>(str.length());
    //Пробелы и табуляция
    if (isSpace(str[cur]) == true)
    {
        str.remove(0, 1);
        return false;
    }
    // * Ключевые слова и идентификаторы
    // * Примечание: проверка идентификатора на существование/повтор происходив в Scanner::scan
    else if (str[cur].isLetter())
    {
        token = str[cur++];

        while ( cur < len && ( str[cur].isLetter() || str[cur].isDigit() ) )
            token += str[cur++];

        lex = (keywords.count(token) == 1 ? L_KEYW : L_ID);
        str.remove(0, cur);
        return true;
    }

    else if (str[cur] == '.')
    {
        token = str[cur++];

        if ( cur < len && str[cur].isDigit() )
        {
            token += str[cur++];

            while ( cur < len && str[cur].isDigit() )
                token += str[cur++];

            if ( cur < len && str[cur] == 'e' )
                getExpNumPart(str, token, cur, line);

            if (err_list.empty() == true)
            {
                lex = L_NUMERIC;
                str.remove(0, cur);
                return true;
            }
        }

        err_list << QString("%1: scan error: invalid constant value: %2").arg(line).arg(token);
        return false;
    }

    else if (str[cur].isDigit())
    {
        token = str[cur++];

        while ( cur < len && str[cur].isDigit() )
            token += str[cur++];

        if ( cur < len && str[cur] == '.' )
        {
            token += str[cur++];

            while ( cur < len && str[cur].isDigit() )
                token += str[cur++];
        }

        if ( cur < len && str[cur] == 'e' )
            getExpNumPart(str, token, cur, line);

        if (err_list.empty())
        {
            lex = L_NUMERIC;
            str.remove(0, cur);
            return true;
        }

         return false;
    }

    else if (isSingleDelimiter(str[cur]))
    {
        token = str[cur++];
        lex = L_KEYW;
        str.remove(0, 1);
        return true;
    }

    else if (str[cur] == '|' || str[cur] == '&')
    {
        token = str[cur++];

        if (cur < len && str[cur] == str[cur - 1])
        {
            token += str[cur++];
            lex = L_KEYW;
            str.remove(0, cur);
            return true;
        }

        err_list << QString("%1: scan error: unresolved external symbol: %2").arg(line).arg(token);
        return false;
    }

    else if (str[cur] == '!' || str[cur] == '=' || str[cur] == '>' || str[cur] == '<')
    {
        token = str[cur++];

        if (str[cur] == '=')
            token += str[cur++];

        lex = L_KEYW;
        str.remove(0, cur);
        return true;
    }

    else if (str[cur++] == '\"')
    {
        token = "";
        bool err(true);

        while (cur < len)
        {
            if (str[cur] == '\"')
            {
                err = false;
                break;
            }
            token += str[cur++];
        }

         if (err == true)
         {
             err_list << QString("%1: scan error: unclosed string mark").arg(line);
             return false;
         }
         else
         {
             lex = L_STRING;
             str.remove(0, ++cur);
             return true;
         }
    }

    else
    {
        err_list << QString("%1: scan error: unresolved external symbol").arg(line);
        return false;
    }
}

void QMicroClangParser::getExpNumPart(const QString &str, QString &token, uint &cur, uint line)
{
    uint len = str.length();
    token += str[cur++];

    if ( cur < len && str[cur].isDigit() )
    {
        token += str[cur++];

        while ( cur < len && str[cur].isDigit() )
            token += str[cur++];
    }
    else if ( cur < len && (str[cur] == '+' || str[cur] == '-') )
    {
        token += str[cur++];

        if ( cur < len && str[cur].isDigit() )
        {
            token += str[cur++];

            while ( cur < len && str[cur].isDigit() )
                token += str[cur++];

        }
        else
        {
            err_list << QString("%1: scan error: invalid constant value: %2").arg(line).arg(token);
        }
    }
    else
    {
        err_list << QString("%1: scan error: invalid constant value: %2").arg(line).arg(token);
    }
}

bool QMicroClangParser::declarationList(uint &cur_lex)
{
    bool res = false;
    uint cur = cur_lex;

    if (declaration(cur_lex) == true)
    {
        ++cur_lex;
        res = true;
        declarationList(cur_lex);
    }
    else
    {
        if (cur != cur_lex)
            err_list << QString("%1: parser error: undeclared \'%2\'").arg(res_table[cur_lex].line_num).arg(res_table[cur_lex].lex);
        else
            res = true;
    }

    return res;
}

bool QMicroClangParser::declaration(uint& cur_lex)
{
    static uint counter(0);
    ++counter;

    bool res = false;

    if (res_table[cur_lex].lex_code == 1) // 1 - double
    {
        if (idList(++cur_lex) == true)
        {
            if (res_table[cur_lex].lex_code == 14) // 14 - ; точка с запятой
                res = true;
            else
                err_list << QString("%1: parser error: excepted \';\'").arg(res_table[cur_lex].line_num);
        }
    }

    if (res == false && counter == 1)
    {
        err_list << QString("%1: parser error: excepted declaration part").arg(res_table[cur_lex].line_num);
    }

    return res;
}

bool QMicroClangParser::idList(uint& cur_lex)
{
    bool res = false;

    if (res_table[cur_lex].lex_code == 31) // 31 - id
    {
        ++cur_lex;
        res = true;

        while (res_table[cur_lex].lex_code == 15 && res) //15 - , запятая
        {
            ++cur_lex;
            if (res_table[cur_lex].lex_code == 31) // 31 - id
                ++cur_lex;
            else
            {
                res = false;
                err_list << QString("%1: parser error: identifier excepted after \',\'").arg(res_table[cur_lex].line_num);
            }
        }
    }
    else
    {
        err_list << QString("%1: parser error: identifier excepted").arg(res_table[cur_lex].line_num);
    }

    return res;
}

bool QMicroClangParser::operatorsList(uint& cur_lex)
{
    bool res = false;

    if (operator_(cur_lex) == true)
    {
        ++cur_lex;
        res = true;
        while (cur_lex < res_table.size() && res_table[cur_lex].lex_code != 13 && res == true)
        {
            if (operator_(cur_lex) == true)
                ++cur_lex;
            else
                break;
        }
    }

    if (err_list.isEmpty() == false)
        res = false;

    return res;
}

bool QMicroClangParser::operator_(uint& cur_lex)
{
    bool res = false;
    //присвоение
    if (res_table[cur_lex].lex_code == 31) // 31 - id
    {
        if (res_table[++cur_lex].lex_code == 20) //20 - = equal
            if (expression(++cur_lex) == true)
                if (res_table[++cur_lex].lex_code == 14) // 14 - ;
                    res = true;
    }
    //read
    else if (res_table[cur_lex].lex_code == 2) // 2 - read
    {
        if (res_table[++cur_lex].lex_code == 27) //27 - (
            if (idList(++cur_lex) == true)
                if (res_table[cur_lex].lex_code == 28) //28 - )
                    if (res_table[++cur_lex].lex_code == 14) // 14 - ;
                        res = true;
    }
    //write
    else if (res_table[cur_lex].lex_code == 3) // 3 - write
    {
        if (res_table[++cur_lex].lex_code == 27) //27 - (
            if (outputList(++cur_lex) == true)
                if (res_table[cur_lex].lex_code == 28) //28 - )
                    if (res_table[++cur_lex].lex_code == 14) // 14 - ;
                        res = true;

    }
    //while
    else if (res_table[cur_lex].lex_code == 7) //7 - while
    {
        if (booleanExpression(++cur_lex) == true)
            if (res_table[++cur_lex].lex_code == 8) //8 - do
                if (statement(++cur_lex) == true)
                    res = true;
    }
    //if
    else if (res_table[cur_lex].lex_code == 4) //4 - if
    {
        if (booleanExpression(++cur_lex) == true)
            if (res_table[++cur_lex].lex_code == 5) //5 - then
                if (statement(++cur_lex) == true)
                    if (res_table[++cur_lex].lex_code == 6) //6 - else
                        if (statement(++cur_lex) == true)
                            res = true;
    }

    if (res == false)
    {
        err_list << QString("%1: parser error: unkown operator").arg(res_table[cur_lex].line_num);;
    }

    return res;
}

bool QMicroClangParser::outputList(uint& cur_lex)
{
    bool res = false;

    if (res_table[cur_lex].lex_code == 31 || res_table[cur_lex].lex_code == 32 || res_table[cur_lex].lex_code == 33)
    {
        ++cur_lex;
        res = true;
        while (res_table[cur_lex].lex_code == 15 && res) //15 - , запятая
        {
            ++cur_lex;
            if (res_table[cur_lex].lex_code == 31 || res_table[cur_lex].lex_code == 32 || res_table[cur_lex].lex_code == 33) // 31 - id
                ++cur_lex;
            else
            {
                res = false;
                err_list << QString("%1: parser error: element of output excepted after \',\'").arg(res_table[cur_lex].line_num);
            }
        }
    }
    else
    {
        err_list << QString("%1: parser error: output list cannot be empty").arg(res_table[cur_lex].line_num);
    }

    return res;
}

bool QMicroClangParser::statement(uint& cur_lex)
{
    bool res = false;

    if (res_table[cur_lex].lex_code == 12) //12 - {
    {
        if (operatorsList(++cur_lex) == true)
        {
            if (res_table[cur_lex].lex_code == 13) //13 - }
                res = true;
            else
                err_list << QString("%1: parser error: missing closing bracket \'}\'").arg(res_table[cur_lex].line_num);
        }
    }
    else if (operator_(cur_lex) == true)
    {
        res = true;
    }

    return res;
}

bool QMicroClangParser::expression(uint& cur_lex)
{
    bool res = false;

    if (res_table[cur_lex].lex_code == 16 || res_table[cur_lex].lex_code == 17) //16 - знак + 17 - знак минус -
    {
        ++cur_lex;
    }

    if (term(cur_lex) == true)
    {
        while (true)
        {
            res = true;
            ++cur_lex;
            if (res_table[cur_lex].lex_code == 16 || res_table[cur_lex].lex_code == 17) //16 - знак + 17 - знак минус -
            {
                if (term(++cur_lex) == true)
                    res = true;
            }
            else
            {
                --cur_lex;
                break;
            }
        }
    }

    if (res == false)
        err_list << QString("%1: parser error: invalid arithmetic expression").arg(res_table[cur_lex].line_num);

    return res;
}

bool QMicroClangParser::term(uint& cur_lex)
{
    bool res = false;

    if (factor(cur_lex) == true)
    {
        while (true)
        {
            res = true;
            ++cur_lex;
            if (res_table[cur_lex].lex_code == 18 || res_table[cur_lex].lex_code == 19) // 18 - знак * умножения  19 - знак / - деления
            {
                if (factor(++cur_lex) == true)
                    res = true;
            }
            else
            {
                --cur_lex;
                break;
            }
        }
    }

    return res;
}

bool QMicroClangParser::factor(uint& cur_lex)
{
    bool res = false;

    if (res_table[cur_lex].lex_code == 31) //31 - id
        res = true;
    else if (res_table[cur_lex].lex_code == 32) //32 - numeric
        res = true;
    else if (res_table[cur_lex].lex_code == 27) //27 - (
    {
        if (expression(++cur_lex) == true)
        {
            if (res_table[++cur_lex].lex_code == 28) //28 - )
                res = true;
        }
    }

    return res;
}

bool QMicroClangParser::booleanExpression(uint& cur_lex)
{
    bool res = false;

    if (booleanTerm(cur_lex) == true)
    {
        while (true)
        {
            res = true;
            if (res_table[++cur_lex].lex_code == 9) // 9 - || - or
            {
                if (booleanTerm(++cur_lex) == true)
                    res = true;
            }
            else
            {
                --cur_lex;
                break;
            }
        }
    }

    if (res == false)
        err_list << QString("%1: parser error: invalid logic expression").arg(res_table[cur_lex].line_num);

    return res;
}

bool QMicroClangParser::booleanTerm(uint& cur_lex)
{
    bool res = false;

    if (booleanFactor(cur_lex) == true)
    {
        while (true)
        {
            res = true;
            if (res_table[++cur_lex].lex_code == 10) //10 - && and
            {
                if (booleanFactor(++cur_lex) == true)
                    res = true;
            }
            else
            {
                --cur_lex;
                break;
            }
        }
    }

    return res;
}

bool QMicroClangParser::booleanFactor(uint& cur_lex)
{
    bool res = false;

    if (res_table[cur_lex].lex_code == 11) // 11- ! - not
    {
        if (booleanExpression(++cur_lex) == true)
            res = true;
    }
    else if (res_table[cur_lex].lex_code == 29) // 29 - [
    {
        if (booleanExpression(++cur_lex) == true)
        {
            if (res_table[++cur_lex].lex_code == 30) // 30 - ]
                res = true;
        }
    }
    else if (expression(cur_lex) == true)
    {
        ++cur_lex;
        if (res_table[cur_lex].lex_code >= 21 && res_table[cur_lex].lex_code <= 26) //21 - 26 > >= < <= == !=
        {
            if (expression(++cur_lex) == true)
                res = true;
        }
    }

    return res;
}

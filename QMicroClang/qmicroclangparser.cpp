#include <QDebug>
#include <QStack>

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
    res_stack.clear();
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

    uint cur_lex(0), state(1);
    bool exit = false;

    QStack<uint> stack;

    while (exit == false)
    {
        switch (state)
        {
        case 1:
            if (res_table[cur_lex].lex_code == 1) //1 - double
            {
                res_stack.push_back(std::vector<uint>(stack.begin(), stack.end()));
                state = 2;
                ++cur_lex;
            }
            else
            {
                err_list << QString("%1: parser error: excepted declaration part").arg(res_table[cur_lex].line_num);
                exit = true;
            }
            break;
        case 2:
            if (res_table[cur_lex].lex_code == 31) //31 - id
            {
                res_stack.push_back(std::vector<uint>(stack.begin(), stack.end()));
                state = 3;
                ++cur_lex;
            }
            else
            {
                err_list << QString("%1: parser error: identifier excepted").arg(res_table[cur_lex].line_num);
                exit = true;
            }
            break;
        case 3:
            if (res_table[cur_lex].lex_code == 15) //15 - ,
            {
                res_stack.push_back(std::vector<uint>(stack.begin(), stack.end()));
                state = 2;
                ++cur_lex;
            }
            else if (res_table[cur_lex].lex_code == 14) //14 - ;
            {
                res_stack.push_back(std::vector<uint>(stack.begin(), stack.end()));
                state = 4;
                ++cur_lex;
            }
            else
            {
                err_list << QString("%1: parser error: declaration error").arg(res_table[cur_lex].line_num);
                exit = true;
            }
            break;
        case 4:
            if (res_table[cur_lex].lex_code == 12) //12 - {
            {
                res_stack.push_back(std::vector<uint>(stack.begin(), stack.end()));
                stack.push(5);
                state = 11;
                ++cur_lex;
            }
            else
            {
                state = 1;
            }
            break;
        case 5:
            if (res_table[cur_lex].lex_code == 13) //13 - }
            {
                exit = true;
            }
            else
            {
                stack.push(5);
                state = 11;
            }
            break;
        case 11:
            if (res_table[cur_lex].lex_code == 31) //31 - id
            {
                res_stack.push_back(std::vector<uint>(stack.begin(), stack.end()));
                state = 12;
                ++cur_lex;
            }
            else if (res_table[cur_lex].lex_code == 2) //2 - read
            {
                res_stack.push_back(std::vector<uint>(stack.begin(), stack.end()));
                state = 14;
                ++cur_lex;
            }
            else if (res_table[cur_lex].lex_code == 3) //3 - write
            {
                res_stack.push_back(std::vector<uint>(stack.begin(), stack.end()));
                state = 17;
                ++cur_lex;
            }
            else if (res_table[cur_lex].lex_code == 7) //7 - while
            {
                res_stack.push_back(std::vector<uint>(stack.begin(), stack.end()));
                stack.push(20);
                state = 301;
                ++cur_lex;
            }
            else if (res_table[cur_lex].lex_code == 4) //4 - if
            {
                res_stack.push_back(std::vector<uint>(stack.begin(), stack.end()));
                stack.push(24);
                state = 301;
                ++cur_lex;
            }
            else
            {
                err_list << QString("%1: parser error: unkown operator").arg(res_table[cur_lex].line_num);
                exit = true;
            }
            break;
        case 12:
            if (res_table[cur_lex].lex_code == 20) //20 - =
            {
                res_stack.push_back(std::vector<uint>(stack.begin(), stack.end()));
                stack.push_back(13);
                state = 201;
                ++cur_lex;
            }
            else
            {
                err_list << QString("%1: parser error: excepted \'=\' after identifier").arg(res_table[cur_lex].line_num);
                exit = true;
            }
            break;
        case 13:
            if (res_table[cur_lex].lex_code == 14) //14 - ;
            {
                res_stack.push_back(std::vector<uint>(stack.begin(), stack.end()));
                state = stack.pop();
                ++cur_lex;
            }
            else
            {
                err_list << QString("%1: parser error: excepted \';\' after operator").arg(res_table[cur_lex].line_num);
                exit = true;
            }
            break;
        case 14:
            if (res_table[cur_lex].lex_code == 27) //27 - (
            {
                res_stack.push_back(std::vector<uint>(stack.begin(), stack.end()));
                state = 15;
                ++cur_lex;
            }
            else
            {
                err_list << QString("%1: parser error: excepted \'(\' in operator \'read\'").arg(res_table[cur_lex].line_num);
                exit = true;
            }
            break;
        case 15:
            if (res_table[cur_lex].lex_code == 31) // 31 - id
            {
                res_stack.push_back(std::vector<uint>(stack.begin(), stack.end()));
                state = 16;
                ++cur_lex;
            }
            else
            {
                err_list << QString("%1: parser error: excepted identifier").arg(res_table[cur_lex].line_num);
                exit = true;
            }
            break;
        case 16:
            if (res_table[cur_lex].lex_code == 28) // 28 - )
            {
                res_stack.push_back(std::vector<uint>(stack.begin(), stack.end()));
                state = 13;
                ++cur_lex;
            }
            else if (res_table[cur_lex].lex_code == 15) //15 - ,
            {
                res_stack.push_back(std::vector<uint>(stack.begin(), stack.end()));
                state = 15;
                ++cur_lex;
            }
            else
            {
                err_list << QString("%1: parser error: excepted \')\'' in operator \'read\'").arg(res_table[cur_lex].line_num);
                exit = true;
            }
            break;
        case 17:
            if (res_table[cur_lex].lex_code == 27) //27 - (
            {
                res_stack.push_back(std::vector<uint>(stack.begin(), stack.end()));
                state = 18;
                ++cur_lex;
            }
            else
            {
                err_list << QString("%1: parser error: excepted \'(\' in operator \'write\'").arg(res_table[cur_lex].line_num);
                exit = true;
            }
            break;
        case 18:
            if (res_table[cur_lex].lex_code == 31 || res_table[cur_lex].lex_code == 32 || res_table[cur_lex].lex_code == 33) // 31 - id
            {
                res_stack.push_back(std::vector<uint>(stack.begin(), stack.end()));
                state = 19;
                ++cur_lex;
            }
            else
            {
                err_list << QString("%1: parser error: excepted (identifier | numeric constant | string constant)").arg(res_table[cur_lex].line_num);
                exit = true;
            }
            break;
        case 19:
            if (res_table[cur_lex].lex_code == 28) // 28 - )
            {
                res_stack.push_back(std::vector<uint>(stack.begin(), stack.end()));
                state = 13;
                ++cur_lex;
            }
            else if (res_table[cur_lex].lex_code == 15) //15 - ,
            {
                res_stack.push_back(std::vector<uint>(stack.begin(), stack.end()));
                state = 18;
                ++cur_lex;
            }
            else
            {
                err_list << QString("%1: parser error: excepted \')\'' in operator \'write\'").arg(res_table[cur_lex].line_num);
                exit = true;
            }
            break;
        case 20:
            if (res_table[cur_lex].lex_code == 8) //8 - do
            {
                res_stack.push_back(std::vector<uint>(stack.begin(), stack.end()));
                state = 21;
                ++cur_lex;
            }
            else
            {
                err_list << QString("%1: parser error: excepted \'do\' in operator \'while\'").arg(res_table[cur_lex].line_num);
                exit = true;
            }
            break;
        case 21:
            state = 11;
            if (res_table[cur_lex].lex_code == 12) //12 - {
            {
                res_stack.push_back(std::vector<uint>(stack.begin(), stack.end()));
                stack.push(22);
                ++cur_lex;
            }
            else
            {
                stack.push(23);
            }
            break;
        case 22:
            if (res_table[cur_lex].lex_code == 13) //13 - }
            {
                res_stack.push_back(std::vector<uint>(stack.begin(), stack.end()));
                state = 23;
                ++cur_lex;
            }
            else
            {
                stack.push(22);
                state = 11;
            }
            break;
        case 23:
            state = stack.pop();
            break;
        case 24:
            if (res_table[cur_lex].lex_code == 5) //5 - then
            {
                res_stack.push_back(std::vector<uint>(stack.begin(), stack.end()));
                state = 25;
                ++cur_lex;
            }
            else
            {
                err_list << QString("%1: parser error: excepted \'then\' in operator \'if\'").arg(res_table[cur_lex].line_num);
                exit = true;
            }
            break;
        case 25:
            state = 11;
            if (res_table[cur_lex].lex_code == 12) //12 - {
            {
                res_stack.push_back(std::vector<uint>(stack.begin(), stack.end()));
                stack.push(26);
                ++cur_lex;
            }
            else
            {
                stack.push(27);
            }
            break;
        case 26:
            if (res_table[cur_lex].lex_code == 13) //13 - }
            {
                res_stack.push_back(std::vector<uint>(stack.begin(), stack.end()));
                state = 27;
                ++cur_lex;
            }
            else
            {
                stack.push(26);
                state = 11;
            }
            break;
        case 27:
            if (res_table[cur_lex].lex_code == 6) //6 - else
            {
                res_stack.push_back(std::vector<uint>(stack.begin(), stack.end()));
                state = 28;
                ++cur_lex;
            }
            else
            {
                err_list << QString("%1: parser error: excepted \'else\' in operator \'if\'").arg(res_table[cur_lex].line_num);
                exit = true;
            }
            break;
        case 28:
            state = 11;
            if (res_table[cur_lex].lex_code == 12) //12 - {
            {
                res_stack.push_back(std::vector<uint>(stack.begin(), stack.end()));
                stack.push(29);
                ++cur_lex;
            }
            else
            {
                stack.push(23);
            }
            break;
        case 29:
            if (res_table[cur_lex].lex_code == 13) //13 - }
            {
                res_stack.push_back(std::vector<uint>(stack.begin(), stack.end()));
                state = 23;
                ++cur_lex;
            }
            else
            {
                stack.push(29);
                state = 11;
            }
            break;
        case 201:
            if (res_table[cur_lex].lex_code == 16 || res_table[cur_lex].lex_code == 17) //16 - + 17 - -
            {
                res_stack.push_back(std::vector<uint>(stack.begin(), stack.end()));
                ++cur_lex;
            }
            state = 202;
            break;
        case 202:
            if (res_table[cur_lex].lex_code == 31 || res_table[cur_lex].lex_code == 32) //31 - id 32 - numeric
            {
                res_stack.push_back(std::vector<uint>(stack.begin(), stack.end()));
                state = 203;
                ++cur_lex;
            }
            else if (res_table[cur_lex].lex_code == 27) //27 - (
            {
                res_stack.push_back(std::vector<uint>(stack.begin(), stack.end()));
                stack.push(204);
                state = 201;
                ++cur_lex;
            }
            else
            {
                err_list << QString("%1: parser error: invalid arithmetic expression").arg(res_table[cur_lex].line_num);
                exit = true;
            }
            break;
        case 203:
            if (res_table[cur_lex].lex_code >= 16 && res_table[cur_lex].lex_code <= 19)
            {
                res_stack.push_back(std::vector<uint>(stack.begin(), stack.end()));
                state = 202;
                ++cur_lex;
            }
            else
            {
                state = stack.pop();
            }
            break;
        case 204:
            if (res_table[cur_lex].lex_code == 28) //28 - )
            {
                res_stack.push_back(std::vector<uint>(stack.begin(), stack.end()));
                state = 203;
                ++cur_lex;
            }
            else
            {
                err_list << QString("%1: parser error: missing \')\'").arg(res_table[cur_lex].line_num);
                exit = true;
            }
            break;
        case 301:
            if (res_table[cur_lex].lex_code == 11) //11 - !
            {
                res_stack.push_back(std::vector<uint>(stack.begin(), stack.end()));
                state = 301;
                ++cur_lex;
            }
            else if (res_table[cur_lex].lex_code == 29) // 29 - [
            {
                res_stack.push_back(std::vector<uint>(stack.begin(), stack.end()));
                stack.push(304);
                state = 301;
                ++cur_lex;
            }
            else
            {
                stack.push(302);
                state = 201;
            }
            break;
        case 302:
            if (res_table[cur_lex].lex_code >= 21 && res_table[cur_lex].lex_code <= 26)
            {
                res_stack.push_back(std::vector<uint>(stack.begin(), stack.end()));
                stack.push(303);
                state = 201;
                ++cur_lex;
            }
            else
            {
                err_list << QString("%1: parser error: invalid logic expression").arg(res_table[cur_lex].line_num);
                exit = true;
            }
            break;
        case 303:
            if (res_table[cur_lex].lex_code == 9 || res_table[cur_lex].lex_code == 10)
            {
                res_stack.push_back(std::vector<uint>(stack.begin(), stack.end()));
                state = 301;
                ++cur_lex;
            }
            else
            {
                state = stack.pop();
            }
            break;
        case 304:
            if (res_table[cur_lex].lex_code == 30) //30 - ]
            {
                res_stack.push_back(std::vector<uint>(stack.begin(), stack.end()));
                state = 303;
                ++cur_lex;
            }
            else
            {
                err_list << QString("%1: parser error: missing \']\'").arg(res_table[cur_lex].line_num);
                exit = true;
            }
            break;
        }
    }

    if (cur_lex < res_table.size() - 1)
        err_list << QString("%1: parser error: end of program found before end of file: \'{\' excepted").arg(res_table[cur_lex - 1].line_num);
    else if (cur_lex > res_table.size() - 1)
        err_list << QString("%1: parser error: \'}\' excepted").arg(res_table[cur_lex - 1].line_num);

    if (err_list.isEmpty() == false)
        emit this->error(err_list);
    else
        emit this->noErrors();

    return err_list.isEmpty();
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

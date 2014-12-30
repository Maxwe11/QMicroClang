#include "scanner.h"

#include "compiler.h"

#include <iterator>
#include <QtCore\QDebug>
#include <QtCore\QStringList>

QMicroClangScanner::QMicroClangScanner(QMicroClangCompiler* parent) : p(parent)
{
}

QMicroClangScanner::~QMicroClangScanner()
{
}

bool QMicroClangScanner::scan(QStringList code)
{
    bool declr_sect(true);
    uint line_number(0), count_id(0), count_str(0), count_const(0);
    value_table::const_iterator vt_it;
    QString token;

    for (
            QStringList::iterator it = code.begin(), end = code.end();
            it != end && p->err_list.empty();
            ++it
        )
    {
        ++line_number;

        while ( it->isEmpty() == false && p->err_list.isEmpty() == true )
        {
            token.clear();
            lex::type lex_type;
            if (getToken(*it, token, lex_type, line_number) == true)
            {
                switch (lex_type)
                {
                case lex::T_ID:
                    vt_it = std::find(p->id_table.begin(), p->id_table.end(), token);
                    if (declr_sect == true && vt_it != p->id_table.end())
                    {
                        pushError(QString("%1: scan error: identifier is already existed: %2").arg(line_number).arg(token));
                    }
                    else if (declr_sect == true && vt_it == p->id_table.end())
                    {
                        p->res_table.push_back(lexeme(line_number, lex::T_ID, ++count_id));
                        p->id_table.push_back(token);
                    }
                    else if (declr_sect == false && vt_it == p->id_table.end())
                    {
                        pushError(QString("%1: scan error: undeclared identifier: %2").arg(line_number).arg(token));
                    }
                    else
                    {
                        p->res_table.push_back(lexeme(line_number, lex::T_ID, std::distance(p->id_table.cbegin(), vt_it) + 1));
                    }
                    break;
                case lex::T_NUMERIC:
                    p->res_table.push_back(lexeme(line_number, lex::T_NUMERIC, ++count_const));
                    p->num_table.push_back(token);
                    break;
                case lex::T_STRING:
                    p->res_table.push_back(lexeme(line_number, lex::T_STRING, ++count_str));
                    p->str_table.push_back(token);
                    break;
                default:
                    p->res_table.push_back(lexeme(line_number, lex_type));
                    if (lex_type == lex::T_BEGIN) //12 - left figure parent {
                        declr_sect = false;
                    break;
                } //end of switch
            } //end of if
        } //end of while
    } //end of for       

    return p->err_list.isEmpty();
}

bool QMicroClangScanner::getToken(QString& line, QString& token, lex::type& type, uint line_n)
{
    uint cur(0), len = static_cast<uint>(line.length());
    //Пробелы и табуляция
    if (isSpace(line[cur]) == true)
    {
        line.remove(0, 1);
        return false;
    }
    // * Ключевые слова и идентификаторы
    // * Примечание: проверка идентификатора на существование/повтор происходив в Scanner::scan
    else if (line[cur].isLetter())
    {
        token = line[cur++];

        while ( cur < len && ( line[cur].isLetter() || line[cur].isDigit() ) )
            token += line[cur++];

        keyword_table::const_iterator it = p->keyword.find(token);
        type = (it != p->keyword.end() ? it->second : lex::T_ID);
        line.remove(0, cur);
        return true;
    }

    else if (line[cur] == '.')
    {
        token = line[cur++];

        if ( cur < len && line[cur].isDigit() )
        {
            token += line[cur++];

            while ( cur < len && line[cur].isDigit() )
                token += line[cur++];

            if ( cur < len && line[cur] == 'e' )
                getExpNumPart(line, token, cur, line_n);

            if (p->err_list.empty() == true)
            {
                type = lex::T_NUMERIC;
                line.remove(0, cur);
                return true;
            }
        }

        pushError(QString("%1: scan error: invalid constant value: %2").arg(line_n).arg(token));
        return false;
    }

    else if (line[cur].isDigit())
    {
        token = line[cur++];

        while ( cur < len && line[cur].isDigit() )
            token += line[cur++];

        if ( cur < len && line[cur] == '.' )
        {
            token += line[cur++];

            while ( cur < len && line[cur].isDigit() )
                token += line[cur++];
        }

        if ( cur < len && line[cur] == 'e' )
            getExpNumPart(line, token, cur, line_n);

        if (p->err_list.empty())
        {
            type = lex::T_NUMERIC;
            line.remove(0, cur);
            return true;
        }

         return false;
    }

    else if (isSingleDelimiter(line[cur]))
    {
        token = line[cur++];
        type = p->keyword.find(token)->second;
        line.remove(0, 1);
        return true;
    }

    else if (line[cur] == '|' || line[cur] == '&')
    {
        token = line[cur++];

        if (cur < len && line[cur] == line[cur - 1])
        {
            token += line[cur++];
            type = p->keyword.find(token)->second;
            line.remove(0, cur);
            return true;
        }

        pushError(QString("%1: scan error: unresolved external symbol: %2").arg(line_n).arg(token));
        return false;
    }

    else if (line[cur] == '!' || line[cur] == '=' || line[cur] == '>' || line[cur] == '<')
    {
        token = line[cur++];

        if (line[cur] == '=')
            token += line[cur++];

        type = p->keyword.find(token)->second;
        line.remove(0, cur);
        return true;
    }

    else if (line[cur++] == '\"')
    {
        token = "";
        bool err(true);

        while (cur < len)
        {
            if (line[cur] == '\"')
            {
                err = false;
                break;
            }
            token += line[cur++];
        }

         if (err == true)
         {
             pushError(QString("%1: scan error: unclosed string mark").arg(line_n));
             return false;
         }
         else
         {
             type = lex::T_STRING;
             line.remove(0, ++cur);
             return true;
         }
    }

    else
    {
        pushError(QString("%1: scan error: unresolved external symbol").arg(line_n));
        return false;
    } 
    return true;
}

void QMicroClangScanner::getExpNumPart(const QString &str, QString &token, uint &cur, uint line)
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
            pushError(QString("%1: scan error: invalid constant value: %2").arg(line).arg(token));
        }
    }
    else
    {
        pushError(QString("%1: scan error: invalid constant value: %2").arg(line).arg(token));
    } 
}

void QMicroClangScanner::pushError(const QString& err)
{
    p->err_list << err;
}

bool QMicroClangScanner::isSpace(QChar s)
{
    return (s == ' ' || s == '\t');
}
 
bool QMicroClangScanner::isSingleDelimiter(QChar s)
{
    return (s == '+' || s == '-' || s == '*' || s == '/' || s == ',' || s == ';' || s == '(' || s == ')' || s == '{' || s == '}' || s == '[' || s == ']');
}
#include <QDebug>
#define CHEAT 1
#include "qmicroclangparser.h"

int QMicroClangParser::matrix[54][54] = { { 0 } };

QMicroClangParser::QMicroClangParser(QObject* parent)
    : QObject(parent)
    , keywords(createKeywords())
    , rules(createRules())
    , map_(createWorkMap())
{
    for (int i = 0; i < 53; ++i)
    {
        matrix[i][53] = 3;
        matrix[53][i] = 1;
    }
//==
    matrix[1][3] = matrix[1][21] = matrix[2][22] = matrix[3][23] = matrix[4][30] = matrix[5][25] = matrix[6][7] =
    matrix[7][23] = matrix[8][16] = matrix[8][30] = matrix[9][33] = matrix[9][35] = matrix[9][41] = matrix[10][36] =
    matrix[11][37] = matrix[13][38] = matrix[16][8] = matrix[17][48] = matrix[17][49] = matrix[19][50] = matrix[19][51] =
    matrix[21][2] = matrix[24][4] = matrix[25][26] = matrix[26][27] = matrix[27][8] = matrix[28][29] = matrix[29][4] =
    matrix[29][8] = matrix[31][29] = matrix[32][9] = matrix[33][10] = matrix[34][9] = matrix[35][10] = matrix[36][10] = 
    matrix[37][12] = matrix[38][14] = matrix[39][9] = matrix[40][9] = matrix[48][18] = matrix[49][18] = matrix[50][20] = 
    matrix[51][20] = 2;

//<
    matrix[21][6] = matrix[21][7] = matrix[21][26] = matrix[21][28] = matrix[21][31] = matrix[21][32] = matrix[21][34] =
    matrix[24][5] = matrix[24][25] = matrix[27][17] = matrix[27][18] = matrix[27][19] = matrix[27][20] = matrix[27][26] =
    matrix[27][29] = matrix[27][48] = matrix[27][52] = matrix[29][5] = matrix[29][17] = matrix[29][18] =
    matrix[29][19] = matrix[29][20] = matrix[29][25] = matrix[29][26] = matrix[29][29] = matrix[29][48] = matrix[29][52] =
    matrix[32][8] = matrix[32][11] = matrix[32][12] = matrix[32][13] = matrix[32][14] = matrix[32][15] = matrix[32][17] =
    matrix[32][18] = matrix[32][19] = matrix[32][20] = matrix[32][26] = matrix[32][29] = matrix[32][39] = matrix[32][40] =
    matrix[32][48] = matrix[32][52] = matrix[33][7] = matrix[33][21] = matrix[33][26] = matrix[33][28] = matrix[33][31] =
    matrix[33][32] = matrix[33][34] = matrix[34][8] = matrix[34][11] = matrix[34][12] = matrix[34][13] = matrix[34][14] =
    matrix[34][15] = matrix[34][17] = matrix[34][18] = matrix[34][19] = matrix[34][20] = matrix[34][26] = matrix[34][29] =
    matrix[34][39] = matrix[34][40] = matrix[34][48] = matrix[34][52] = matrix[35][7] = matrix[35][21] = matrix[35][26] =
    matrix[35][28] = matrix[35][31] = matrix[35][32] = matrix[35][34] = matrix[36][7] = matrix[36][21] = matrix[36][26] =
    matrix[36][28] = matrix[36][31] = matrix[36][32] = matrix[36][34] = matrix[37][8] = matrix[37][13] = matrix[37][14] =
    matrix[37][15] = matrix[37][17] = matrix[37][18] = matrix[37][19] = matrix[37][20] = matrix[37][26] = matrix[37][29] =
    matrix[37][39] = matrix[37][40] = matrix[37][48] = matrix[37][52] = matrix[38][8] = matrix[38][15] = matrix[38][17] =
    matrix[38][18] = matrix[38][19] = matrix[38][20] = matrix[38][26] = matrix[38][29] = matrix[38][39] = matrix[38][40] =
    matrix[38][48] = matrix[38][52] = matrix[39][8] = matrix[39][11] = matrix[39][12] = matrix[39][13] = matrix[39][14] =
    matrix[39][15] = matrix[39][17] = matrix[39][18] = matrix[39][19] = matrix[39][20] = matrix[39][26] = matrix[39][29] =
    matrix[39][39] = matrix[39][40] = matrix[39][48] = matrix[39][52] = matrix[40][8] = matrix[40][11] = matrix[40][12] =
    matrix[40][13] = matrix[40][14] = matrix[40][15] = matrix[40][17] = matrix[40][18] = matrix[40][19] = matrix[40][20] =
    matrix[40][26] = matrix[40][29] = matrix[40][39] = matrix[40][40] = matrix[40][48] = matrix[40][52] = matrix[48][19] =
    matrix[49][19] = matrix[48][20] = matrix[49][20] = matrix[48][26] = matrix[48][29] = matrix[48][52] = matrix[49][26] =
    matrix[49][29] = matrix[49][52] = matrix[50][26] = matrix[50][29] = matrix[50][52] = matrix[51][26] = matrix[51][29] =
    matrix[51][52] = 1;
//>
    matrix[4][23] = matrix[5][23] = matrix[5][30] = matrix[6][22] = matrix[7][36] = matrix[8][23] = matrix[8][33] =
    matrix[8][35] = matrix[8][36] = matrix[8][37] = matrix[8][38] = matrix[8][41] = matrix[9][37] = matrix[9][38] =
    matrix[10][23] = matrix[11][33] = matrix[11][35] = matrix[11][38] = matrix[11][41] = matrix[12][33] = matrix[12][35] =
    matrix[12][37] = matrix[12][38] = matrix[12][41] = matrix[13][33] = matrix[13][35] = matrix[13][37] = matrix[13][41] =
    matrix[14][33] = matrix[14][35] = matrix[14][37] = matrix[14][38] = matrix[14][41] = matrix[15][33] = matrix[15][35] =
    matrix[15][37] = matrix[15][38] = matrix[15][41] = matrix[17][23] = matrix[17][30] = matrix[17][33] = matrix[17][35] =
    matrix[17][36] = matrix[17][37] = matrix[17][38] = matrix[17][41] = matrix[17][42] = matrix[17][43] = matrix[17][44] =
    matrix[17][45] = matrix[17][46] = matrix[17][47] = matrix[18][23] = matrix[18][30] = matrix[18][33] = matrix[18][35] =
    matrix[18][36] = matrix[18][37] = matrix[18][38] = matrix[18][41] = matrix[18][42] = matrix[18][43] = matrix[18][44] =
    matrix[18][45] = matrix[18][46] = matrix[18][47] = matrix[18][48] = matrix[18][49] = matrix[19][23] = matrix[19][30] =
    matrix[19][33] = matrix[19][35] = matrix[19][36] = matrix[19][37] = matrix[19][38] = matrix[19][41] = matrix[19][42] =
    matrix[19][43] = matrix[19][44] = matrix[19][45] = matrix[19][46] = matrix[19][47] = matrix[19][48] = matrix[19][49] =
    matrix[20][23] = matrix[20][30] = matrix[20][33] = matrix[20][35] = matrix[20][36] = matrix[20][37] = matrix[20][38] =
    matrix[20][41] = matrix[20][42] = matrix[20][43] = matrix[20][44] = matrix[20][45] = matrix[20][46] = matrix[20][47] =
    matrix[20][48] = matrix[20][49] = matrix[20][50] = matrix[20][51] = matrix[22][23] = matrix[22][36] = matrix[23][21] =
    matrix[23][22] = matrix[23][24] = matrix[23][26] = matrix[23][28] = matrix[23][31] = matrix[23][32] = matrix[23][34] =
    matrix[26][23] = matrix[26][25] = matrix[26][30] = matrix[26][33] = matrix[26][35] = matrix[26][36] = matrix[26][37] =
    matrix[26][38] = matrix[26][41] = matrix[26][42] = matrix[26][43] = matrix[26][44] = matrix[26][45] = matrix[26][46] =
    matrix[26][47] = matrix[26][48] = matrix[26][49] = matrix[26][50] = matrix[26][51] = matrix[30][23] = matrix[30][30] =
    matrix[30][33] = matrix[30][35] = matrix[30][36] = matrix[30][37] = matrix[30][38] = matrix[30][41] = matrix[30][42] =
    matrix[30][43] = matrix[30][44] = matrix[30][45] = matrix[30][46] = matrix[30][47] = matrix[30][48] = matrix[30][49] =
    matrix[30][50] = matrix[30][51] = matrix[42][17] = matrix[42][18] = matrix[42][19] = matrix[42][20] = matrix[42][26] =
    matrix[42][29] = matrix[42][48] = matrix[42][52] = matrix[43][17] = matrix[43][18] = matrix[43][19] = matrix[43][20] =
    matrix[43][26] = matrix[43][29] = matrix[43][48] = matrix[43][52] = matrix[44][17] = matrix[44][18] = matrix[44][19] =
    matrix[44][20] = matrix[44][26] = matrix[44][29] = matrix[44][48] = matrix[44][52] = matrix[45][17] = matrix[45][18] =
    matrix[45][19] = matrix[45][20] = matrix[45][26] = matrix[45][29] = matrix[45][48] = matrix[45][52] = matrix[46][17] =
    matrix[46][18] = matrix[46][19] = matrix[46][20] = matrix[46][26] = matrix[46][29] = matrix[46][48] = matrix[46][52] =
    matrix[47][17] = matrix[47][18] = matrix[47][19] = matrix[47][20] = matrix[47][26] = matrix[47][29] = matrix[47][48] =
    matrix[47][52] = matrix[52][23] = matrix[52][30] = matrix[52][33] = matrix[52][35] = matrix[52][36] = matrix[52][37] =
    matrix[52][38] = matrix[52][41] = matrix[52][42] = matrix[52][43] = matrix[52][44] = matrix[52][45] = matrix[52][46] =
    matrix[52][47] = matrix[52][48] = matrix[52][49] = matrix[52][50] = matrix[52][51] = 3;

#if CHEAT
    matrix[1][24] = 1;
    matrix[6][26] = 1;
    matrix[6][28] = 1;
    matrix[6][31] = 1;
    matrix[6][32] = 1;
    matrix[6][34] = 1;

    matrix[8][42] = matrix[8][43] = matrix[8][44] = matrix[8][45] = matrix[8][46] = matrix[8][47] = 1;
    matrix[16][17] = matrix[16][18] = matrix[16][19] = matrix[16][20] = matrix[16][52] = matrix[16][26] = 1;
    matrix[10][22] = matrix[41][33] = matrix[41][35] = 3;
    matrix[41][37] = matrix[41][38] = 3;
//    matrix[42][8] = matrix[43][8] = matrix[44][8] = matrix[45][8] = matrix[46][8] = matrix[47][8] = 2;
#endif
}

bool QMicroClangParser::scan(QStringList code)
{
    bool declr_sect(true);
    uint line_number(0), count_id(0), count_const(0), lex_code(0), id_index(0);
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

bool QMicroClangParser::parse(const QStringList &list)
{
    result_.clear();
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

    bool res = this->parse();

    if (res == false)
        emit this->error(err_list);
    else
        emit this->noErrors();

    return res;
}

const std::map< QString, uint > QMicroClangParser::createKeywords()
{
    std::map< QString, uint > keywords;

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

    return keywords;
}

const std::vector< QMicroClangParser::rule > QMicroClangParser::createRules()
{
    std::vector< rule > rules;

    rules.push_back(rule());
    rules.back().left = "<program>";
    rules.back().right << "<decl_list>{<operator_list1>}";

    rules.push_back(rule());
    rules.back().left = "<decl_list>";
    rules.back().right << "<declaration>;" << "<decl_list><declaration>;";

    rules.push_back(rule());
    rules.back().left = "<declaration>";
    rules.back().right << "double<id_list1>";

    rules.push_back(rule());
    rules.back().left = "<id_list1>";
    rules.back().right << "<id_list>";

    rules.push_back(rule());
    rules.back().left = "<id_list>";
    rules.back().right << ",id" << "<id_list>,id";

    rules.push_back(rule());
    rules.back().left = "<operator_list1>";
    rules.back().right << "<operator_list>";

    rules.push_back(rule());
    rules.back().left = "<operator_list>";
    rules.back().right << "<operator>;" << "<operator_list><operator>;";

    rules.push_back(rule());
    rules.back().left = "<operator>";
    rules.back().right << "id=<expr1>" << "read(<id_list1>)" << "write(<id_list1>)"
                       << "while<logic_expr1>do<statement>"
                       << "if<logic_expr1>then<statement>else<statement>";

    rules.push_back(rule());
    rules.back().left = "<statement>";
    rules.back().right << "{<operator_list1>}";

    rules.push_back(rule());
    rules.back().left = "<logic_expr1>";
    rules.back().right << "<logic_expr>";

    rules.push_back(rule());
    rules.back().left = "<logic_expr>";
    rules.back().right << "<logic_term1>" << "<logic_expr>||<logic_term1>";

    rules.push_back(rule());
    rules.back().left = "<logic_term1>";
    rules.back().right << "<logic_term>";

    rules.push_back(rule());
    rules.back().left = "<logic_term>";
    rules.back().right << "<logic_factor>" << "<logic_term>&&<logic_factor>";

    rules.push_back(rule());
    rules.back().left = "<logic_factor>";
    rules.back().right << "<relation>" << "!<logic_expr1>" << "[<logic_expr1>]";

    rules.push_back(rule());
    rules.back().left = "<relation>";
    rules.back().right << "<expr1><relation_sign><expr1>";

    rules.push_back(rule());
    rules.back().left = "<relation_sign>";
    rules.back().right << ">" << "<" << ">=" << "<=" << "!=" << "==";

    rules.push_back(rule());
    rules.back().left = "<expr1>";
    rules.back().right << "<expr>";

    rules.push_back(rule());
    rules.back().left = "<expr>";
    rules.back().right << "<term1>" << "-<term1>" << "<expr>-<term1>" << "<expr>+<term1>";

    rules.push_back(rule());
    rules.back().left = "<term1>";
    rules.back().right << "<term>";

    rules.push_back(rule());
    rules.back().left = "<term>";
    rules.back().right << "<factor>" << "<term>*<factor>" << "<term>/<factor>";

    rules.push_back(rule());
    rules.back().left = "<factor>";
    rules.back().right << "id" << "numeric" << "(<expr1>)";

    return rules;
}

const std::map< QString, uint > QMicroClangParser::createWorkMap()
{
    std::map< QString, uint > m;

    m["<program>"]          = 0;
    m["<decl_list>"]        = 1;
    m["<operator_list1>"]   = 2;
    m["<declaration>"]      = 3;
    m["<id_list1>"]         = 4;
    m["<id_list>"]          = 5;
    m["<operator_list>"]    = 6;
    m["<operator>"]         = 7;
    m["<expr1>"]            = 8;
    m["<logic_expr1>"]      = 9;
    m["<statement>"]        = 10;
    m["<logic_expr>"]       = 11;
    m["<logic_term1>"]      = 12;
    m["<logic_term>"]       = 13;
    m["<logic_factor>"]     = 14;
    m["<relation>"]         = 15;
    m["<relation_sign>"]    = 16;
    m["<expr>"]             = 17;
    m["<term1>"]            = 18;
    m["<term>"]             = 19;
    m["<factor>"]           = 20;
    m["{"]                  = 21;
    m["}"]                  = 22;
    m[";"]                  = 23;
    m["double"]             = 24;
    m[","]                  = 25;
    m["id"]                 = 26;
    m["="]                  = 27;
    m["read"]               = 28;
    m["("]                  = 29;
    m[")"]                  = 30;
    m["write"]              = 31;
    m["while"]              = 32;
    m["do"]                 = 33;
    m["if"]                 = 34;
    m["then"]               = 35;
    m["else"]               = 36;
    m["||"]                 = 37;
    m["&&"]                 = 38;
    m["!"]                  = 39;
    m["["]                  = 40;
    m["]"]                  = 41;
    m[">"]                  = 42;
    m["<"]                  = 43;
    m[">="]                 = 44;
    m["<="]                 = 45;
    m["!="]                 = 46;
    m["=="]                 = 47;
    m["-"]                  = 48;
    m["+"]                  = 49;
    m["*"]                  = 50;
    m["/"]                  = 51;
    m["numeric"]            = 52;
    m["#"]                  = 53;

    return m;
}

QString copy_lexems(const std::vector<QMicroClangParser::Lex>& v)
{
    QString res;
    for (std::vector<QMicroClangParser::Lex>::const_iterator it = v.begin(), end = v.end(); it != end; ++it)
        res += it->type + " ";
    return res;
}

bool QMicroClangParser::parse()
{
    std::vector< Lex > lexemes, vec;

    for (std::vector< lexeme >::const_iterator it = res_table.begin(), end = res_table.end(); it != end; ++it)
    {
        switch (it->lex_type)
        {
        case L_KEYW:
            lexemes.push_back(Lex(it->lex, it->lex));
            break;
        case L_NUMERIC:
            lexemes.push_back(Lex(it->lex, "numeric"));
            break;
        case L_ID:
            lexemes.push_back(Lex(it->lex, "id"));
            break;
        }
    }
    vec.push_back(Lex("#", "#"));
    lexemes.push_back(Lex("#", "#"));

    while (lexemes.size() >= 1)
    {
        result_.push_back(result());
        result_.back().stack = copy_lexems(vec);
        result_.back().input = copy_lexems(lexemes);
        
        if (vec.size() == 2 && vec[1].type == "<program>" && lexemes.size() == 1)
            break;

        uint i = map_[vec.back().type];
        uint j = map_[lexemes.front().type];

        if (matrix[i][j] == 2 || matrix[i][j] == 1) //2(=) 1(<)
        {
            vec.push_back(lexemes.front());
            lexemes.erase(lexemes.begin());

            if (matrix[i][j] == 2)
                result_.back().relation = "=";
            else
                result_.back().relation = "<";
        }
        else if (matrix[i][j] == 3) //3(>)
        {
            result_.back().relation = ">";
            std::vector< Lex >::iterator it = base(vec);
            QString rule = make_rule(it, vec.end()), tmp;
            tmp = find_rule(rule);
            if (tmp.isEmpty())
            {
                err_list << "Syntax error: no such rule with right part: " << rule;
                return false;
            }
            it->type = tmp;
            vec.erase(it + 1, vec.end());
        }
        else
        {
            if (vec.back().type[0] == '<')
            {
                vec.back().type.remove('<');
                vec.back().type.remove('>');
            }
            if (lexemes.front().type[0] == '<')
            {
                lexemes.front().type.remove('<');
                lexemes.front().type.remove('>');
            }
            err_list << "Syntax error: no such rule between" << vec.back().type << "and" << lexemes.front().type;
            return false;
        }
    }

    return true;
}

std::vector< QMicroClangParser::Lex >::iterator QMicroClangParser::base(std::vector< Lex >& v)
{
    std::vector< Lex >::iterator end = (v.end() - 1), prend = (v.end() - 2);

    while (matrix[map_[prend->type]][map_[end->type]] != 1)
    {
        --prend;
        --end;
    }

    return end;
}

QString QMicroClangParser::make_rule(std::vector< Lex >::const_iterator a, std::vector< Lex >::const_iterator b)
{
    QString str;
    while (a != b)
    {
        str += a->type;
        ++a;
    }
    return str;
}

QString QMicroClangParser::find_rule(const QString& s)
{
    std::vector< rule >::const_iterator begin = rules.begin(), end = rules.end();

    while (begin != end)
    {
        if (begin->right.contains(s) == true)
            return begin->left;
        ++begin;
    }

    return "";
}

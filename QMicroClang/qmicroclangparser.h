#ifndef QMICROCLANGPARSER_H
#define QMICROCLANGPARSER_H

#include <map>
#include <vector>
#include <utility>
#include <QObject>
#include <QString>
#include <QStringList>

class QMicroClangParser : public QObject {
    Q_OBJECT
public:
    typedef std::pair< QString, uint > T_pair;
    typedef std::vector< T_pair >      T_vec_pair;
    typedef enum { L_KEYW = 1, L_ID = 2, L_NUMERIC = 3, L_STRING = 4 } lexeme_type;
public:
    struct lexeme {

        lexeme(uint line, const QString& str, lexeme_type type, uint code, uint index = 0)
            : line_num(line)
            , lex(str)
            , lex_type(type)
            , lex_code(code)
            , table_indx(index)
        { }

        uint        line_num;
        QString     lex;
        lexeme_type lex_type;
        uint        lex_code;
        uint        table_indx;
    };
public:
    std::vector< lexeme >          res_table;
    std::map< QString, uint >      keywords;
    QStringList                    err_list;
    T_vec_pair                     id_table;
    T_vec_pair                     str_table;
    T_vec_pair                     num_table;
public:
    explicit QMicroClangParser(QObject* parent = 0);
    ~QMicroClangParser() { }
    bool parse(const QStringList& list);
signals:
    void error(QStringList lst);
    void noErrors();
private:
//scanner
    bool scan(QStringList list);
    bool getToken(QString& str, QString& token, lexeme_type& lex, uint line);
    void getExpNumPart(const QString& str, QString& token, uint& cur_pos, uint line);
    bool isSpace(QChar s)
    {
        return (s == ' ' || s == '\t');
    }
    bool isSingleDelimiter(QChar s)
    {
        return (s == '+' || s == '-' || s == '*' || s == '/' || s == ',' || s == ';' || s == '(' || s == ')' || s == '{' || s == '}' || s == '[' || s == ']');
    }
    uint findID(const QString& ID)
    {
        for (
                T_vec_pair::const_iterator it = id_table.begin(), end = id_table.end();
                it != end;
                ++it
            )
        {
            if (it->first == ID)
                return it->second;
        }
        return 0;
    }
//parser
    bool declarationList(uint& cur_lex);
    bool declaration(uint& cur_lex);
    bool idList(uint& cur_lex);
    bool operatorsList(uint& cur_lex);
    bool operator_(uint& cur_lex);
    bool outputList(uint& cur_lex);
    bool statement(uint& cur_lex);
    bool expression(uint& cur_lex);
    bool term(uint& cur_lex);
    bool factor(uint& cur_lex);
    bool booleanExpression(uint& cur_lex);
    bool booleanTerm(uint& cur_lex);
    bool booleanFactor(uint& cur_lex);
};

#endif // QMICROCLANGPARSER_H

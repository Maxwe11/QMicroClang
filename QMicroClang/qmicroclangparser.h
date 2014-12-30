#ifndef QMICROCLANGPARSER_H
#define QMICROCLANGPARSER_H

#include <map>
#include <vector>
#include <utility>
#include <QtCore\QObject>
#include <QtCore\QString>
#include <QtCore\QStringList>

class QMicroClangParser : public QObject {
    Q_OBJECT
public:
    typedef std::pair< QString, uint > T_pair;
    typedef std::vector< T_pair >      T_vec_pair;
    typedef enum { L_KEYW = 1, L_ID = 2, L_NUMERIC = 3 } lexeme_type;
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

    struct result {
        QString stack;
        QString relation;
        QString input;
    };

    struct rule {
        QString     left;
        QStringList right;
    };

    struct Lex {
        Lex(const QString& l, const QString& t) : lex_(l), type(t) {}
        QString lex_;
        QString type;
    };

public:
    std::vector< result >          result_;
    std::vector< lexeme >          res_table;
    QStringList                    err_list;
    T_vec_pair                     id_table;
    T_vec_pair                     str_table;
    T_vec_pair                     num_table;
private:
    const std::map< QString, uint >      keywords;
    const std::vector< rule >            rules;
    std::map< QString, uint >      map_;
    static int matrix[54][54];
public:
    explicit QMicroClangParser(QObject* parent = 0);
    ~QMicroClangParser() { }
    bool parse(const QStringList& list);
private:
    const std::map< QString, uint > createKeywords();
    const std::map< QString, uint > createWorkMap();
    const std::vector< rule >       createRules();
signals:
    void error(QStringList lst);
    void noErrors();
private:
    std::vector< Lex >::iterator base(std::vector< Lex >&);
    QString make_rule(std::vector< Lex >::const_iterator a, std::vector< Lex >::const_iterator b);
    QString find_rule(const QString& s);
    bool parse();
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
};

#endif // QMICROCLANGPARSER_H

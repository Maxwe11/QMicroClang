#ifndef COMPILER_H
#define COMPILER_H

#include <QtCore\QStack>
#include <QtCore\QObject>
#include <QtCore\QStringList>

#include "utility.h"
#include "polish_elemnts.h"

class QMicroClangScanner;
class QMicroClangParser;
class QMicroClangCodeGenerator;

class QMicroClangCompiler : public QObject {
    Q_OBJECT
public:
    lexeme_list                     res_table;
    QStringList                     err_list;
    QStringList                     variables;
    value_table                     id_table;
    value_table                     str_table;
    value_table                     num_table;
    std::vector<std::vector<uint> > res_stack;
    std::vector<polish_element>     polish_vec;
    QString                         polish_str;
    const keyword_table      keyword;
public:
    explicit QMicroClangCompiler(QObject* parent = 0);
    ~QMicroClangCompiler();
    void clearAllTables();
    QString lexemeByType(lex::type type);
    bool run(const QStringList& list);
private:
    keyword_table createKeywordTable();
signals:
    void error(QStringList lst);
    void noErrors();
private:
    QMicroClangScanner*       scanner;
    QMicroClangParser*        parser;
    QMicroClangCodeGenerator* generator;
};

#endif // COMPILER_H

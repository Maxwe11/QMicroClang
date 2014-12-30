#ifndef SCANNER_H
#define SCANNER_H

#include "utility.h"

class QMicroClangCompiler;

class QMicroClangScanner {
public:
    QMicroClangScanner(QMicroClangCompiler*);
    ~QMicroClangScanner();
    bool scan(QStringList);
private:
    bool getToken(QString& line, QString& token, lex::type& type, uint line_n);
    void getExpNumPart(const QString& str, QString& token, uint& cur_pos, uint line);
    bool isSpace(QChar);
    bool isSingleDelimiter(QChar);
    void pushError(const QString&);
private:
    QMicroClangCompiler* p;
};

#endif //SCANNER_H

#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H

#include "utility.h"
#include "polish_elemnts.h"

class QMicroClangCompiler;

class QMicroClangCodeGenerator {
public:
    QMicroClangCodeGenerator(QMicroClangCompiler*);
    ~QMicroClangCodeGenerator();

    void generate();
private:
    priority_table createPriorityTable();
    void operator_(lexeme_list::iterator&);
private:
    QMicroClangCompiler* p;
    priority_table       priority;
};

#endif //CODEGENERATOR_H

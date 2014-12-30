#ifndef QMICROCLANGTABLES_H
#define QMICROCLANGTABLES_H

#include <QWidget>

#include "compiler.h"

namespace Ui {
    class QMicroClangTables;
}

class QMicroClangTables : public QWidget
{
    Q_OBJECT

public:
    explicit QMicroClangTables(QMicroClangCompiler* p, QWidget *parent = 0);
    ~QMicroClangTables();

private slots:
    void on_stackButton_clicked();

    void on_prev_clicked();

private:
    Ui::QMicroClangTables *ui;
    QMicroClangCompiler* parser;
    uint counter;
};

#endif // QMICROCLANGTABLES_H

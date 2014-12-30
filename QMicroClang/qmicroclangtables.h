#ifndef QMICROCLANGTABLES_H
#define QMICROCLANGTABLES_H

#include <QWidget>

#include "qmicroclangparser.h"

namespace Ui {
    class QMicroClangTables;
}

class QMicroClangTables : public QWidget
{
    Q_OBJECT

public:
    explicit QMicroClangTables(QMicroClangParser* p, QWidget *parent = 0);
    ~QMicroClangTables();

private slots:
    void on_stackButton_clicked();

    void on_prev_clicked();

private:
    Ui::QMicroClangTables *ui;
    QMicroClangParser* parser;
    uint counter;
};

#endif // QMICROCLANGTABLES_H

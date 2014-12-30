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

private:
    Ui::QMicroClangTables *ui;
};

#endif // QMICROCLANGTABLES_H

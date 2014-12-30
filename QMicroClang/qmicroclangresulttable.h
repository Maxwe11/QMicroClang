#ifndef QMICROCLANGRESULTTABLE_H
#define QMICROCLANGRESULTTABLE_H

#include <QWidget>

#include "qmicroclangparser.h"

namespace Ui {
class QMicroClangResultTable;
}

class QMicroClangResultTable : public QWidget
{
    Q_OBJECT
    
public:
    explicit QMicroClangResultTable(QMicroClangParser* p, QWidget *parent = 0);
    ~QMicroClangResultTable();

protected:
    void resizeEvent(QResizeEvent *);

private:
    Ui::QMicroClangResultTable *ui;
};

#endif // QMICROCLANGRESULTTABLE_H

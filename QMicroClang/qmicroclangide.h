#ifndef QMICROCLANGIDE_H
#define QMICROCLANGIDE_H

#include <QMainWindow>

#include "compiler.h"

namespace Ui {
    class QMicroClangIDE;
}

class QMicroClangIDE : public QMainWindow {
    Q_OBJECT

public:
    explicit QMicroClangIDE(QWidget *parent = 0);
    ~QMicroClangIDE();

private slots:
    void openFile();
    void showTables();
    void analyze();
    void execute();
    void catchedError(QStringList);
    void noErrors();

private:
    Ui::QMicroClangIDE*     ui;
    QMicroClangCompiler*    parser;
    QString                 filename;
};

#endif // QMICROCLANGIDE_H

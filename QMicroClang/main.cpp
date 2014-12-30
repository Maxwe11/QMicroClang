#include <QtGui/QApplication>

#include "qmicroclangide.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QMicroClangIDE wgt;
    wgt.show();
    return app.exec();
}

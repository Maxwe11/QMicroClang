#include <QtGui/QApplication>
#include <QtCore/QTextCodec>

#include "qmicroclangide.h"

int main(int argc, char *argv[])
{
    QTextCodec *utfcodec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(utfcodec);
    QTextCodec::setCodecForCStrings(utfcodec);
    
    QApplication app(argc, argv);
    QMicroClangIDE wgt;
    wgt.show();
    return app.exec();
}

/********************************************************************************
** Form generated from reading UI file 'qmicroclangide.ui'
**
** Created: Wed 25. Jan 21:42:31 2012
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QMICROCLANGIDE_H
#define UI_QMICROCLANGIDE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCommandLinkButton>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>
#include "codeeditor.h"

QT_BEGIN_NAMESPACE

class Ui_QMicroClangIDE
{
public:
    QAction *actionOpen;
    QAction *actionExit;
    QAction *actionShow_tables;
    QWidget *centralWidget;
    QTextEdit *errorsLog;
    QCommandLinkButton *AnalyzeButton;
    QFrame *line;
    CodeEditor *codeEditor;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuProject;

    void setupUi(QMainWindow *QMicroClangIDE)
    {
        if (QMicroClangIDE->objectName().isEmpty())
            QMicroClangIDE->setObjectName(QString::fromUtf8("QMicroClangIDE"));
        QMicroClangIDE->resize(560, 540);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QMicroClangIDE->sizePolicy().hasHeightForWidth());
        QMicroClangIDE->setSizePolicy(sizePolicy);
        actionOpen = new QAction(QMicroClangIDE);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionExit = new QAction(QMicroClangIDE);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionShow_tables = new QAction(QMicroClangIDE);
        actionShow_tables->setObjectName(QString::fromUtf8("actionShow_tables"));
        centralWidget = new QWidget(QMicroClangIDE);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        errorsLog = new QTextEdit(centralWidget);
        errorsLog->setObjectName(QString::fromUtf8("errorsLog"));
        errorsLog->setEnabled(false);
        errorsLog->setGeometry(QRect(10, 340, 541, 121));
        AnalyzeButton = new QCommandLinkButton(centralWidget);
        AnalyzeButton->setObjectName(QString::fromUtf8("AnalyzeButton"));
        AnalyzeButton->setGeometry(QRect(10, 470, 111, 41));
        AnalyzeButton->setLayoutDirection(Qt::LeftToRight);
        line = new QFrame(centralWidget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(10, 320, 541, 16));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        codeEditor = new CodeEditor(centralWidget);
        codeEditor->setObjectName(QString::fromUtf8("codeEditor"));
        codeEditor->setGeometry(QRect(10, 10, 541, 311));
        QMicroClangIDE->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QMicroClangIDE);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 560, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuProject = new QMenu(menuBar);
        menuProject->setObjectName(QString::fromUtf8("menuProject"));
        QMicroClangIDE->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuProject->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuProject->addAction(actionShow_tables);

        retranslateUi(QMicroClangIDE);
        QObject::connect(actionExit, SIGNAL(triggered()), QMicroClangIDE, SLOT(close()));
        QObject::connect(actionOpen, SIGNAL(triggered()), QMicroClangIDE, SLOT(openFile()));
        QObject::connect(actionShow_tables, SIGNAL(triggered()), QMicroClangIDE, SLOT(showTables()));
        QObject::connect(AnalyzeButton, SIGNAL(clicked()), QMicroClangIDE, SLOT(analyze()));

        QMetaObject::connectSlotsByName(QMicroClangIDE);
    } // setupUi

    void retranslateUi(QMainWindow *QMicroClangIDE)
    {
        QMicroClangIDE->setWindowTitle(QApplication::translate("QMicroClangIDE", "QMicroClangIDE", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("QMicroClangIDE", "&Open", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("QMicroClangIDE", "&Exit", 0, QApplication::UnicodeUTF8));
        actionShow_tables->setText(QApplication::translate("QMicroClangIDE", "&Show tables", 0, QApplication::UnicodeUTF8));
        AnalyzeButton->setText(QApplication::translate("QMicroClangIDE", "Analyze", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("QMicroClangIDE", "&File", 0, QApplication::UnicodeUTF8));
        menuProject->setTitle(QApplication::translate("QMicroClangIDE", "&Project", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QMicroClangIDE: public Ui_QMicroClangIDE {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QMICROCLANGIDE_H

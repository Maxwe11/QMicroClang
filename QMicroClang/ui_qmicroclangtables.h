/********************************************************************************
** Form generated from reading UI file 'qmicroclangtables.ui'
**
** Created: Tue 8. Nov 08:22:07 2011
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QMICROCLANGTABLES_H
#define UI_QMICROCLANGTABLES_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QMicroClangTables
{
public:
    QLabel *resultTableLabel;
    QTableWidget *resultTable;
    QTableWidget *strTable;
    QTableWidget *numTable;
    QTableWidget *idTable;
    QLabel *idTableLabel;
    QLabel *numericTableLabel;
    QLabel *strTableLabel;

    void setupUi(QWidget *QMicroClangTables)
    {
        if (QMicroClangTables->objectName().isEmpty())
            QMicroClangTables->setObjectName(QString::fromUtf8("QMicroClangTables"));
        QMicroClangTables->setWindowModality(Qt::NonModal);
        QMicroClangTables->resize(740, 480);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QMicroClangTables->sizePolicy().hasHeightForWidth());
        QMicroClangTables->setSizePolicy(sizePolicy);
        resultTableLabel = new QLabel(QMicroClangTables);
        resultTableLabel->setObjectName(QString::fromUtf8("resultTableLabel"));
        resultTableLabel->setGeometry(QRect(10, 10, 71, 16));
        resultTable = new QTableWidget(QMicroClangTables);
        if (resultTable->columnCount() < 4)
            resultTable->setColumnCount(4);
        resultTable->setObjectName(QString::fromUtf8("resultTable"));
        resultTable->setGeometry(QRect(10, 30, 321, 441));
        resultTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        resultTable->setSelectionMode(QAbstractItemView::SingleSelection);
        resultTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        resultTable->setColumnCount(4);
        resultTable->horizontalHeader()->setDefaultSectionSize(69);
        strTable = new QTableWidget(QMicroClangTables);
        if (strTable->columnCount() < 1)
            strTable->setColumnCount(1);
        strTable->setObjectName(QString::fromUtf8("strTable"));
        strTable->setGeometry(QRect(600, 30, 131, 441));
        strTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        strTable->setSelectionMode(QAbstractItemView::SingleSelection);
        strTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        strTable->setColumnCount(1);
        strTable->horizontalHeader()->setDefaultSectionSize(120);
        numTable = new QTableWidget(QMicroClangTables);
        if (numTable->columnCount() < 1)
            numTable->setColumnCount(1);
        numTable->setObjectName(QString::fromUtf8("numTable"));
        numTable->setGeometry(QRect(470, 30, 121, 441));
        numTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        numTable->setSelectionMode(QAbstractItemView::SingleSelection);
        numTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        numTable->setColumnCount(1);
        numTable->horizontalHeader()->setDefaultSectionSize(100);
        idTable = new QTableWidget(QMicroClangTables);
        if (idTable->columnCount() < 1)
            idTable->setColumnCount(1);
        idTable->setObjectName(QString::fromUtf8("idTable"));
        idTable->setGeometry(QRect(340, 30, 121, 441));
        idTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        idTable->setSelectionMode(QAbstractItemView::SingleSelection);
        idTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        idTable->setColumnCount(1);
        idTable->horizontalHeader()->setDefaultSectionSize(100);
        idTableLabel = new QLabel(QMicroClangTables);
        idTableLabel->setObjectName(QString::fromUtf8("idTableLabel"));
        idTableLabel->setGeometry(QRect(340, 10, 81, 16));
        numericTableLabel = new QLabel(QMicroClangTables);
        numericTableLabel->setObjectName(QString::fromUtf8("numericTableLabel"));
        numericTableLabel->setGeometry(QRect(470, 10, 81, 16));
        strTableLabel = new QLabel(QMicroClangTables);
        strTableLabel->setObjectName(QString::fromUtf8("strTableLabel"));
        strTableLabel->setGeometry(QRect(600, 10, 81, 16));

        retranslateUi(QMicroClangTables);

        QMetaObject::connectSlotsByName(QMicroClangTables);
    } // setupUi

    void retranslateUi(QWidget *QMicroClangTables)
    {
        QMicroClangTables->setWindowTitle(QApplication::translate("QMicroClangTables", "Form", 0, QApplication::UnicodeUTF8));
        resultTableLabel->setText(QApplication::translate("QMicroClangTables", "Result table:", 0, QApplication::UnicodeUTF8));
        idTableLabel->setText(QApplication::translate("QMicroClangTables", "Identifiers table:", 0, QApplication::UnicodeUTF8));
        numericTableLabel->setText(QApplication::translate("QMicroClangTables", "Numerics table:", 0, QApplication::UnicodeUTF8));
        strTableLabel->setText(QApplication::translate("QMicroClangTables", "Strings table:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QMicroClangTables: public Ui_QMicroClangTables {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QMICROCLANGTABLES_H

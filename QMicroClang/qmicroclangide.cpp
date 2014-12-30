#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include "qmicroclangide.h"
#include "ui_qmicroclangide.h"

#include "qmicroclangtables.h"
#include "qmicroclangresulttable.h"

QMicroClangIDE::QMicroClangIDE(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::QMicroClangIDE)
    , parser(new QMicroClangParser(this))
{
    ui->setupUi(this);
    this->setFixedSize(560, 540);

    this->ui->codeEditor->setTabStopWidth(40);
    this->ui->codeEditor->setFont(QFont("Courier New", 12));

    QObject::connect(this->parser, SIGNAL(error(QStringList)), this, SLOT(catchedError(QStringList)));
    QObject::connect(this->parser, SIGNAL(noErrors()), this, SLOT(noErrors()));
}

QMicroClangIDE::~QMicroClangIDE()
{
    delete ui;
}

void QMicroClangIDE::openFile()
{
    this->filename = QFileDialog::getOpenFileName(this, "Open file", QDir::currentPath(), "(*.mc)");

    QFile file(filename);
    file.open(QFile::ReadOnly);
    this->ui->codeEditor->clear();
    this->ui->codeEditor->insertPlainText(file.readAll());
    file.close();

    this->setWindowTitle(QString("QMicroClangIDE - %1").arg(filename.split('/').last()));
    this->ui->errorsLog->clear();
}

void QMicroClangIDE::showTables()
{
    QMicroClangTables* tables = new QMicroClangTables(this->parser);
    tables->setAttribute(Qt::WA_DeleteOnClose, true);
    tables->show();
}

void QMicroClangIDE::showResultTable()
{
    QMicroClangResultTable* table = new QMicroClangResultTable(this->parser);
    table->setAttribute(Qt::WA_DeleteOnClose, true);
    table->show();
}

void QMicroClangIDE::analyze()
{
    QString code(ui->codeEditor->toPlainText());
    if (code.isEmpty() == false)
    {
        QStringList list(code.split('\n'));
        qDebug() << this->parser->parse(list);
    }
    else
    {
        QMessageBox::warning(this, "Error", "There is no code");
    }
}

void QMicroClangIDE::catchedError(QStringList lst)
{
    this->ui->errorsLog->clear();
    foreach(QString str, lst)
    {
        this->ui->errorsLog->append(QString("<font color=red>%1</font>\n").arg(str));
    }
}

void QMicroClangIDE::noErrors()
{
    this->ui->errorsLog->clear();
    this->ui->errorsLog->setText("<font color=green>No errors</font>");
}

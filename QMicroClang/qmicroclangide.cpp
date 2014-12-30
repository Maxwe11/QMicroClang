#include <QtCore/QFile>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtCore/QDebug>
#include <QtCore/QProcess>

#include "qmicroclangide.h"
#include "ui_qmicroclangide.h"

#include "qmicroclangtables.h"

QMicroClangIDE::QMicroClangIDE(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::QMicroClangIDE)
    , parser(new QMicroClangCompiler(this))
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
    if (parser->res_table.empty() == false)
    {
        QMicroClangTables* tables = new QMicroClangTables(this->parser);
        tables->setAttribute(Qt::WA_DeleteOnClose, true);
        tables->show();
    }
}

void QMicroClangIDE::analyze()
{
    QString code(ui->codeEditor->toPlainText());
    if (code.isEmpty() == false)
    {
        QStringList list(code.split('\n'));
        qDebug() << this->parser->run(list);
    }
    else
    {
        QMessageBox::warning(this, "Error", "There is no code");
    }
}

void QMicroClangIDE::execute()
{
    if (!parser->variables.empty())
    {
        QProcess proc;
        proc.startDetached("polish.exe", parser->variables);
    }
    else
    {
        QMessageBox::warning(this, "Error", "Nothing to execute");
    }
}

void QMicroClangIDE::catchedError(QStringList lst)
{
    this->ui->errorsLog->clear();
    foreach(QString str, lst)
    {
        this->ui->errorsLog->appendPlainText(QString("%1").arg(str));
    }
}

void QMicroClangIDE::noErrors()
{
    this->ui->errorsLog->clear();
    this->ui->errorsLog->appendPlainText(QString("No errors"));
    this->ui->errorsLog->appendPlainText(QString("\n%1").arg(parser->polish_str));
}

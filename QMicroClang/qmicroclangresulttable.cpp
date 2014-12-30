#include "qmicroclangresulttable.h"
#include "ui_qmicroclangresulttable.h"

QMicroClangResultTable::QMicroClangResultTable(QMicroClangParser* p, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QMicroClangResultTable)
{
    ui->setupUi(this);
    ui->tableWidget->setRowCount(static_cast<int>(p->result_.size()));

    for (int i = 0; i < ui->tableWidget->rowCount(); ++i)
    {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(p->result_[i].stack));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(p->result_[i].relation));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(p->result_[i].input));
    }
}

QMicroClangResultTable::~QMicroClangResultTable()
{
    delete ui;
}

void QMicroClangResultTable::resizeEvent(QResizeEvent *)
{
    this->ui->tableWidget->resize(this->width() - 20, this->height() - 20);
}

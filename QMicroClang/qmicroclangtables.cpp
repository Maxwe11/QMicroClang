#include <vector>

#include "qmicroclangtables.h"
#include "ui_qmicroclangtables.h"

QMicroClangTables::QMicroClangTables(QMicroClangParser* p, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QMicroClangTables)
    , parser(p)
    , counter()
{
    this->setFixedSize(884, 333);

    ui->setupUi(this);
    ui->resultTable->setRowCount(p->res_table.size());
    ui->idTable->setRowCount(p->id_table.size());
    ui->strTable->setRowCount(p->str_table.size());
    ui->numTable->setRowCount(p->num_table.size());

    QTableWidgetItem* item;

    for (uint i = 0, size = p->res_table.size(); i < size; ++i)
    {
        item = new QTableWidgetItem(QString("%1").arg(p->res_table[i].line_num));
        this->ui->resultTable->setItem(i, 0, item);

        item = new QTableWidgetItem(p->res_table[i].lex);
        this->ui->resultTable->setItem(i, 1, item);

        item = new QTableWidgetItem(QString("%1").arg(p->res_table[i].lex_code));
        this->ui->resultTable->setItem(i, 2, item);

        item = new QTableWidgetItem(QString("%1").arg(p->res_table[i].table_indx));
        this->ui->resultTable->setItem(i, 3, item);
    }

    uint i = 0;
    for (QMicroClangParser::T_vec_pair::const_iterator it = p->id_table.begin(); it != p->id_table.end(); ++it, ++i)
    {
        item = new QTableWidgetItem(it->first);
        this->ui->idTable->setItem(i, 0, item);
    }

    i = 0;
    for (QMicroClangParser::T_vec_pair::const_iterator it = p->num_table.begin(); it != p->num_table.end(); ++it, ++i)
    {
        item = new QTableWidgetItem(it->first);
        this->ui->numTable->setItem(i, 0, item);
    }

    i = 0;
    for (QMicroClangParser::T_vec_pair::const_iterator it = p->str_table.begin(); it != p->str_table.end(); ++it, ++i)
    {
        item = new QTableWidgetItem(it->first);
        this->ui->strTable->setItem(i, 0, item);
    }
}

QMicroClangTables::~QMicroClangTables()
{
    delete ui;
}

void QMicroClangTables::on_stackButton_clicked()
{
    this->ui->stackTable->clear();

    if (parser->res_stack.size() == counter)
        counter = 0;

    QTableWidgetItem* tmp;
    uint j = 0;
    this->ui->stackTable->setRowCount(parser->res_stack[counter].size());
    for (
         std::vector<uint>::iterator it = parser->res_stack[counter].begin(), end = parser->res_stack[counter].end();
         it != end;
         ++it, ++j)
    {
        tmp = new QTableWidgetItem(QString("%1").arg(parser->res_stack[counter][j]));
        this->ui->stackTable->setItem(j, 0, tmp);
    }
    this->ui->resultTable->setCurrentItem(ui->resultTable->item(counter, 0));
    ++counter;
}

void QMicroClangTables::on_prev_clicked()
{
    this->ui->stackTable->clear();

    if (0 == counter)
        counter = parser->res_stack.size();

    QTableWidgetItem* tmp;
    uint j = 0;
    this->ui->stackTable->setRowCount(parser->res_stack[counter].size());
    for (
         std::vector<uint>::iterator it = parser->res_stack[counter].begin(), end = parser->res_stack[counter].end();
         it != end;
         ++it, ++j)
    {
        tmp = new QTableWidgetItem(QString("%1").arg(parser->res_stack[counter][j]));
        this->ui->stackTable->setItem(j, 0, tmp);
    }
    this->ui->resultTable->setCurrentItem(ui->resultTable->item(counter, 0));
    --counter;
}

#include <vector>

#include "qmicroclangtables.h"
#include "ui_qmicroclangtables.h"

#include "utility.h"

QMicroClangTables::QMicroClangTables(QMicroClangCompiler* p, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QMicroClangTables)
    , parser(p)
    , counter()
{
    this->ui->setupUi(this);
    this->ui->resultTable->setRowCount(p->res_table.size());
    this->ui->idTable->setRowCount(p->id_table.size());
    this->ui->strTable->setRowCount(p->str_table.size());
    this->ui->numTable->setRowCount(p->num_table.size());

    this->ui->resultTable->setColumnWidth(0,  45);
    this->ui->resultTable->setColumnWidth(1, 152);
    this->ui->resultTable->setColumnWidth(2,  40);
    this->ui->resultTable->setColumnWidth(3,  50);

    this->ui->stackTable->setColumnWidth(0, 70);
    this->ui->idTable->setColumnWidth(0, 150);
    this->ui->numTable->setColumnWidth(0, 84);
    this->ui->strTable->setColumnWidth(0, 124);

    this->setFixedSize(this->size());
    
    QTableWidgetItem* item;
    
    for (uint i = 0, size = p->res_table.size(); i < size; ++i)
    {
        item = new QTableWidgetItem(QString("%1").arg(p->res_table[i].line_num));
        this->ui->resultTable->setItem(i, 0, item);

        if (p->res_table[i].type == lex::T_ID)
            item = new QTableWidgetItem(p->id_table[p->res_table[i].table_indx - 1]);
        else if (p->res_table[i].type == lex::T_NUMERIC)
            item = new QTableWidgetItem(p->num_table[p->res_table[i].table_indx - 1]);
        else if (p->res_table[i].type == lex::T_STRING)
            item = new QTableWidgetItem(p->str_table[p->res_table[i].table_indx - 1]);
        else
            item = new QTableWidgetItem(p->lexemeByType(p->res_table[i].type));

        this->ui->resultTable->setItem(i, 1, item);

        item = new QTableWidgetItem(QString("%1").arg(uint(p->res_table[i].type)));
        this->ui->resultTable->setItem(i, 2, item);

        item = new QTableWidgetItem(QString("%1").arg(p->res_table[i].table_indx));
        this->ui->resultTable->setItem(i, 3, item);
    }

    uint i = 0;
    for (value_table::const_iterator it = p->id_table.begin(); it != p->id_table.end(); ++it, ++i)
    {
        item = new QTableWidgetItem(*it);
        this->ui->idTable->setItem(i, 0, item);
    }

    i = 0;
    for (value_table::const_iterator it = p->num_table.begin(); it != p->num_table.end(); ++it, ++i)
    {
        item = new QTableWidgetItem(*it);
        this->ui->numTable->setItem(i, 0, item);
    }

    i = 0;
    for (value_table::const_iterator it = p->str_table.begin(); it != p->str_table.end(); ++it, ++i)
    {
        item = new QTableWidgetItem(*it);
        this->ui->strTable->setItem(i, 0, item);
    }

    on_stackButton_clicked();
}

QMicroClangTables::~QMicroClangTables()
{
    delete ui;
}

void QMicroClangTables::on_stackButton_clicked()
{
  
    for (int i = 0; i < ui->stackTable->rowCount(); ++i)
        this->ui->stackTable->removeRow(i);

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
/*
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
*/
}

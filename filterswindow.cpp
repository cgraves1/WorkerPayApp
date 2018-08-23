#include "filterswindow.h"
#include "ui_filterswindow.h"
#include "mainwindow.h"

FiltersWindow::FiltersWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FiltersWindow)
{
    ui->setupUi(this);
}

FiltersWindow::~FiltersWindow()
{
    delete ui;
}

void FiltersWindow::on_pushButton_2_clicked()
{
    this->hide();
}

void FiltersWindow::on_pushButton_clicked()
{
    QString invNum = ui->lineEdit->text();
    QString tmp = ui->comboBox->currentText();
}

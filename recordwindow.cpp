#include "recordwindow.h"
#include "ui_recordwindow.h"

recordWindow::recordWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::recordWindow)
{
    ui->setupUi(this);
}

recordWindow::~recordWindow()
{
    delete ui;
}

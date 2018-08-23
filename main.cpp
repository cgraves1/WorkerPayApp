#include "mainwindow.h"
#include "filterswindow.h"
#include <QApplication>
#include <QCheckBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

#ifndef FILTERSWINDOW_H
#define FILTERSWINDOW_H

#include <QDialog>

namespace Ui {
class FiltersWindow;
}

class FiltersWindow : public QDialog
{
    Q_OBJECT

public:
    explicit FiltersWindow(QWidget *parent = 0);
    ~FiltersWindow();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::FiltersWindow *ui;
};

#endif // FILTERSWINDOW_H

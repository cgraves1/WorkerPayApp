#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "employee.h"
#include "wage.h"
#include "part.h"
#include "invoice.h"
#include "deduction.h"
#include "employeerecord.h"
#include <QDate>
#include <QCheckBox>
#include <QVector>
#include <QDateTime>
#include <QMessageBox>
#include <QString>
#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <QListWidgetItem>
#include <QFileDialog>
#include <QStandardPaths>

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void updateMainTable();
    void updateEmployeeTable();
    void updateComboBoxes();
    void updateWages();
    void updateDeductions();
    void updateInvoiceEmployeeTable();
    void updatePartTable();
    void updateInvoiceTable();
    void updateRecordTable();
    void bubbleSort(QVector<employee> &);
    int employeeLocation(QString);
    void calculateAmountOwed();
    void syncPaid(invoice&, invoice&);
    void clearRecordInfo();
    void saveFile(QString);
    void loadFile(QString);
    void openLast();



private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_14_clicked();

    void on_comboBox_3_activated(const QString &arg1);

    void on_comboBox_activated(const QString &arg1);

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_3_clicked();

    void on_radioButton_4_clicked();

    void on_radioButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_11_clicked();

    void on_radioButton_6_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_20_clicked();

    void on_recordTable_itemDoubleClicked(QListWidgetItem *item);

    void on_listWidget_9_itemDoubleClicked(QListWidgetItem *item);

    void on_listWidget_6_itemDoubleClicked(QListWidgetItem *item);

    void on_pushButton_19_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_employeeListWidget_itemDoubleClicked(QListWidgetItem *item);

    void openInvoice(QString);

    void openEmployee(QString);

    void openRecordList(QString);

    void openSpecificRecord(QString);

    void openRecordInvoice(QString);

    void on_pushButton_22_clicked();

    void on_pushButton_21_clicked();

    void on_actionLoad_triggered();


    void on_pushButton_23_clicked();

    void on_actionSave_As_triggered();

    void on_actionBackup_triggered();

    void on_pushButton_26_clicked();

    void on_pushButton_27_clicked();

    void on_actionAbout_triggered();

    void on_pushButton_28_clicked();

    void on_pushButton_24_clicked();

    void on_pushButton_25_clicked();

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H

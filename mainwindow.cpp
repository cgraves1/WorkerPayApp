#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "filterswindow.h"


QVector<employee> employeeList;
QVector<wage> wageList;
QVector<deduction> deductionList;
QVector<invoice> invoiceList;
QVector<invoice> sortedInvoiceList;
QVector<employee> invoiceEmployeeList;
QVector<double> commissionList;
QVector<part> partList;
QVector<bool> paidList;
QVector<employeeRecord> recordList;


QListWidgetItem *selectedListItem;
QString saveFileName;
bool sorted = false;

struct filter{
    QString invoiceNumber;
    QString worker;
};

//main
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MainWindow::setWindowTitle("Worker Pay Program");
    QPixmap pixmap(":/new/prefix1/checkbox.png");
    QIcon ButtonIcon(pixmap);
    ui->pushButton->setIcon(ButtonIcon);
    ui->pushButton->setIconSize(pixmap.rect().size());

    //testing
    //updateMainTable();

    //size table columns
    ui->tableWidget->setColumnWidth(0,70);
    ui->tableWidget->setColumnWidth(1,170);
    ui->tableWidget->setColumnWidth(2,170);
    ui->tableWidget->setColumnWidth(3,100);
    ui->tableWidget->setColumnWidth(4,100);
    //attempt to open last openned file
    openLast();

}


//bubble sort employee list
void MainWindow::bubbleSort(QVector<employee> &eList)
{
    bool swap = true;

    for(int i = 0; swap; i++)
    {
        swap = false;
        for(int j = eList.size()-1; j > i; j--)
        {
            if(eList[j].getlName() < eList[j-1].getlName())
            {
                employee temp = eList[j];
                qSwap(eList[j], eList[j-1]);
                qSwap(eList[j-1], temp);
                swap = true;
            }
            else if (eList[j].getlName() == eList[j-1].getlName())
            {
                if(eList[j].getfName() < eList[j-1].getfName())
                {
                    employee temp = eList[j];
                    qSwap(eList[j], eList[j-1]);
                    qSwap(eList[j-1], temp);
                    swap = true;
                }
            }
        }
    }
}

//update combo boxes
void MainWindow::updateComboBoxes()
{
    int hiddenCount = 0;
    //clear boxes
    ui->comboBox->clear();
    ui->comboBox_2->clear();
    ui->comboBox_3->clear();
    ui->comboBox_4->clear();

    //update with new info
    for(int i = 0; i < employeeList.count(); i++)
    {
        if(employeeList[i].isHidden())
            continue;
        //check if hidden
        ui->comboBox->insertItem(i, employeeList[i].getlName() + ", " + employeeList[i].getfName());
        ui->comboBox_2->insertItem(i, employeeList[i].getlName() + ", " + employeeList[i].getfName());
        ui->comboBox_3->insertItem(i, employeeList[i].getlName() + ", " + employeeList[i].getfName());
        ui->comboBox_4->insertItem(i, employeeList[i].getlName() + ", " + employeeList[i].getfName());
    }

    //sets first selection blank
    ui->comboBox->addItem("");
    ui->comboBox_2->addItem("");
    ui->comboBox_3->addItem("");
    ui->comboBox_4->addItem("");

    //find number of hidden employees to get blank spot
    for(int i = 0; i < employeeList.count(); i++)
    {
        if(employeeList[i].isHidden())
            hiddenCount+=1;
    }

    //set current index
    ui->comboBox->setCurrentIndex(employeeList.count() - hiddenCount);
    ui->comboBox_2->setCurrentIndex(employeeList.count() - hiddenCount);
    ui->comboBox_3->setCurrentIndex(employeeList.count() - hiddenCount);
    ui->comboBox_4->setCurrentIndex(employeeList.count() - hiddenCount);
}

//update employee table
void MainWindow::updateEmployeeTable()
{
    //clear rows
    ui->employeeListWidget->clear();

    //update rows with new info
    for(int i = 0; i < employeeList.count(); i++)
    {
        if(!employeeList[i].isHidden())
            ui->employeeListWidget->addItem(employeeList[i].getlName() + ", " + employeeList[i].getfName());
    }
    saveFile(saveFileName);
}

//update record table
void MainWindow::updateRecordTable()
{
    ui->recordTable->clear();
    for(int i = 0; i < employeeList.count(); i++)
    {
        ui->recordTable->addItem(employeeList[i].getlName() + ", " + employeeList[i].getfName());
        if(employeeList[i].isHidden())
            ui->recordTable->item(i)->setForeground(Qt::red);
    }
    saveFile(saveFileName);
}

//update main table
void MainWindow::updateMainTable()
{
    int rowCount = 0;
    //clear rows
    ui->tableWidget->setRowCount(0);
    //update rows with new info
    for(int i = 0; i < employeeList.size(); i++)
    {
        //check if removed
        if(!employeeList[i].isHidden())
        {
            ui->tableWidget->insertRow(rowCount);
            //create checkbox
            QTableWidgetItem *checkBoxItem = new QTableWidgetItem();
            checkBoxItem->setCheckState(Qt::Unchecked);
            checkBoxItem->setTextAlignment(Qt::AlignCenter);

            //set values in row. Checkbox (centered), int, string, string, double
            ui->tableWidget->setItem(rowCount, 0, checkBoxItem);
            ui->tableWidget->setItem(rowCount, 1, new QTableWidgetItem(employeeList[i].getlName()));
            ui->tableWidget->setItem(rowCount, 2, new QTableWidgetItem(employeeList[i].getfName()));

            //create item for amt owed
            QTableWidgetItem *amtOwedItem = new QTableWidgetItem(QString::number(employeeList[i].getAmtOwed(), 'f', 2));
            ui->tableWidget->setItem(rowCount, 3, amtOwedItem);

            //create last paid date item
            ui->tableWidget->setItem(rowCount, 4, new QTableWidgetItem(employeeList[i].getLastPaid()));

            //make all items uneditable
            for(int j = 0; j < 5; j++)
                ui->tableWidget->item(rowCount,j)->setFlags( ui->tableWidget->item(rowCount,j)->flags() &  ~Qt::ItemIsEditable);
            rowCount++;
        }
    }
    updateRecordTable();
    saveFile(saveFileName);
}

//destructor
MainWindow::~MainWindow()
{
    delete ui;
}

//add employee
void MainWindow::on_pushButton_2_clicked()
{
    //validator and error message
    bool valid;
    bool duplicate = false;
    bool save = false;
    QMessageBox errmsg;
    errmsg.setWindowTitle("WorkerPayProgram");
    errmsg.setText("Invalid Input!");

    //employee values
    QString f, l;
    double w, c;

    //clear spaces from first and last name input
    f = ui->fNameLine->text().toUpper();
    l = ui->lNameLine->text().toUpper();
    f = f.simplified();
    l = l.simplified();
    f.replace( " ", "" );
    l.replace( " ", "" );

    //make sure name is valid
    if(f == "" || l == "")
    {
        errmsg.exec();
        valid = false;
    }

    //make sure other input is numeric
    else if(ui->wageLine->text() != "" && !ui->wageLine->text().toDouble() && ui->wageLine->text().toInt() != 0)
    {
        errmsg.exec();
        valid = false;
    }
    else if(ui->commLine->text() != "" && !ui->commLine->text().toDouble() && ui->commLine->text().toInt() != 0)
    {
        errmsg.exec();
        valid = false;
    }
    else
    {
        if(ui->wageLine->text() == "")
            w = 0.00;
        else
            w = ui->wageLine->text().toDouble();
        if(ui->commLine->text() == "")
            c = 0.00;
        else
            c = ui->commLine->text().toDouble();
        valid = true;
    }

    //check if duplicate
    if(valid)
    {
        for(int i = 0; i < employeeList.count(); i++)
        {
            if(f == employeeList[i].getfName() && l == employeeList[i].getlName())
            {
                QMessageBox::StandardButton hiddenmsg;
                if(employeeList[i].isHidden())
                {
                    hiddenmsg = QMessageBox::question(this, "Removed", "Employee has been removed previously. Would you like to add them again?",
                                                QMessageBox::Yes|QMessageBox::No);
                    if (hiddenmsg == QMessageBox::Yes)
                    {
                        employeeList[i].toggleHidden();
                        updateMainTable();
                        updateEmployeeTable();
                        updateComboBoxes();
                    }
                    return;
                }

                duplicate = true;
                QMessageBox::StandardButton dupmsg;
                dupmsg = QMessageBox::question(this, "Duplicate", "Employee already exists. Save Changes?",
                                            QMessageBox::Yes|QMessageBox::No);
                if (dupmsg == QMessageBox::Yes)
                    save = true;
                else
                    save = false;
            }
        }
    }

    //if valid create employee. if duplicate, save?
    if((valid && !duplicate) || (valid && duplicate && save))
    {
        //clear lineedits
        ui->fNameLine->setText("");
        ui->lNameLine->setText("");
        ui->wageLine->setText("");
        ui->commLine->setText("");

        //create employee
        employee tmp(f,l,w,c);

        //add to tables, if save then don't need to change
        if(!save)
            employeeList.append(tmp);

        //else update edited employee
        else
        {
            for(int i = 0; i < employeeList.size(); i ++)
            {
                if(employeeList[i].getfName() == f && employeeList[i].getlName() == l)
                {
                    employeeList[i].setWage(w);
                    employeeList[i].setComm(c);
                }
            }
        }
    }
    //update ui with new employee list info
    if(valid)
    {
        bubbleSort(employeeList);
        updateMainTable();
        updateEmployeeTable();
        updateComboBoxes();
    }
}

//clear employee info
void MainWindow::on_pushButton_5_clicked()
{
    ui->fNameLine->setText("");
    ui->lNameLine->setText("");
    ui->wageLine->setText("");
    ui->commLine->setText("");
}

//remove employee / hide employee
void MainWindow::on_pushButton_4_clicked()
{
    //tmp
    //return;

    bool found = false;
    for(int i = 0; i < employeeList.count(); i++)
    {
        //find selected employee
        if((employeeList[i].getlName() + ", " + employeeList[i].getfName())
                == ui->employeeListWidget->currentItem()->text())
            found = true;

        //remove from employee list
        if(found)
        {
            //permanently remove, not active yet

            if(employeeList[i].isHidden())
            {
                QMessageBox::StandardButton warningmsg;
                warningmsg = QMessageBox::question(this, "Warning", "Employee "
                    + employeeList[i].getlName() + ", " + employeeList[i].getfName() + " already hidden."
                    + "?\nDo you want to permanently remove employee, including records?",
                                            QMessageBox::Yes|QMessageBox::No);
                if (warningmsg == QMessageBox::No)
                    return;
                //remove employee and records permanently
                for(int j = 0; j < recordList.count(); j++)
                {
                    if(recordList[j].getEmployee() == employeeList[i])
                        recordList.removeAt(j);
                }
                employeeList.removeAt(i);
                break;
            }
            //if already hidden, undo?

            /*
            if(employeeList[i].isHidden())
            {
                QMessageBox::StandardButton warningmsg;
                warningmsg = QMessageBox::question(this, "Warning", "Employee "
                    + employeeList[i].getlName() + ", " + employeeList[i].getfName() + " already hidden."
                    + "?\nDo you want to undo this?",
                                            QMessageBox::Yes|QMessageBox::No);
                if (warningmsg == QMessageBox::No)
                    return;
                employeeList[i].toggleHidden();
                break;
            }*/

            QMessageBox::StandardButton warningmsg2;
            warningmsg2 = QMessageBox::question(this, "Warning", "Are you sure you want to remove employee "
                + employeeList[i].getlName() + ", " + employeeList[i].getfName() + "?\nThis will not remove"
                + " records for this employee.",
                                        QMessageBox::Yes|QMessageBox::No);
            if (warningmsg2 == QMessageBox::No)
                return;
            employeeList[i].toggleHidden();
            break;
        }
        found = false;
    }
    updateMainTable();
    updateEmployeeTable();
    updateComboBoxes();
}

//get employee location
int MainWindow::employeeLocation(QString s)
{
    for(int i = 0; i < employeeList.count(); i++)
        if(s == (employeeList[i].getlName() + ", " + employeeList[i].getfName()))
            return i;
    return -1; //should never return -1
}

//add wage
void MainWindow::on_pushButton_15_clicked()
{
    double rate, hours;

    //validate
    QMessageBox errmsg;
    errmsg.setWindowTitle("WorkerPayProgram");
    errmsg.setText("Please select an employee");
    if(ui->comboBox_3->currentText() == "")
    {
        errmsg.exec();
        return;
    }
    errmsg.setText("Wage rate must be greater than 0");
    if(ui->lineEdit_6->text() == "" || (ui->lineEdit_6->text().toDouble() <= 0.00))
    {
        errmsg.exec();
        return;
    }
    errmsg.setText("Wage hours must be greater than 0");
    if(ui->lineEdit_7->text() == "" || (ui->lineEdit_7->text().toDouble() <= 0.00))
    {
        errmsg.exec();
        return;
    }
    //determine rate
    rate = ui->lineEdit_6->text().toDouble();
    //determine hours
    hours = ui->lineEdit_7->text().toDouble();
    wage tmp(employeeList[employeeLocation(ui->comboBox_3->currentText())], ui->textEdit_3->toPlainText().simplified()
        , hours, rate);
    wageList.append(tmp);
    updateWages();
    calculateAmountOwed();
    //clear lines
    ui->textEdit_3->setText("");
    ui->lineEdit_6->setText("");
    ui->lineEdit_7->setText("");
}

//update wage table
void MainWindow::updateWages()
{
    ui->listWidget_5->clear();
    for(int i = 0; i < wageList.count(); i++)
        ui->listWidget_5->addItem(wageList[i].getEmployee().getfName() + " - "
                                  + QString::number(wageList[i].getWage())
                                  + " - " + wageList[i].getDescription());
    updateRecordTable();
    saveFile(saveFileName);
}

//remove wage
void MainWindow::on_pushButton_16_clicked()
{
    if(!ui->listWidget_5->currentItem())
        return;
    for(int i = 0; i < wageList.count(); i++)
    {
        if(ui->listWidget_5->currentItem()->text() == wageList[i].getEmployee().getfName()
                + " - " + QString::number(wageList[i].getWage())
                + " - " + wageList[i].getDescription())
        {
            wageList.remove(i);
            ui->listWidget_5->removeItemWidget(ui->listWidget_5->item(i));
            updateWages();
            calculateAmountOwed();
            break;
        }
    }
}

//add deduction
void MainWindow::on_pushButton_18_clicked()
{
    //validate
    QMessageBox errmsg;
    errmsg.setWindowTitle("WorkerPayProgram");
    errmsg.setText("Please select an employee");
    if(ui->comboBox_2->currentText() == "")
    {
        errmsg.exec();
        return;
    }
    errmsg.setText("Deduction amount must be greater than 0");
    if(ui->lineEdit_5->text() == "" || (ui->lineEdit_5->text().toDouble() <= 0.00))
    {
        errmsg.exec();
        return;
    }
    //add deduction
    deduction tmp(employeeList[employeeLocation(ui->comboBox_2->currentText())],
            ui->textEdit_2->toPlainText().simplified(), ui->lineEdit_5->text().toDouble());
    deductionList.append(tmp);
    updateDeductions();
    calculateAmountOwed();
    //clear lines
    ui->textEdit_2->setText("");
    ui->lineEdit_5->setText("");
}

//update deduction table
void MainWindow::updateDeductions()
{
    ui->listWidget_4->clear();
    for(int i = 0; i < deductionList.count(); i++)
    {
        ui->listWidget_4->addItem(deductionList[i].getEmployee().getfName() + " - "
                                  + QString::number(deductionList[i].getAmount())
                                  + " - " + deductionList[i].getDescription());
    }
    updateRecordTable();
    saveFile(saveFileName);
}

//remove deduction
void MainWindow::on_pushButton_14_clicked()
{
    if(!ui->listWidget_4->currentItem())
        return;
    for(int i = 0; i < deductionList.count(); i++)
    {
        if(ui->listWidget_4->currentItem()->text() == deductionList[i].getEmployee().getfName()
                + " - " + QString::number(deductionList[i].getAmount())
                + " - " + deductionList[i].getDescription())
        {
            deductionList.remove(i);
            ui->listWidget_4->removeItemWidget(ui->listWidget_4->item(i));
            updateDeductions();
            calculateAmountOwed();
            break;
        }
    }
}

//main calculation function
void MainWindow::calculateAmountOwed()
{
    double totalPartsAmount = 0;
    for(int i = 0; i < employeeList.count(); i++)
    {
        //to avoid double adding
        //double tmpAmtOwed = employeeList[i].getAmtOwed();
        employeeList[i].setAmtOwed(0);

        for(int j = 0; j < wageList.count(); j++)
        {
            if(wageList[j].isPaid())
                continue;
            else if(wageList[j].getEmployee() == employeeList[i])
                employeeList[i].setAmtOwed(employeeList[i].getAmtOwed() + wageList[j].getWage());
        }

        for(int j = 0; j < deductionList.count(); j++)
        {
            if(deductionList[j].isPaid())
                continue;
            else if(deductionList[j].getEmployee() == employeeList[i])
                employeeList[i].setAmtOwed(employeeList[i].getAmtOwed() - deductionList[j].getAmount());
        }
        for(int j = 0; j < invoiceList.count(); j++)
        {
            totalPartsAmount = 0.00;
            QVector<part> tmpPartList = invoiceList[j].getPartList();
            for(int k = 0; k < tmpPartList.count(); k++)
            {
                totalPartsAmount += tmpPartList[k].getPrice();
            }
            QVector<employee> tmpEmpList = invoiceList[j].getEmployeeList();
            QVector<double> tmpCommList = invoiceList[j].getCommissionList();
            QVector<bool> tmpPaidList = invoiceList[j].getPaidList();
            for(int k = 0; k < tmpEmpList.count(); k++)
            {
                //match employees
                if(employeeList[i] == tmpEmpList[k])
                {
                    if(tmpPaidList[k])
                        break;
                    employeeList[i].setAmtOwed(employeeList[i].getAmtOwed()
                                               + ((invoiceList[j].getInvoiceAmount() - totalPartsAmount)
                                               * tmpCommList[k] * .01));
                }
            }
        }

    }
    updateMainTable();
}

//choose wage employee
void MainWindow::on_comboBox_3_activated(const QString &arg1)
{
    if(ui->comboBox_3->currentText() != "")
    {
        ui->lineEdit_6->setText(QString::number(employeeList[employeeLocation(arg1)].getWage()));
        ui->lineEdit_7->setText("0");
    }
    else
    {
        ui->lineEdit_6->setText("");
        return;
    }
}

//choose invoice employee
void MainWindow::on_comboBox_activated(const QString &arg1)
{
    if(ui->comboBox->currentText() != "" && (employeeList[employeeLocation(arg1)].getComm() > 0)
            && (ui->lineEdit_3->text() == "" || (!ui->radioButton->isChecked() &&
            !ui->radioButton_2->isChecked() && !ui->radioButton_3->isChecked() &&
            !ui->radioButton_4->isChecked() && !ui->radioButton_5->isChecked() &&
            !ui->radioButton_6->isChecked())))
        ui->lineEdit_3->setText(QString::number(employeeList[employeeLocation(arg1)].getComm()));
}

//Commission radios
void MainWindow::on_radioButton_clicked()
{
    ui->lineEdit_3->setText(QString::number(15));
    ui->lineEdit_3->setEnabled(false);
}
void MainWindow::on_radioButton_2_clicked()
{
    ui->lineEdit_3->setText(QString::number(17.5));
    ui->lineEdit_3->setEnabled(false);
}
void MainWindow::on_radioButton_3_clicked()
{
    ui->lineEdit_3->setText(QString::number(20));
    ui->lineEdit_3->setEnabled(false);
}
void MainWindow::on_radioButton_4_clicked()
{
    ui->lineEdit_3->setText(QString::number(30));
    ui->lineEdit_3->setEnabled(false);
}
void MainWindow::on_radioButton_5_clicked()
{
    ui->lineEdit_3->setText(QString::number(35));
    ui->lineEdit_3->setEnabled(false);
}
void MainWindow::on_radioButton_6_clicked()
{
    ui->lineEdit_3->setEnabled(true);
}
//Radio Commissions END -------------------

//add employee to invoice
void MainWindow::on_pushButton_6_clicked()
{
    QMessageBox errmsg;
    errmsg.setWindowTitle("WorkerPayProgram");
    //validate
    if(ui->comboBox->currentText() == "")
    {
        errmsg.setText("Choose an employee");
        errmsg.exec();
        return;
    }
    if(ui->lineEdit_3->text().toDouble() <= 0)
    {
        errmsg.setText("Commission must be over 0%");
        errmsg.exec();
        return;
    }
    for(int i = 0; i < invoiceEmployeeList.count(); i++)
    {
        if(ui->comboBox->currentText() == invoiceEmployeeList[i].getlName() + ", "
                + invoiceEmployeeList[i].getfName())
        {
            errmsg.setText("Employee already added for this invoice");
            errmsg.exec();
            return;
        }
    }
    //add chosen employee to employeelist for that invoice
    invoiceEmployeeList.append(employeeList[employeeLocation(ui->comboBox->currentText())]);
    //add commission for that employee
    commissionList.append(ui->lineEdit_3->text().toDouble());
    //add paid for employee
    paidList.append(false);
    //add worker to listWidget_2 (employee list for invoice)
    updateInvoiceEmployeeTable();
    //clear lines
    //ui->lineEdit_3->setText("");

    ui->comboBox->setCurrentIndex(ui->comboBox->count()-1);
}

//update invoice employee table
void MainWindow::updateInvoiceEmployeeTable()
{
    //clear rows
    ui->listWidget_2->clear();
    //update rows with new info
    for(int i = 0; i < invoiceEmployeeList.count(); i++)
    {
        ui->listWidget_2->addItem(invoiceEmployeeList[i].getlName() + ", "
                                  + invoiceEmployeeList[i].getfName() + " - "
                                  + QString::number(commissionList[i]) + "%");
    }
    saveFile(saveFileName);
}

//remove employee from invoice
void MainWindow::on_pushButton_10_clicked()
{
    if(!ui->listWidget_2->currentItem())
        return;
    //find matching invoice employee from selected employee, then delete from invoice as well as commission
    for(int i = 0; i < invoiceEmployeeList.count(); i++)
    {
        if(ui->listWidget_2->currentItem()->text() == invoiceEmployeeList[i].getlName() + ", "
                + invoiceEmployeeList[i].getfName() + " - "
                + QString::number(commissionList[i])+ "%")
        {
            invoiceEmployeeList.remove(i);
            commissionList.remove(i);
            paidList.remove(i);
            ui->listWidget_2->removeItemWidget(ui->listWidget_2->item(i));
            break;
        }
    }
    updateInvoiceEmployeeTable();
}

//add part
void MainWindow::on_pushButton_7_clicked()
{
    //validate
    QMessageBox errmsg;
    errmsg.setWindowTitle("WorkerPayProgram");
    if(ui->textEdit->toPlainText().simplified() == "")
    {
        errmsg.setText("Part description cannot be empty");
        errmsg.exec();
        return;
    }
    if(ui->lineEdit_4->text().toDouble() <= 0)
    {
        errmsg.setText("Part amount must be greater than 0.00");
        errmsg.exec();
        return;
    }
    //add part to part list and table
    part tmp(ui->textEdit->toPlainText().simplified(), ui->lineEdit_4->text().toDouble());
    partList.append(tmp);
    updatePartTable();

    //clear lines
    ui->textEdit->clear();
    ui->lineEdit_4->setText("");
}

//update part table
void MainWindow::updatePartTable()
{
    ui->listWidget_3->clear();
    for(int i = 0; i < partList.count(); i++)
        ui->listWidget_3->addItem(partList[i].getDesc().simplified() + " - "
                                  + QString::number(partList[i].getPrice()));
    saveFile(saveFileName);
}

//remove part
void MainWindow::on_pushButton_11_clicked()
{

    if(!ui->listWidget_3->currentItem())
        return;
    //find matching part from part list
    for(int i = 0; i < partList.count(); i++)
    {
        if(ui->listWidget_3->currentItem()->text() == partList[i].getDesc().simplified() + " - "
                + QString::number(partList[i].getPrice()))
        {
            partList.removeAt(i);
            break;
        }
    }
    updatePartTable();
}

//add invoice
void MainWindow::on_pushButton_12_clicked()
{
    bool duplicate = false;
    bool save = false;
    int originalInvoiceIndex;
    QMessageBox errmsg;
    errmsg.setWindowTitle("WorkerPayProgram");
    double invoiceAmount;
    QString invoiceNumber = ui->lineEdit->text().simplified();

    //validate invoice amount
    if(invoiceNumber == "")
    {
        errmsg.setText("Invoice number cannot be blank");
        errmsg.exec();
        return;
    }
    if(ui->lineEdit_2->text().toDouble() <= 0)
    {
        errmsg.setText("Invoice amount must be greater than 0.00");
        errmsg.exec();
        return;
    }
    if(invoiceEmployeeList.count() <= 0)
    {
        errmsg.setText("Must add at least one employee to invoice");
        errmsg.exec();
        return;
    }
    if(ui->comboBox->currentText() != "")
    {
        errmsg.setText("Invoice has unadded worker, please add or clear worker first");
        errmsg.exec();
        return;
    }
    if(ui->lineEdit_4->text() != "" || ui->textEdit->toPlainText() != "")
    {
        errmsg.setText("Invoice has unadded parts, please add part or clear part amount and description first");
        errmsg.exec();
        return;
    }
    //check if duplicate
    //----------------------
    for(int i = 0; i < invoiceList.count(); i++)
    {
        if(invoiceList[i].getInvoiceNumber().simplified() == invoiceNumber)
        {
            duplicate = true;
            QMessageBox::StandardButton dupmsg;
            dupmsg = QMessageBox::question(this, "Duplicate", "Invoice already exists. Save Changes?",
                                        QMessageBox::Yes|QMessageBox::No);
            if (dupmsg == QMessageBox::Yes)
                save = true;
            else
                save = false;
            if(save)
                originalInvoiceIndex = i;
        }
    }
    //check if duplicate of record invoices
    bool recordDuplicate = false;
    for(int i = 0; i < recordList.count(); i++)
    {
        for(int j = 0; j < recordList[i].getInvoiceListCount(); j++)
        {
            if(recordList[i].getInvoice(j).getInvoiceNumber().simplified() == invoiceNumber)
            {
                recordDuplicate = true;
                QMessageBox::StandardButton dupmsg;
                dupmsg = QMessageBox::question(this, "Duplicate", "Invoice exists in records. Add anyway?",
                                            QMessageBox::Yes|QMessageBox::No);
                if (dupmsg == QMessageBox::Yes)
                    break;
                else
                    return;
            }
            if(recordDuplicate)
                break;
        }
    }

    //----------------------
    //date and amount
    QString date = QDate::currentDate().toString("MM/dd/yyyy");
    invoiceAmount = ui->lineEdit_2->text().toDouble();

    //not duplicate, then add as new

    if(!duplicate)
    {
        invoice tmp(invoiceNumber, date, invoiceAmount, invoiceEmployeeList, commissionList, partList);
        invoiceList.append(tmp);
    }
    //duplicate but no save then do nothing
    if(duplicate && !save)
        return;
    //duplicate and override original
    if(duplicate && save)
    {
        invoice tmp(invoiceNumber, date, invoiceAmount, invoiceEmployeeList, commissionList, partList);
        //tmp.setPaidList(invoiceList[originalInvoiceIndex].getPaidList()); //transfer over paid list
        syncPaid(invoiceList[originalInvoiceIndex], tmp);
        invoiceList[originalInvoiceIndex] = tmp;
    }

    //clear data and update lists
    ui->lineEdit->setText("");
    ui->lineEdit_2->setText("");
    //invoiceEmployeeList.clear();
    //commissionList.clear();
    partList.clear();
    paidList.clear();
    updateInvoiceEmployeeTable();
    updatePartTable();

    //update invoice table and calculate amount owed
    updateInvoiceTable();
    calculateAmountOwed();
}

//sync paid list from invoice function
void MainWindow::syncPaid(invoice &o, invoice &n)
{
    QVector<bool> tmpPaid = o.getPaidList();
    QVector<bool> newTmpPaid;
    QVector<employee> oldEmployees = o.getEmployeeList();
    QVector<employee> newEmployees = n.getEmployeeList();

    //from 0 to old paid, transfer. From old paid to new paid (new employees added), set false default
    for(int i = 0; i < n.getPaidList().count(); i++)
    {
        //if employee matches old employee, overwrite paid
        if(i < tmpPaid.count())
        {
            if(oldEmployees[i] == newEmployees[i])
                newTmpPaid.append(tmpPaid[i]);
            else
                newTmpPaid.append(false);
        }
        else
            newTmpPaid.append(false);
    }
    n.setPaidList(newTmpPaid);
}

//update invoice table
void MainWindow::updateInvoiceTable()
{
    //update list
    ui->listWidget->clear();
    for(int i = 0; i < invoiceList.count(); i++)
    {
        ui->listWidget->addItem(invoiceList[i].getInvoiceNumber() + " - "
                                + invoiceList[i].getDateEntered());
    }
    saveFile(saveFileName);
}

//remove invoice
void MainWindow::on_pushButton_9_clicked()
{
    bool save;
    if(!ui->listWidget->currentItem())
        return;
    //find matching part from part list
    for(int i = 0; i < invoiceList.count(); i++)
    {
        if(ui->listWidget->currentItem()->text() == (invoiceList[i].getInvoiceNumber() + " - "
                + invoiceList[i].getDateEntered()))
        {
            QMessageBox::StandardButton warningmsg;
            warningmsg = QMessageBox::question(this, "Warning", "Are you sure you want to remove invoice #"
                                               + invoiceList[i].getInvoiceNumber(),
                                        QMessageBox::Yes|QMessageBox::No);
            if (warningmsg == QMessageBox::Yes)
                save = true;
            else
                save = false;

            if(save)
                invoiceList.removeAt(i);
            break;
        }
    }
    //update invoice table and calculate amount owed
    updateInvoiceTable();
    calculateAmountOwed();
}

//clear invoice
void MainWindow::on_pushButton_13_clicked()
{
    ui->lineEdit->setText("");
    ui->lineEdit_2->setText("");
    ui->lineEdit_4->setText("");
    ui->textEdit->setText("");
    ui->comboBox->setCurrentIndex(ui->comboBox->count()-1);
    ui->radioButton->setChecked(0);
    ui->radioButton_2->setChecked(0);
    ui->radioButton_3->setChecked(0);
    ui->radioButton_4->setChecked(0);
    ui->radioButton_5->setChecked(0);
    ui->radioButton_6->setChecked(0);
    invoiceEmployeeList.clear();
    commissionList.clear();
    partList.clear();
    paidList.clear();
    updateInvoiceEmployeeTable();
    updatePartTable();
}

//main pay / create record function
void MainWindow::on_pushButton_clicked()
{
    QString date = QDate::currentDate().toString("MM/dd/yyyy");
    //record tmpRec;
    //employee tmpEmp;
    double tmpPaidAmt;
    QVector<invoice> tmpInvoices;
    QVector<deduction> tmpDeductions;
    QVector<wage> tmpWages;
    QString tmpName;
    //go through each item in main table
    for(int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        //locate employee location by name in tableWidget
        tmpName = ui->tableWidget->item(i, 1)->text() + ", " + ui->tableWidget->item(i, 2)->text();
        tmpPaidAmt = 0;
        tmpWages.clear();
        tmpDeductions.clear();
        //if checked
        if(ui->tableWidget->item(i, 0)->checkState() == Qt::Checked)
        {
            if(employeeList[employeeLocation(tmpName)].getAmtOwed() == 0.00)
                break;
            else
                tmpPaidAmt = employeeList[employeeLocation(tmpName)].getAmtOwed();

            employeeList[employeeLocation(tmpName)].setLastPaidAmt(employeeList[employeeLocation(tmpName)].getAmtOwed());
            employeeList[employeeLocation(tmpName)].setAmtOwed(0.00);
            employeeList[employeeLocation(tmpName)].setLastPaid(date);

            for(int j = 0; j < invoiceList.count(); j++)
            {
                QVector<employee> tmpEmpList = invoiceList[j].getEmployeeList();
                QVector<bool> tmpPaidList = invoiceList[j].getPaidList();
                for(int k = 0; k < tmpEmpList.count(); k++)
                {
                    if(tmpEmpList[k] == employeeList[employeeLocation(tmpName)])
                    {

                        if(!tmpPaidList[k])//if not paid, mark paid
                        {
                            tmpPaidList[k] = true;
                            tmpInvoices.append(invoiceList[j]);
                        }
                    }
                }
                invoiceList[j].setPaidList(tmpPaidList);
            }
            //remove fully paid invoices
            bool fullyPaid = false;
            for(int j = 0; j < invoiceList.count(); j++)
            {
                fullyPaid = true;
                QVector<bool> tmpPaidList = invoiceList[j].getPaidList();
                for(int k = 0; k < tmpPaidList.count(); k++)
                {
                    if(!tmpPaidList[k])
                        fullyPaid = false;
                }
                if(fullyPaid)
                {
                    invoiceList.removeAt(j);
                    j-=1;
                }
            }
            //pay wages/deductions
            for(int j = 0; j < wageList.count(); j++)
            {
                if(wageList[j].getEmployee() == employeeList[employeeLocation(tmpName)])
                {
                    tmpWages.append(wageList[j]);
                    wageList[j].markPaid();
                    wageList.removeAt(j);
                    j-=1;
                }
            }
            for(int j = 0; j < deductionList.count(); j++)
            {
                if(deductionList[j].getEmployee() == employeeList[employeeLocation(tmpName)])
                {
                    tmpDeductions.append(deductionList[j]);
                    deductionList[j].markPaid();
                    deductionList.removeAt(j);
                    j -= 1;
                }
            }
        }

        //create record
        if(tmpPaidAmt != 0)
        {
            employeeRecord tmpRec(employeeList[employeeLocation(tmpName)], date, tmpPaidAmt, tmpInvoices, tmpDeductions, tmpWages);
            //recordList.append(tmpRec); //this pushes record to end, we want at front
            recordList.insert(0, tmpRec);
            //clear current record view list in other panel
            ui->listWidget_9->clear();
            //clear lines
            clearRecordInfo();
        }
        tmpInvoices.clear();
    }
    calculateAmountOwed();
    updateWages();
    updateDeductions();
    //updateInvoiceTable();
    updateInvoiceTable(); //have to run func twice for some reason, need to fix later.
}

//clear record info
void MainWindow::clearRecordInfo()
{
    ui->lineEdit_8->setText("");
    ui->lineEdit_9->setText("");
    ui->lineEdit_10->setText("");
    ui->listWidget_6->clear();
    ui->listWidget_7->clear();
    ui->listWidget_8->clear();
    ui->textEdit_4->clear();
}

//open invoice
void MainWindow::on_pushButton_8_clicked()
{
    if(!ui->listWidget->currentItem())
        return;
    openInvoice(ui->listWidget->currentItem()->text());
}

//open double clicked invoice
void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    openInvoice(item->text());
}

//open employee
void MainWindow::on_pushButton_3_clicked()
{
    if(!ui->employeeListWidget->currentItem())
        return;
    openEmployee(ui->employeeListWidget->currentItem()->text());
}

//open double clicked employee
void MainWindow::on_employeeListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    openEmployee(item->text());
}

//open record
void MainWindow::on_pushButton_20_clicked()
{
    if(!ui->listWidget_9->currentItem())
        return;
    openSpecificRecord(ui->listWidget_9->currentItem()->text());
}

//open double clicked record
void MainWindow::on_listWidget_9_itemDoubleClicked(QListWidgetItem *item)
{
    openSpecificRecord(item->text());
}

//open record invoice
void MainWindow::on_pushButton_19_clicked()
{
    if(!ui->listWidget_6->currentItem())
        return;
    openRecordInvoice(ui->listWidget_6->currentItem()->text());
}

//open double clicked record invoice
void MainWindow::on_listWidget_6_itemDoubleClicked(QListWidgetItem *item)
{
    openRecordInvoice(item->text());
}

//open record list
void MainWindow::on_pushButton_17_clicked()
{
    if(!ui->recordTable->currentItem())
        return;
    openRecordList(ui->recordTable->currentItem()->text());
}

//open double clicked record list
void MainWindow::on_recordTable_itemDoubleClicked(QListWidgetItem *item)
{
    openRecordList(item->text());
}

//open invoice
void MainWindow::openInvoice(QString tmp)
{
    if(tmp == "")
        return;

    //must clear current invoice data/workers/parts and fill with new info
    invoiceEmployeeList.clear();
    commissionList.clear();
    partList.clear();

    //new info
    for(int i = 0; i < invoiceList.count(); i++)
    {
        if(tmp == invoiceList[i].getInvoiceNumber() + " - "
                + invoiceList[i].getDateEntered())
        {
            ui->lineEdit->setText(invoiceList[i].getInvoiceNumber());
            ui->lineEdit_2->setText(QString::number(invoiceList[i].getInvoiceAmount()));
            invoiceEmployeeList = invoiceList[i].getEmployeeList();
            commissionList = invoiceList[i].getCommissionList();
            partList = invoiceList[i].getPartList();
            paidList = invoiceList[i].getPaidList();
        }
    }
    updateInvoiceEmployeeTable();
    updatePartTable();
}

//open employee
void MainWindow::openEmployee(QString tmp)
{
    if(tmp == "")
        return;
    for(int i = 0; i < employeeList.count(); i++)
    {
        if((employeeList[i].getlName() + ", " + employeeList[i].getfName())
                == tmp)
        {
            ui->fNameLine->setText(employeeList[i].getfName());
            ui->lNameLine->setText(employeeList[i].getlName());
            ui->wageLine->setText(QString::number(employeeList[i].getWage()));
            ui->commLine->setText(QString::number(employeeList[i].getComm()));
        }
    }
}

//open record list
void MainWindow::openRecordList(QString tmp)
{
    if(tmp=="")
        return;
    ui->listWidget_9->clear();
    clearRecordInfo();
    //cycle employees
    for(int i = 0; i < employeeList.count(); i++)
    {
        if((employeeList[i].getlName() + ", " + employeeList[i].getfName())
                == tmp)
            for(int j = 0; j < recordList.count(); j++)
                if(recordList[j].getEmployee() == employeeList[i])
                {
                    ui->listWidget_9->addItem(recordList[j].getEmployee().getfName() + " - " +
                        QString::number(recordList[j].getPaidAmount()) + " - " +recordList[j].getDatePaid()
                        + " - " + QString::number(recordList.count()-j));
                }
    }
}

//open record
void MainWindow::openSpecificRecord(QString tmp)
{
    if(tmp == "")
        return;
    //clear lines
    clearRecordInfo();
    for(int i = 0; i < recordList.count(); i++)
    {
        if(tmp == recordList[i].getEmployee().getfName() + " - " +
               QString::number(recordList[i].getPaidAmount()) + " - " + recordList[i].getDatePaid()
               + " - " + QString::number(recordList.count() - i))
        {
            ui->lineEdit_8->setText(recordList[i].getEmployee().getfName() + " " +
                recordList[i].getEmployee().getlName());
            ui->lineEdit_9->setText(recordList[i].getDatePaid());
            ui->lineEdit_10->setText(QString::number(recordList[i].getPaidAmount()));

            //ui->lineEdit_8->setText(QString::number(recordList[i].getInvoiceListCount()));//tmp
            for(int j = 0; j < recordList[i].getInvoiceListCount(); j++)
                ui->listWidget_6->addItem(recordList[i].getInvoice(j).getInvoiceNumber());
            for(int j = 0; j < recordList[i].getDedListCount(); j++)
                ui->listWidget_7->addItem(QString::number(recordList[i].getDeduction(j).getAmount())
                    + " - " + recordList[i].getDeduction(j).getDescription());
            for(int j = 0; j < recordList[i].getWageListCount(); j++)
                ui->listWidget_8->addItem(QString::number(recordList[i].getWage(j).getWage())
                    + " - " + recordList[i].getWage(j).getDescription());
        }
    }
}

//remove record
void MainWindow::on_pushButton_22_clicked()
{
    if(!ui->listWidget_9->currentItem())
        return;
    QMessageBox::StandardButton warningmsg;
    warningmsg = QMessageBox::question(this, "Warning", "Are you sure you want to remove this record?"
        ,QMessageBox::Yes|QMessageBox::No);
    if (warningmsg == QMessageBox::No)
        return;

    for(int i = 0; i < recordList.count(); i++)
    {
        if(ui->listWidget_9->currentItem()->text() == recordList[i].getEmployee().getfName() + " - " +
                QString::number(recordList[i].getPaidAmount()) + " - " + recordList[i].getDatePaid()
                + " - " + QString::number(recordList.count() - i))
        {
            recordList.removeAt(i);
            break;
        }
    }
    clearRecordInfo();
    ui->listWidget_9->clear();
    saveFile(saveFileName);
}

//open invoice from record
void MainWindow::openRecordInvoice(QString tmp)
{
    if(tmp=="")
        return;
    employee tmpEmployee;
    part tmpPart;
    double tmpCommission;
    bool found;
    double totalPartsAmount = 0;
    //find clicked on invoice
    invoice tmpInvoice;
    for(int i = 0; i < recordList.count(); i++)
    {
        found = false;
        for(int j = 0; j < recordList[i].getInvoiceListCount(); j++)
        {
            if(tmp == recordList[i].getInvoice(j).getInvoiceNumber())
            {
                tmpInvoice = recordList[i].getInvoice(j);
                found = true;
                break;
            }
        }
        if(found)
            break;
    }
    if(!found) //should never be called
        return;
    //move invoice info to text edit
    ui->textEdit_4->clear();
    ui->textEdit_4->append("Invoice #" + tmpInvoice.getInvoiceNumber() + "\n");
    ui->textEdit_4->append("Amount: " + QString::number(tmpInvoice.getInvoiceAmount()) + "\n");
    ui->textEdit_4->append("Workers: \n");

    //total parts amount
    for(int i = 0; i < tmpInvoice.getPartList().count(); i++)
    {
        tmpPart = tmpInvoice.getPartList().at(i);
        totalPartsAmount += tmpPart.getPrice();
    }
    //workers and pay
    for(int i = 0; i < tmpInvoice.getEmployeeList().count(); i++)
    {
        tmpEmployee = tmpInvoice.getEmployeeList().at(i);
        tmpCommission = tmpInvoice.getCommissionList().at(i);
        ui->textEdit_4->append("\t" + tmpEmployee.getfName() + " " +
            tmpEmployee.getlName() + " - $" +
            QString::number((tmpInvoice.getInvoiceAmount() - totalPartsAmount) * tmpCommission * .01)
            + "\n");
    }
    //parts
    ui->textEdit_4->append("Parts: \n");
    if(tmpInvoice.getPartList().count() <= 0)
        ui->textEdit_4->append("\tNO PARTS\n");
    for(int i = 0; i < tmpInvoice.getPartList().count(); i++)
    {
        tmpPart = tmpInvoice.getPartList().at(i);
        ui->textEdit_4->append("\t" + tmpPart.getDesc() + " - $" +
            QString::number(tmpPart.getPrice()) + "\n");
    }
}

//clear record info button
void MainWindow::on_pushButton_21_clicked()
{
    clearRecordInfo();
    ui->listWidget_9->clear();
}

//save (ONLY WORKS FOR EMPLOYEES ATM)
void MainWindow::saveFile(QString fileName)
{
    if(fileName == "")
        return;
    //open file
    QFile file(fileName);
    // Trying to open in WriteOnly and Text mode
    if(!file.open(QFile::WriteOnly |
                  QFile::Text))
        return;
    QTextStream out(&file);

    //save counts
    out << employeeList.count() << "\n";
    out << invoiceList.count() << "\n";
    for(int i = 0; i < invoiceList.count(); i++)
    {
        out << invoiceList[i].getEmployeeList().count() << "\n"; //reuse for commissions and paid list
        out << invoiceList[i].getPartList().count() << "\n";
    }
    out << deductionList.count() << "\n";
    out << wageList.count() << "\n";
    out << recordList.count() << "\n";

    //save record counts
    for(int i = 0; i < recordList.count(); i++)
    {
        out << recordList[i].getInvoiceListCount() << "\n";
        for(int j = 0; j < recordList[i].getInvoiceListCount(); j++)
        {
            out << recordList[i].getInvoiceEmpListCount(j) << "\n";
            out << recordList[i].getInvoicePartListCount(j) << "\n";
        }
        out << recordList[i].getDedListCount() << "\n";
        out << recordList[i].getWageListCount() << "\n";
    }

    //save employees
    for(int i = 0; i < employeeList.count(); i++)
    {
        out << employeeList[i].getfName() << "\n";
        out << employeeList[i].getlName() << "\n";
        out << QString::number(employeeList[i].getComm()) << "\n";
        out << employeeList[i].getLastPaid() << "\n";
        out << QString::number(employeeList[i].getLastPaidAmt()) << "\n";
        out << employeeList[i].getSelected() << "\n";
        out << QString::number(employeeList[i].getWage()) << "\n";
        out << employeeList[i].isHidden() << "\n";
    }
    //save invoices
    for(int i = 0; i < invoiceList.count(); i++)
    {
        out << invoiceList[i].getInvoiceNumber() << "\n";
        out << invoiceList[i].getDateEntered() << "\n";
        out << QString::number(invoiceList[i].getInvoiceAmount()) << "\n";
        QVector <employee> tempEmployeeList = invoiceList[i].getEmployeeList();
        QVector<double> tempCommissionList = invoiceList[i].getCommissionList();
        QVector<bool> tempPaidList = invoiceList[i].getPaidList();
        QVector<part> tempPartList = invoiceList[i].getPartList();
        for(int j = 0; j < invoiceList[i].getEmployeeList().count(); j++)
        {
            out << tempEmployeeList[j].getlName() << ", " << tempEmployeeList[j].getfName() << "\n";
            out << QString::number(tempCommissionList[j]) << "\n";
            out << QString::number(tempPaidList[j]) << "\n";
        }
        for(int j = 0; j < invoiceList[i].getPartList().count(); j++)
        {
            out << tempPartList[j].getDesc() << "\n";
            out << QString::number(tempPartList[j].getPrice()) << "\n";
        }
    }
    //save deductions
    for(int i = 0; i < deductionList.count(); i++)
    {
        out << deductionList[i].getEmployee().getlName() << ", "
            << deductionList[i].getEmployee().getfName() << "\n";
        out << deductionList[i].getDescription() << "\n";
        out << QString::number(deductionList[i].getAmount()) << "\n";
        out << QString::number(deductionList[i].isPaid()) << "\n";
    }

    //save wages
    for(int i = 0; i < wageList.count(); i++)
    {
        out << wageList[i].getEmployee().getlName() << ", "
            << wageList[i].getEmployee().getfName() << "\n";
        out << wageList[i].getDescription() << "\n";
        out << QString::number(wageList[i].getHours()) << "\n";
        out << QString::number(wageList[i].getRate()) << "\n";
        out << QString::number(wageList[i].isPaid()) << "\n";
    }

    //save records
    for(int i = 0; i < recordList.count(); i++)
    {
        out << recordList[i].getEmployee().getlName() << ", "
            << recordList[i].getEmployee().getfName() << "\n";
        out << recordList[i].getDatePaid() << "\n";
        out << QString::number(recordList[i].getPaidAmount()) << "\n";
        for(int j = 0; j < recordList[i].getInvoiceListCount(); j++)
        {
            invoice tmp = recordList[i].getInvoice(j);
            out << tmp.getInvoiceNumber() << "\n";
            out << tmp.getDateEntered() << "\n";
            out << QString::number(tmp.getInvoiceAmount()) << "\n";
            QVector <employee> tempEmployeeList = tmp.getEmployeeList();
            QVector<double> tempCommissionList = tmp.getCommissionList();
            QVector<bool> tempPaidList = tmp.getPaidList();
            QVector<part> tempPartList = tmp.getPartList();
            for(int j = 0; j < tmp.getEmployeeList().count(); j++)
            {
                out << tempEmployeeList[j].getlName() << ", " << tempEmployeeList[j].getfName() << "\n";
                out << QString::number(tempCommissionList[j]) << "\n";
                out << QString::number(tempPaidList[j]) << "\n";
            }
            for(int j = 0; j < tmp.getPartList().count(); j++)
            {
                out << tempPartList[j].getDesc() << "\n";
                out << QString::number(tempPartList[j].getPrice()) << "\n";
            }
        }
        for(int j = 0; j < recordList[i].getDedListCount(); j++)
        {
            deduction tmpDeduction = recordList[i].getDeduction(j);
            out << tmpDeduction.getEmployee().getlName() << ", "
                << tmpDeduction.getEmployee().getfName() << "\n";
            out << tmpDeduction.getDescription() << "\n";
            out << QString::number(tmpDeduction.getAmount()) << "\n";
            out << QString::number(tmpDeduction.isPaid()) << "\n";
        }
        for(int j = 0; j < recordList[i].getWageListCount(); j++)
        {
            wage tmpWage = recordList[i].getWage(j);
            out << tmpWage.getEmployee().getlName() << ", "
                << tmpWage.getEmployee().getfName() << "\n";
            out << tmpWage.getDescription() << "\n";
            out << QString::number(tmpWage.getHours()) << "\n";
            out << QString::number(tmpWage.getRate()) << "\n";
            out << QString::number(tmpWage.isPaid()) << "\n";
        }
    }

    file.flush();
    file.close();
}

//load
void MainWindow::loadFile(QString fileName)
{

    QString fname, lname, comm, lastPaid, lastPaidAmt, selected,empWage, fullName,
            tempDescription, tempDatePaid, invNum, invDate, invAmt, invPartDesc,
            invPartAmt, invEmployee, tmpName;
    double recordAmount, tempDedAmount, tempHours, tempRate;
    bool tempPaid, hidden;
    QVector<double> invCommList;
    QVector<bool> invPaidList;
    QVector<part> invPartList;
    QVector<employee> invEmployeeList;
    QVector<invoice> recordInvoiceList;
    QVector<wage> recordWageList;
    QVector<deduction> recordDeductionList;
    QVector <int> numberOfRecordInvoices;
    QVector<QVector<int>> numberOfRecordInvoiceEmployees;
    QVector<QVector<int>> numberOfRecordInvoiceParts;
    QVector <int> numberOfRecordDeductions;
    QVector<int> numberOfRecordWages;
    QVector <int> numberOfInvoiceEmployees;
    QVector <int> numberOfInvoiceParts;

    //open file
    QFile inputFile(fileName);
    if (inputFile.open(QIODevice::ReadOnly))
    {
        //clear
       employeeList.clear();
       wageList.clear();
       deductionList.clear();
       invoiceList.clear();
       invoiceEmployeeList.clear();
       commissionList.clear();
       partList.clear();
       paidList.clear();
       recordList.clear();

       QTextStream in(&inputFile);
       //read counts
       int numberOfEmployees = in.readLine().toInt();
       int numberOfInvoices = in.readLine().toInt();
       for(int i = 0; i < numberOfInvoices; i++)
       {
           numberOfInvoiceEmployees.append(in.readLine().toInt()); //reuse for com/paid list
           numberOfInvoiceParts.append(in.readLine().toInt());
       }
       int numberOfDeductions = in.readLine().toInt();
       int numberOfWages = in.readLine().toInt();
       int numberOfEmployeeRecords = in.readLine().toInt();

       for(int i = 0; i < numberOfEmployeeRecords; i++)
       {
           QVector<int> tmpEmpCount, tmpPartCount;
           numberOfRecordInvoices.append(in.readLine().toInt());
           for(int j = 0; j < numberOfRecordInvoices[i];j++)
           {
               tmpEmpCount.append(in.readLine().toInt());
               tmpPartCount.append(in.readLine().toInt());
           }
           numberOfRecordInvoiceEmployees.append(tmpEmpCount);
           numberOfRecordInvoiceParts.append(tmpPartCount);
           numberOfRecordDeductions.append(in.readLine().toInt());
           numberOfRecordWages.append(in.readLine().toInt());
           tmpEmpCount.clear();
           tmpPartCount.clear();
       }
       //read employees
       for(int i = 0; i < numberOfEmployees; i++)
       {
           fname = in.readLine();
           lname = in.readLine();
           //amtOwed = in.readLine();
           comm = in.readLine();
           lastPaid = in.readLine();
           lastPaidAmt = in.readLine();
           selected = in.readLine();
           empWage = in.readLine();
           hidden = in.readLine().toInt();
           //create employee
           employee tmpEmployee(fname, lname, empWage.toDouble(), comm.toDouble(), selected.toDouble(),
             lastPaid, lastPaidAmt.toDouble());
           if(hidden)
               tmpEmployee.toggleHidden();
           employeeList.append(tmpEmployee);
       }
       //read invoices
       for(int i = 0; i < numberOfInvoices; i++)
       {
           invNum = in.readLine();
           invDate = in.readLine();
           invAmt  = in.readLine();
           for(int j = 0; j < numberOfInvoiceEmployees[i]; j++)
           {
               invEmployee = in.readLine(); //reads name of invoice employee
               //invEmployeeList.append(employeeList[employeeLocation(invEmployee)]);
               /////////////////////////////////////////////////////////////
               if(employeeLocation(invEmployee) > -1)
                    invEmployeeList.append(employeeList[employeeLocation(invEmployee)]); //finds employee by name
               else //parse name into two names and make temporary employee, this is used for removed employees
               {
                   QString l, f;
                   invEmployee.simplified();
                   invEmployee.replace(" ", "");
                   QStringList list = invEmployee.split(",", QString::SkipEmptyParts);
                   l = list.at(0);
                   f = list.at(1);
                   employee tmp(f,l,0,0);
                   invEmployeeList.append(tmp);
               }
               /////////////////////////////////////////////////////////////
               invCommList.append(in.readLine().toDouble());
               invPaidList.append(in.readLine().toInt());
           }
           for(int j = 0; j < numberOfInvoiceParts[i]; j++)
           {
               invPartDesc = in.readLine();
               invPartAmt = in.readLine();
               part tempPart(invPartDesc, invPartAmt.toDouble());
               invPartList.append(tempPart);
           }
           invoice tmpInvoice(invNum, invDate, invAmt.toDouble(), invEmployeeList, invCommList, invPartList);
           tmpInvoice.setPaidList(invPaidList);
           invoiceList.append(tmpInvoice);
           //clear lists
           invEmployeeList.clear();
           invCommList.clear();
           invPartList.clear();
           invPaidList.clear();
       }

       //read deductions
       for(int i = 0; i < numberOfDeductions; i++)
       {
           fullName = in.readLine();
           tempDescription = in.readLine();
           tempDedAmount = in.readLine().toDouble();
           tempPaid = in.readLine().toInt();
           deduction tempDeduction(employeeList[employeeLocation(fullName)], tempDescription, tempDedAmount);
           if(tempPaid)
               tempDeduction.markPaid();
           deductionList.append(tempDeduction);
       }

       //read wages
       for(int i = 0; i < numberOfWages; i++)
       {
           fullName = in.readLine();
           tempDescription = in.readLine();
           tempHours = in.readLine().toDouble();
           tempRate = in.readLine().toDouble();
           tempPaid = in.readLine().toInt();
           wage tempWage(employeeList[employeeLocation(fullName)], tempDescription, tempHours, tempRate);
           if(tempPaid)
               tempWage.markPaid();
           wageList.append(tempWage);
       }

       //read records

       for(int i = 0; i < numberOfEmployeeRecords; i++)
       {
           fullName = in.readLine();
           tempDatePaid = in.readLine();
           recordAmount = in.readLine().toDouble();
           for(int j = 0; j < numberOfRecordInvoices[i]; j++)
           {
               //clear lists
               invEmployeeList.clear();
               invCommList.clear();
               invPartList.clear();
               invPaidList.clear();
               //read
               invNum = in.readLine();
               invDate = in.readLine();
               invAmt  = in.readLine();
               for(int k = 0; k < numberOfRecordInvoiceEmployees[i][j]; k++)
               {
                   invEmployee = in.readLine(); //reads name of invoice employee
                   //invEmployeeList.append(employeeList[employeeLocation(invEmployee)]); //finds employee by name
                   ///////////////////////////////////////////////
                   if(employeeLocation(invEmployee) > -1)
                        invEmployeeList.append(employeeList[employeeLocation(invEmployee)]); //finds employee by name
                   else //parse name into two names and make temporary employee, this is used for removed employees
                   {
                       QString l, f;
                       invEmployee.simplified();
                       invEmployee.replace(" ", "");
                       QStringList list = invEmployee.split(",", QString::SkipEmptyParts);
                       l = list.at(0);
                       f = list.at(1);
                       employee tmp(f,l,0,0);
                       invEmployeeList.append(tmp);
                   }
                   ///////////////////////////////////////////////
                   invCommList.append(in.readLine().toDouble());
                   invPaidList.append(in.readLine().toInt());
               }
               for(int k = 0; k < numberOfRecordInvoiceParts[i][j]; k++)
               {
                   invPartDesc = in.readLine();
                   invPartAmt = in.readLine();
                   part tempPart(invPartDesc, invPartAmt.toDouble());
                   invPartList.append(tempPart);
               }
               invoice tmpInvoice(invNum, invDate, invAmt.toDouble(), invEmployeeList, invCommList, invPartList);
               tmpInvoice.setPaidList(invPaidList);
               recordInvoiceList.append(tmpInvoice);

           }
           for(int j = 0; j < numberOfRecordDeductions[i]; j++)
           {

               tmpName = in.readLine();
               tempDescription = in.readLine();
               tempDedAmount = in.readLine().toDouble();
               tempPaid = in.readLine().toInt();
               deduction tempDeduction(employeeList[employeeLocation(tmpName)], tempDescription, tempDedAmount);
               if(tempPaid)
                   tempDeduction.markPaid();
               recordDeductionList.append(tempDeduction);
           }
           for(int j = 0; j < numberOfRecordWages[i]; j++)
           {
               tmpName = in.readLine();
               tempDescription = in.readLine();
               tempHours = in.readLine().toDouble();
               tempRate = in.readLine().toDouble();
               tempPaid = in.readLine().toInt();
               wage tempWage(employeeList[employeeLocation(fullName)], tempDescription, tempHours, tempRate);
               if(tempPaid)
                   tempWage.markPaid();
               recordWageList.append(tempWage);
           }
           //create record and clear lists
           employeeRecord tmp(employeeList[employeeLocation(fullName)], tempDatePaid, recordAmount
               , recordInvoiceList, recordDeductionList, recordWageList);
           recordList.append(tmp);
           recordInvoiceList.clear();
           recordDeductionList.clear();
           recordWageList.clear();
       }
       inputFile.close();
    }
    calculateAmountOwed();
    updateMainTable();
    updateEmployeeTable();
    updateComboBoxes();
    updateWages();
    updateDeductions();
    updateInvoiceEmployeeTable();
    updatePartTable();
    updateInvoiceTable();
    updateRecordTable();

}

/*
 * on save/load file from browser
 * record invoice employees / parts not saving or loading right (might need double vector)
*/

//open file
void MainWindow::on_actionLoad_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Open File"), "C://", "All Files (*.*);; Save File (*.dat)");
    if(filename == "")
        return;
    //QMessageBox::information(this,tr("File Name"),filename);
    //load file chosen
    loadFile(filename);
    //update current save file and main window title
    saveFileName = filename;
    MainWindow::setWindowTitle("Worker Pay Program        " + saveFileName); //update main window title
    //save last opened file
    QString docLocation = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    //make folder if doesn't exist
    if(!QDir(docLocation + "/WorkerPayProgram").exists())
        QDir().mkdir(docLocation + "/WorkerPayProgram");
    //open file
    QFile lastFile(docLocation + "/WorkerPayProgram/saveLoc.dat");
    // Trying to open in WriteOnly and Text mode
    if(!lastFile.open(QFile::WriteOnly |
                  QFile::Text))
    {
        QMessageBox::information(this,tr("ERROR"),filename);
        return;
    }
    QTextStream out(&lastFile);

    out << filename;
    lastFile.flush();
    lastFile.close();
}

//open last file
void MainWindow::openLast()
{
    QString docLocation = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QFile inputFile(docLocation + "/WorkerPayProgram/saveLoc.dat");
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       //read name of last opened file
       QString fileName = in.readLine();
       loadFile(fileName);
       //update current save file
       saveFileName = fileName;
       MainWindow::setWindowTitle("Worker Pay Program        " + saveFileName); //update main window title
    }
    inputFile.close();
}

/*
//create filter
void MainWindow::on_pushButton_23_clicked()
{
    //tmp
    return;
    filter tmpFilter;
    FiltersWindow filtersWindow;
    filtersWindow.setModal(true);
    filtersWindow.exec();
}
*/

//save as action
void MainWindow::on_actionSave_As_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this,tr("Save File"), "C://", "All Files (*.*);; Save File (*.dat)");
    if(filename == "")
        return;
    //QMessageBox::information(this,tr("File Name"),filename);
    //save file chosen
    saveFile(filename);
    /*
    //update current save file
    saveFileName = filename;
    //save last opened file
    QString docLocation = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    //make folder if doesn't exist
    if(!QDir(docLocation + "/WorkerPayProgram").exists())
        QDir().mkdir(docLocation + "/WorkerPayProgram");
    //open file
    QFile lastFile(docLocation + "/WorkerPayProgram/saveLoc.dat");

    // Trying to open in WriteOnly and Text mode
    if(!lastFile.open(QFile::WriteOnly |
                  QFile::Text))
    {
        QMessageBox::information(this,tr("ERROR"),filename);
        return;
    }
    QTextStream out(&lastFile);
    out << filename;
    lastFile.flush();
    lastFile.close();
    */
}

//backup
void MainWindow::on_actionBackup_triggered()
{
    QString date = QDate::currentDate().toString("MMddyyyy");
    QString time = QTime::currentTime().toString("hhmmss");
    QString docLocation = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    //make folder if doesn't exist
    if(!QDir(docLocation + "/WorkerPayProgram").exists())
        QDir().mkdir(docLocation + "/WorkerPayProgram");

    QString save = docLocation + "/WorkerPayProgram/backup" + date;
    if(QFileInfo(save + ".dat").exists())
        save = save + time;
    save = save + ".dat";
    saveFile(save);
    QMessageBox::information(this,tr("Backup"), "Backup saved at " + docLocation + "/WorkerPayProgram");
}

//show removed employees
void MainWindow::on_pushButton_26_clicked()
{
    //clear rows
    ui->employeeListWidget->clear();

    //update rows with new info
    for(int i = 0; i < employeeList.count(); i++)
    {
        ui->employeeListWidget->addItem(employeeList[i].getlName() + ", " + employeeList[i].getfName());
        if(employeeList[i].isHidden())
            ui->employeeListWidget->item(i)->setForeground(Qt::red);
    }
}

//hide removed employees
void MainWindow::on_pushButton_27_clicked()
{
    //clear rows
    ui->employeeListWidget->clear();

    //update rows with new info
    for(int i = 0; i < employeeList.count(); i++)
    {
        if(!employeeList[i].isHidden())
            ui->employeeListWidget->addItem(employeeList[i].getlName() + ", " + employeeList[i].getfName());
    }
}

//about
void MainWindow::on_actionAbout_triggered()
{
    QMessageBox aboutDial;
    aboutDial.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    aboutDial.setWindowTitle("About");
    aboutDial.setText("This program is free software: you can redistribute it and/or modify"
                      "\nit under the terms of the GNU General Public License as published by"
                      "\nthe Free Software Foundation, either version 3 of the License, or"
                      "\n(at your option) any later version."

                      "\n\nThis program is distributed in the hope that it will be useful,"
                      "\nbut WITHOUT ANY WARRANTY; without even the implied warranty of"
                      "\nMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the"
                      "\nGNU General Public License for more details."

                      "\n\nYou should have received a copy of the GNU General Public License"
                      "\nalong with this program.  If not, see <http://www.gnu.org/licenses/>.");
    aboutDial.exec();
}

//select all WIP
void MainWindow::on_pushButton_28_clicked()
{
    return;
    bool allChecked = false;
    for(int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        if(ui->tableWidget->item(i,0)->checkState() == Qt::Checked)
            allChecked = true;
        else
        {
            allChecked = false;
            break;
        }

    }
    for(int i = 0; i < ui->tableWidget->rowCount();i++)
    {
        ui->tableWidget->item(i,0)->setCheckState(Qt::Checked);
    }
}

//search for invoice #
void MainWindow::on_pushButton_23_clicked()
{
    //update list
    ui->listWidget->clear();
    for(int i = 0; i < invoiceList.count(); i++)
    {
        if(invoiceList[i].getInvoiceNumber().contains(ui->lineEdit_11->text().simplified()))
        {
            if(ui->comboBox_4->currentText() == "")
                ui->listWidget->addItem(invoiceList[i].getInvoiceNumber() + " - "
                                + invoiceList[i].getDateEntered());
            else//see if any workers match
                for(int j = 0; j < invoiceList[i].getEmployeeList().count(); j++)
                {
                    QVector<employee> tmpEmpList = invoiceList[i].getEmployeeList();
                    if(tmpEmpList[j].getlName() + ", " + tmpEmpList[j].getfName()== ui->comboBox_4->currentText())
                        ui->listWidget->addItem(invoiceList[i].getInvoiceNumber() + " - "
                                                + invoiceList[i].getDateEntered());
                }
        }
    }
}

//clear filters
void MainWindow::on_pushButton_24_clicked()
{
    updateInvoiceTable();
    //ui->comboBox_4->clear();
    ui->lineEdit_11->clear();
}

//toggle sort invoices
void MainWindow::on_pushButton_25_clicked()
{
    sortedInvoiceList = invoiceList;
    sorted = !sorted; //flip which invoice list to use
    //bubble sort
    bool swap = true;
    for(int i = 0; swap; i++)
    {
        swap = false;
        for(int j = sortedInvoiceList.count()-1; j > i; j--)
        {
            if(sortedInvoiceList[j].getInvoiceNumber().toInt() < sortedInvoiceList[j-1].getInvoiceNumber().toInt())
            {
                invoice temp = sortedInvoiceList[j];
                qSwap(sortedInvoiceList[j], sortedInvoiceList[j-1]);
                qSwap(sortedInvoiceList[j-1], temp);
                swap = true;
            }
            else if (sortedInvoiceList[j].getInvoiceNumber().toInt() == sortedInvoiceList[j-1].getInvoiceNumber().toInt())
            {
                if(sortedInvoiceList[j].getInvoiceNumber().toInt() < sortedInvoiceList[j-1].getInvoiceNumber().toInt())
                {
                    invoice temp = sortedInvoiceList[j];
                    qSwap(sortedInvoiceList[j], sortedInvoiceList[j-1]);
                    qSwap(sortedInvoiceList[j-1], temp);
                    swap = true;
                }
            }
        }
    }
    //update invoice list
    if(sorted) //use sorted list
    {
        //update list
        ui->listWidget->clear();
        for(int i = 0; i < sortedInvoiceList.count(); i++)
        {
            if(sortedInvoiceList[i].getInvoiceNumber().contains(ui->lineEdit_11->text()))
            {
                ui->listWidget->addItem(sortedInvoiceList[i].getInvoiceNumber() + " - "
                                    + sortedInvoiceList[i].getDateEntered());
            }
        }
    }
    else //use original unsorted list
        updateInvoiceTable();
}

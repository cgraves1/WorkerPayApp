#include "invoice.h"

invoice::invoice()
{
    this->invoiceNumber ="";
    this->dateEntered = "";
    this->invoiceAmount = 0.0;
    this->hidden = false;
}

invoice::invoice(QString n, QString d, double a, QVector<employee> e, QVector<double> c, QVector<part> p)
{
    this->invoiceNumber = n;
    this->dateEntered = d;
    this->invoiceAmount = a;
    this->invoiceEmployees = e;
    this->employeeCommission = c;
    this->invoiceParts = p;
    this->hidden = false;
    for(int i = 0; i < e.count(); i++)
        invoicePaidList.append(false);
}

void invoice::setPaidList(QVector<bool> b)
{
    this->invoicePaidList = b;
}

QString invoice::getInvoiceNumber()
{
    return this->invoiceNumber;
}

QString invoice::getDateEntered()
{
    return this->dateEntered;
}
double invoice::getInvoiceAmount()
{
    return this->invoiceAmount;
}
QVector<employee> invoice::getEmployeeList()
{
    return this->invoiceEmployees;
}
QVector<double> invoice::getCommissionList()
{
    return this->employeeCommission;
}
QVector<part> invoice::getPartList()
{
    return this->invoiceParts;
}

QVector<bool> invoice::getPaidList()
{
    return this->invoicePaidList;
}

void invoice::toggleHidden()
{
    this->hidden = !this->hidden;
}

bool invoice::isHidden()
{
    return this->hidden;
}

#ifndef INVOICE_H
#define INVOICE_H
#include <QVector>
#include <QString>
#include "employee.h"
#include "part.h"


class invoice
{
public:
    invoice();
    invoice(QString,QString,double,QVector<employee>,QVector<double>,QVector<part>);
    QString getInvoiceNumber();
    QString getDateEntered();
    double getInvoiceAmount();
    QVector<bool> getPaidList();
    QVector<employee> getEmployeeList();
    QVector<double> getCommissionList();
    QVector<part> getPartList();
    void setPaidList(QVector<bool>);
    void toggleHidden();
    bool isHidden();

private:
    QString invoiceNumber;
    QString dateEntered;
    double invoiceAmount;
    bool hidden;
    QVector<employee> invoiceEmployees;
    QVector<double> employeeCommission; //chosen when adding invoice
    QVector<part> invoiceParts;
    QVector<bool> invoicePaidList; //keep track of when each employee paid
};

#endif // INVOICE_H

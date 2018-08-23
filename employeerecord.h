#ifndef EMPLOYEERECORD_H
#define EMPLOYEERECORD_H
#include "employee.h"
#include "invoice.h"
#include <QDate>
#include <QString>
#include <QVector>
#include "deduction.h"
#include "wage.h"


class employeeRecord
{
public:
    employeeRecord();
    employeeRecord(employee, QString, double, QVector<invoice>, QVector<deduction>, QVector<wage>);
    employee getEmployee();
    QString getDatePaid();
    double getPaidAmount();
    invoice getInvoice(int);
    deduction getDeduction(int);
    wage getWage(int);
    int getInvoiceListCount();
    int getInvoiceEmpListCount(int);
    int getInvoicePartListCount(int);
    int getWageListCount();
    int getDedListCount();
private:
    employee worker;
    QString datePaid;
    double paidAmount;
    QVector<invoice> recordInvoiceList;
    QVector<deduction> recordDeductionList;
    QVector<wage> recordWageList;
};

#endif // EMPLOYEERECORD_H

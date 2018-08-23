#include "employeerecord.h"

employeeRecord::employeeRecord()
{
}

employeeRecord::employeeRecord(employee w, QString d, double p, QVector<invoice> inv,
                               QVector<deduction> ded, QVector<wage> wage)
{
    this->worker = w;
    this->datePaid = d;
    this->paidAmount = p;
    this->recordInvoiceList = inv;
    this->recordDeductionList = ded;
    this->recordWageList = wage;
}

employee employeeRecord::getEmployee()
{
    return this->worker;
}

QString employeeRecord::getDatePaid()
{
    return this->datePaid;
}

double employeeRecord::getPaidAmount()
{
    return roundf(this->paidAmount*100)/100;
    //return this->paidAmount;
}

invoice employeeRecord::getInvoice(int i)
{
    return this->recordInvoiceList[i];
}

deduction employeeRecord::getDeduction(int i)
{
    return this->recordDeductionList[i];
}

wage employeeRecord::getWage(int i)
{
    return this->recordWageList[i];
}

int employeeRecord::getInvoiceListCount()
{
    return this->recordInvoiceList.count();
}

int employeeRecord::getWageListCount()
{
    return this->recordWageList.count();
}

int employeeRecord::getDedListCount()
{
    return this->recordDeductionList.count();
}

int employeeRecord::getInvoiceEmpListCount(int j)
{
    return this->recordInvoiceList[j].getEmployeeList().count();
}

int employeeRecord::getInvoicePartListCount(int j)
{
    return this->recordInvoiceList[j].getPartList().count();
}

#include "deduction.h"

deduction::deduction()
{

}

deduction::deduction(employee e, QString s , double a)
{
    worker = e;
    description = s;
    amount = a;
    paid = false;
}

employee deduction::getEmployee()
{
    return worker;
}

QString deduction::getDescription()
{
    return description;
}

double deduction::getAmount()
{
    return amount;
}

void deduction::markPaid()
{
    paid = true;
}

bool deduction::isPaid()
{
    return paid;
}

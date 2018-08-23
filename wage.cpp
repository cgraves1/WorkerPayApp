#include "wage.h"

wage::wage()
{
}

wage::wage(employee e, QString s, double h, double r)
{
    worker = e;
    description = s;
    hours = h;
    rate = r;
    paid = false;
}

employee wage::getEmployee()
{
    return worker;
}

QString wage::getDescription()
{
    return description;
}

double wage::getWage()
{
    return (hours * rate);
}

void wage::markPaid()
{
    paid = true;
}

bool wage::isPaid()
{
    return paid;
}

double wage::getHours()
{
    return hours;
}

double wage::getRate()
{
    return rate;
}

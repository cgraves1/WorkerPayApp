#ifndef WAGE_H
#define WAGE_H
#include <QString>
#include "employee.h"


class wage
{
public:
    wage();
    wage(employee, QString, double, double);
    employee getEmployee();
    QString getDescription();
    double getWage(); //hours * rate
    double getHours();
    double getRate();
    void markPaid();
    bool isPaid();
private:
    QString description;
    employee worker;
    double hours;
    double rate;
    bool paid;
};

#endif // WAGE_H

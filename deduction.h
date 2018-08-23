#ifndef DEDUCTION_H
#define DEDUCTION_H
#include <QString>
#include "employee.h"


class deduction
{
public:
    deduction();
    deduction(employee, QString, double);
    employee getEmployee();
    QString getDescription();
    double getAmount();
    void markPaid();
    bool isPaid();
private:
    QString description;
    double amount;
    employee worker;
    bool paid;
};

#endif // DEDUCTION_H

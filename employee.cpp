#include "employee.h"
#include "math.h"

employee::employee()
{
    isSelected = false;
    hidden = false;
    fName = "";
    lName = "";
    amtOwed = -1.0;
    paidDate = "";
}
employee::employee(QString f,QString l, double w, double c)
{
    isSelected = false;
    hidden = false;
    fName = f;
    lName = l;
    wage = w;
    commission = c;
    amtOwed = 0.0;
}

//f, l, wage, com, selected, owed, paiddate, paidamt
employee::employee(QString f, QString l, double w, double c, bool s, QString d, double a)
{
    hidden = false;
    fName = f;
    lName = l;
    wage = w;
    commission = c;
    isSelected = s;
    //amtOwed = o;
    paidDate = d;
    amtLastPaid = a;
}

bool employee::getSelected()
{
    return this->isSelected;
}

void employee::setSelected(bool s)
{
    isSelected = s;
}


QString employee::getfName()
{
    return this->fName;
}

void employee::setfName(QString s)
{
    fName = s;
}


QString employee::getlName()
{
    return this->lName;
}

void employee::setlName(QString s)
{
    lName = s;
}

double employee::getAmtOwed()
{

    return roundf(this->amtOwed*100)/100;
}

void employee::setAmtOwed(double d)
{

    amtOwed = roundf(d*100)/100;
}

double employee::getWage()
{
    return this->wage;
}

void employee::setWage(double w)
{
    wage = w;
}

double employee::getComm()
{
    return this->commission;
}

void employee::setComm(double c)
{
    commission = c;
}

QString employee::getLastPaid()
{
    return this->paidDate;
}

void employee::setLastPaid(QString d)
{
    paidDate = d;
}

double employee::getLastPaidAmt()
{
    return this->amtLastPaid;
}

void employee::setLastPaidAmt(double d)
{
   amtLastPaid = d;
}

bool employee::operator==(employee e)
{
    if((this->getlName() == e.getlName()) && (this->getfName() == e.getfName()))
        return true;
    else return false;
}

void employee::toggleHidden()
{
    this->hidden = !this->hidden;
}

bool employee::isHidden()
{
    return this->hidden;
}

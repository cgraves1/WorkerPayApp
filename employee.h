#ifndef EMPLOYEE_H
#define EMPLOYEE_H
#include <QString>


class employee
{
public:
    //constructor
    employee();
    employee(QString,QString, double, double);//first, last, wage, com
    //f, l, wage, com, selected, paiddate, paidamt
    employee(QString, QString, double, double, bool, QString, double);
    bool getSelected();
    void setSelected(bool);
    QString getfName();
    void setfName(QString);
    QString getlName();
    void setlName(QString);
    double getAmtOwed();
    void setAmtOwed(double);
    double getWage();
    void setWage(double);
    double getComm();
    void setComm(double);
    QString getLastPaid();
    void setLastPaid(QString);
    double getLastPaidAmt();
    void setLastPaidAmt(double);
    bool operator==(employee);
    void toggleHidden();
    bool isHidden();

private:
    bool isSelected; // to pay
    //user set
    QString fName;
    QString lName;
    double wage;
    double commission;
    bool hidden;


    //calculated
    QString paidDate;
    double amtOwed;
    double amtLastPaid;
};

#endif // EMPLOYEES_H

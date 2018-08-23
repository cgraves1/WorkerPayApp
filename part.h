#ifndef PART_H
#define PART_H
#include <QString>


class part
{
public:
    part();
    part(QString, double);
    QString getDesc();
    double getPrice();
private:
    QString description;
    double price;
};

#endif // PART_H

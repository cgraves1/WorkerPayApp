#include "part.h"

part::part()
{
    price = 0.00;
    description = "";
}

part::part(QString d, double p)
{
    description = d;
    price = p;
}

QString part::getDesc()
{
    return this->description;
}

double part::getPrice()
{
    return this->price;
}

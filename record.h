#ifndef RECORD_H
#define RECORD_H
#include <QVector>
#include <QString>
#include "invoice.h"


class record
{
public:
    record();
private:
    QVector<invoice> invoices;
    QVector<employee> workers;

};

#endif // RECORD_H

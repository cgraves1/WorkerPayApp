#include "filter.h"

Filter::Filter()
{
    invoiceNumber = "";
    workerName = "";
}

Filter::Filter(QString n, QString w)
{
    invoiceNumber = n;
    workerName = w;
}

void Filter::setInvoiceNumber(QString n)
{
    invoiceNumber = n;
}

void Filter::setWorker(QString w)
{
    workerName = w;
}

QString Filter::getInvoiceNumber()
{
    return invoiceNumber;
}

QString Filter::getWorker()
{
    return workerName;
}

#ifndef FILTER_H
#define FILTER_H
#include <QString>


class Filter
{
public:
    Filter();
    Filter(QString, QString);
    void setInvoiceNumber(QString);
    void setWorker(QString);
    QString getInvoiceNumber();
    QString getWorker();
private:
    QString invoiceNumber;
    QString workerName;
};

#endif // FILTER_H

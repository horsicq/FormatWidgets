#ifndef ELFPROCESSDATA_H
#define ELFPROCESSDATA_H

#include <QObject>

class ELFProcessData : public QObject
{
    Q_OBJECT
public:
    explicit ELFProcessData(QObject *parent = nullptr);

signals:

};

#endif // ELFPROCESSDATA_H

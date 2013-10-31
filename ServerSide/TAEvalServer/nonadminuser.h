#ifndef NONADMINUSER_H
#define NONADMINUSER_H
#include <QtCore>

class NonAdminUser
{
public:
    NonAdminUser(QString);

    QString getId();
    virtual QString getUserType() =0;

    void setId(QString);

private:
    QString id;

};

#endif // NONADMINUSER_H

#ifndef NONADMINUSER_H
#define NONADMINUSER_H
#include <QtCore>

class NonAdminUser
{
public:
    NonAdminUser(QString);

    QString getId();

    void setId(QString);

private:
    QString id;
};

#endif // NONADMINUSER_H

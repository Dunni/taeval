#ifndef NONADMINUSER_H
#define NONADMINUSER_H
#include <QtCore>

class NonAdminUser
{
public:
    NonAdminUser(QString aid, QString arole);
    QString getID(){return id;}
    QString getRole(){return role;}

private:
    QString id;
    QString role;
};

#endif // NONADMINUSER_H

#ifndef NONADMINUSER_H
#define NONADMINUSER_H
#include <QtCore>

class NonAdminUser
{
public:
    NonAdminUser(QString);

    //Getters
    QString getId();
    virtual QString getUserType() =0;

    //Setters
    void setId(QString);

    QString toString();


private:
    QString id;

};

#endif // NONADMINUSER_H

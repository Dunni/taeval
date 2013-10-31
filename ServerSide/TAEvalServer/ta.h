#ifndef TA_H
#define TA_H
#include "nonadminuser.h"

class TA : public NonAdminUser
{
    typedef QList<TA> TAList;

public:
    TA(QString);

    QString getUserType();

    //Parsers
    static QString listToString(TAList);
    static TAList stringToList(QString);

};

#endif // TA_H

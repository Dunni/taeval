#ifndef TA_H
#define TA_H
#include "nonadminuser.h"

class TA : public NonAdminUser
{
public:
    TA(QString);

    QString getUserType();

};

#endif // TA_H

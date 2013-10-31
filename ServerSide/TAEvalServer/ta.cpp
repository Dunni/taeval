#include "ta.h"

TA::TA(QString id)
    :NonAdminUser(id)
{}

QString TA::getUserType(){return QString("TA");}


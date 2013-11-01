#include "nonadminuser.h"

NonAdminUser::NonAdminUser(QString aId)
    : id(aId)
{}

QString NonAdminUser::toString()
{
    return id;
}

QString NonAdminUser::getId(){return id;}



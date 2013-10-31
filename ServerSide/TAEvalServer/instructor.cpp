#include "instructor.h"

Instructor::Instructor(QString id)
    :NonAdminUser(id)
{}

QString Instructor::getUserType(){return QString("Instructor");}

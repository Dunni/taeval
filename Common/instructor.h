#ifndef INSTRUCTOR_H
#define INSTRUCTOR_H
#include "nonadminuser.h"
#include "course.h"


class Instructor : public NonAdminUser
{
    typedef QList<Instructor> InstructorList;

public:
    Instructor(QString id);
    QString getUserType();

    //Parsers
    static QString listToString(QList<Instructor>);
    static QList<Instructor> stringToList(QString);
};

#endif // INSTRUCTOR_H

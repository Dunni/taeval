#ifndef INSTRUCTOR_H
#define INSTRUCTOR_H
#include "nonadminuser.h"
#include "course.h"

typedef QList<Course> CourseList;

class Instructor : public NonAdminUser
{
public:
    Instructor(QString id);
    bool manageTask(QString action, QString TAUsername, QString CourseID, QString description, QDate startDate, QDate dueDate);
private:
    CourseList coursesTaught;
//    Course teaching;

};

#endif // INSTRUCTOR_H

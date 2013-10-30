#ifndef TAEVAL_H
#define TAEVAL_H
#include "course.h"
#include "task.h"
#include "instructor.h"
#include "nonadminuser.h"
#include "Storage/Storage.h"

typedef QList<NonAdminUser> UserList;

class TAEval
{
public:
    TAEval();
    QString verifyUser(QString);
    bool logIn(QString);
    bool createTask(QString userID, QString TAUsername, QString CourseID, QString description, QDate startDate, QDate dueDate);

private:
    UserList loggedOn;
    Storage* dataStore;

};

#endif // TAEVAL_H

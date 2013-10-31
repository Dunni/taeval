#ifndef TAEVAL_H
#define TAEVAL_H
#include "course.h"
#include "task.h"
#include "instructor.h"
#include "nonadminuser.h"
#include "Storage/Storage.h"

typedef QMap<QString, NonAdminUser*> UserList;
typedef QList<TA> TAList;
typedef QList<Course> CourseList;
typedef QList<Task> TaskList;
typedef QList<QString> StringList;

class TAEval
{
public:
    //Constructors and Destructors
    TAEval();
    ~TAEval();

    //API Functions
    QString verifyUser(QString);
    bool logIn(QString);
    bool logOut(QString);
    bool createTask(QString userID, QString TAUsername, QString CourseID, QString description, QDate startDate, QDate dueDate);
    bool editTask(QString userID, QString description, QDate startDate, QDate dueDate, QString taskId);
    bool deleteTask(QString userID, QString taskId);
    bool getCourses(QString userID, CourseList *&list, QString instructor, QString term="");
    bool getTAs(QString userID, QString course, TAList*& list);
    bool getTasks(QString userID, QString course, QString ta, TaskList*& list);

    //Parsers
    StringList stringToList(QString);
    QString listToString(StringList);

private:
    UserList loggedOn;
    Storage* dataStore;
    QString currentTerm;
};

#endif // TAEVAL_H

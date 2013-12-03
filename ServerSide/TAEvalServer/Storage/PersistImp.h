#ifndef PERSISTIMP_H
#define PERSISTIMP_H

#include "../../../Common/course.h"
#include "../../../Common/task.h"
#include "../../../Common/ta.h"
#include "../../../Common/instructor.h"
typedef qint32 INT;

class PersistImp {
public:

    virtual ~PersistImp(){}

    virtual bool connectToDB(QString connection, QString path = QDir::toNativeSeparators("Storage/taeval.db"), QString DBType = "QSQLITE") = 0;
    virtual void disconnect()=0;

    virtual bool restore(QString path = QDir::toNativeSeparators("Storage/taeval.sql"))=0;

    virtual bool verifyUser(QString name, QString &role)=0;

    /* actions: create, edit, delete*/
    /* manage Tasks */
    virtual  bool manageTask(QString user, QString action, Task task)=0;

    /* get courses teaching need delete *list after use */
    virtual bool getCourses(QString user, QString term, QList<Course> *& list,QString role)=0;

    /* get TAs for a course need delete *list after use */
    virtual bool getTAsForCourse(QString courseKey, QList<TA> *&list)=0;

    /* get Tasks for a TA and course need delete *list after use */
    virtual bool getTasksForTA(QString info, QString TAKey, QList<Task> *&list, QString role)=0;

    /* get Semesters for a given instructor */
    virtual QStringList getSemesters(QString user)=0;

    /* get a Task based on TaskID */
    virtual bool getTask(QString taskID, Task *& rv)=0;

    /* enterEvaluation */
    virtual bool enterEvaluation(INT TaskID,INT rating, QString feedback)=0;
};

#endif // PERSISTIMP_H

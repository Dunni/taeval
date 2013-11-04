#ifndef STORAGE_H
#define STORAGE_H

#define DEBUG

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QVariant>
#include <QSqlDriver>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QStringList>
#include "../../../Common/course.h"
#include "../../../Common/task.h"
#include "../../../Common/ta.h"
#include "../../../Common/instructor.h"


typedef qint32 INT;

class Storage{
public:
    /* Contructor */
    Storage(){ connected = false; }

    /* connect and disconnect */
    bool connectToDB(QString connection, QString path = "Storage//taeval.db", QString DBType = "QSQLITE");
    void disconnect();

    /* verifyUser */
    bool verifyUser(QString name, QString &role);

    /* actions: create, edit, delete*/
    /* manage Tasks */
    bool manageTask(QString user, QString action, Task task);

    /* get courses teaching need delete *list after use */
    bool getCoursesTeaching(QString Instrcutor, QString term, QList<Course> *& list);

    /* get TAs for a course need delete *list after use */
    bool getTAsForCourse(QString courseKey, QList<TA> *&list);

    /* get Tasks for a TA and course need delete *list after use */
    bool getTasksForTA(QString courseKey, QString TAKey, QList<Task> *&list);

    /* get Semesters for a given instructor */
    QStringList getSemesters(QString instrcutor);

    /* get a Task based on TaskID */
    bool getTask(QString taskID, Task *& rv);

    /* enterEvaluation */
    bool enterEvaluation(INT TaskID,INT rating, QString feedback);

    /* sendRequest for debug*/
    bool sendRequest(QString cmd);


#ifndef DEBUG
private:
#endif
    QSqlDatabase db; /* DataBase */
    bool connected; /* flag if connected to DB */
    void fixNull(QString &s);
    QString fixEscape(QString s){return s.replace("'","''");}
    QStringList splitDate(QString d){return d.split("-");}
    QString getUserKey(QString name);
    QString getCourseKey(QString term, QString title, INT num);
    QString getInstructor(QString courseKey);
    bool createTask(QString user, QString TAKey, QString courseKey,QString desc, QString start,QString end);
    bool editTask(INT TaskID, QString desc, QString start, QString end);
    bool deleteTask(INT TaskID);
};

#endif // STORAGE_H

#ifndef PERSISTIMPSQL_H
#define PERSISTIMPSQL_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QVariant>
#include <QSqlDriver>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QStringList>
#include "../Storage/PersistImp.h"

class PersistImpSQL: public PersistImp{
public:

    PersistImpSQL(){
        connected = false;
    }

    /* connect and disconnect */
    bool connectToDB(QString connection, QString path, QString DBType);
    void disconnect();

    bool restore(QString path);


    /* verifyUser */
    bool verifyUser(QString name, QString &role);

    /* actions: create, edit, delete*/
    /* manage Tasks */
    bool manageTask(QString user, QString action, Task task);

    /* get courses teaching need delete *list after use */
    bool getCourses(QString user, QString term, QList<Course> *& list, QString role);

    /* get TAs for a course need delete *list after use */
    bool getTAsForCourse(QString courseKey, QList<TA> *&list);

    /* get Tasks for a TA and course need delete *list after use */
    bool getTasksForTA(QString info, QString TAKey, QList<Task> *&list, QString role);

    /* get Semesters for a given user */
    QStringList getSemesters(QString user);

    /* get a Task based on TaskID */
    bool getTask(QString taskID, Task *& rv);

    /* enterEvaluation */
    bool enterEvaluation(INT TaskID,INT rating, QString feedback);
private:
    QSqlDatabase db; /* DataBase */
    bool connected; /* flag if connected to DB */
    void fixNull(QString &s);
    QString fixEscape(QString s){return s.replace("'","''");}
    QStringList splitDate(QString d){return d.split("-");}
    QString getUserKey(QString name);
    QString getUserType(QString name);
    QString getCourseKey(QString term, QString title, INT num);
    QString getInstructor(QString courseKey);
    bool createTask(QString user, QString TAKey, QString courseKey,QString desc, QString start,QString end);
    bool editTask(INT TaskID, QString desc, QString start, QString end);
    bool deleteTask(INT TaskID);
};

#endif // PERSISTIMPSQL_H

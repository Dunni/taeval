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
#include "PersistImp.h"

class PersistImpSQL: public PersistImp{
public:

    PersistImpSQL(){
        connected = false;
    }

    /* connect and disconnect */
    virtual bool connectToDB(QString connection, QString path = QDir::toNativeSeparators("Storage/taeval.db"), QString DBType = "QSQLITE");
    virtual void disconnect();

    virtual bool restore(QString path = QDir::toNativeSeparators("Storage/taeval.sql"));


    /* verifyUser */
    virtual bool verifyUser(QString name, QString &role);

    /* actions: create, edit, delete*/
    /* manage Tasks */
    virtual  bool manageTask(QString user, QString action, Task task);

    /* get courses teaching need delete *list after use */
    virtual bool getCoursesTeaching(QString Instrcutor, QString term, QList<Course> *& list);

    /* get TAs for a course need delete *list after use */
    virtual bool getTAsForCourse(QString courseKey, QList<TA> *&list);

    /* get Tasks for a TA and course need delete *list after use */
    virtual bool getTasksForTA(QString courseKey, QString TAKey, QList<Task> *&list);

    /* get Semesters for a given instructor */
    virtual QStringList getSemesters(QString instrcutor);

    /* get a Task based on TaskID */
    virtual bool getTask(QString taskID, Task *& rv);

    /* enterEvaluation */
    virtual bool enterEvaluation(INT TaskID,INT rating, QString feedback);
private:
    QSqlDatabase db; /* DataBase */
    bool connected; /* flag if connected to DB */
    void fixNull(QString &s);
    QString fixEscape(QString s){return s.replace("'","''");}
    QStringList splitDate(QString d){return d.split("-");}
    QString getUserKey(QString name);
    QString getCourseKey(QString term, QString title, INT num);
    QString getInstructor(QString courseKey);
    virtual bool createTask(QString user, QString TAKey, QString courseKey,QString desc, QString start,QString end);
    virtual bool editTask(INT TaskID, QString desc, QString start, QString end);
    virtual bool deleteTask(INT TaskID);
};

#endif // PERSISTIMPSQL_H

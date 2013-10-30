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

typedef qint32 INT;

class Storage{
public:
    /* Contructor */
    Storage(){ connected = false; }

    /* connect and disconnect */
    bool connectToDB(QString connection, QString path = "Storage//taeval.db", QString DBType = "QSQLITE");
    void disconnect();

    /* manage Tasks */
    bool manageTask();

    /* view courses teaching*/
    //???

    /* viewTAsforCourse */
    //???

    /* Debug related */
#ifdef DEBUG
    bool dumpDB();
#endif

#ifndef DEBUG
private:
#endif
    QSqlDatabase db; /* DataBase */
    bool connected; /* flag if connected to DB */
    QString fixEscape(QString s){return s.replace("'","''");}
    QString getUserKey(QString name);
    QString getCourseKey(QString term, QString title, INT num);
    bool createTask(QString TAName, QString courseTitle, QString courseTerm, INT courseNum, QString desc, QString start, QString end);
    bool editTask(INT TaskID, QString desc, QString start, QString end);
    bool deleteTask(INT TaskID);
    bool viewCoursesTeaching(QString Instrcutor, QString term);
    bool viewTAsforCourse(QString term, QString title, INT num);
    bool viewTasksforTA(QString term, QString title, INT num,QString TAKey);
    bool enterEvaluation(INT TaskID,INT rating, QString feedback);
};

#endif // STORAGE_H

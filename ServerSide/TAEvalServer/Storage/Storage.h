#ifndef STORAGE_H
#define STORAGE_H

#define DEBUG

#include "PersistImpSQL.h"

class Storage{
public:
    /* get Instance of Storage*/
    static Storage& getInstance(){
        static Storage s;
        return s;
    }

    /* connect and disconnect */
    bool connectToDB(QString connection, QString path = QDir::toNativeSeparators("Storage/taeval.db"), QString DBType = "QSQLITE");
    void disconnect();

    /* restore database */
    bool restore(QString path = QDir::toNativeSeparators("Storage/taeval.sql"));

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

    /* get Semesters for a given user */
    QStringList getSemesters(QString user);

    /* get a Task based on TaskID */
    bool getTask(QString taskID, Task *& rv);

    /* enterEvaluation */
    bool enterEvaluation(INT TaskID,INT rating, QString feedback);

    /* sendRequest for debug*/
    bool sendRequest(QString cmd);


#ifndef DEBUG
private:
#endif
    /* Contructor */
    Storage(){
        Imp = new PersistImpSQL();
    }
    ~Storage(){
        delete Imp;
    }
    Storage(Storage const&);
    void operator=(Storage const &);

    PersistImp * Imp;
};

#endif // STORAGE_H

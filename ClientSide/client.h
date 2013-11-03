#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QTcpSocket>
#include <string.h>
#include "../Common/course.h"
#include "../Common/task.h"
#include "../Common/nonadminuser.h"
#include "../Common/ta.h"

typedef QList<TA> TAList;
typedef QList<Course> CourseList;
typedef QList<Task> TaskList;
typedef QList<QString> StringList;

using namespace std;

class Client : public QObject
{
    Q_OBJECT

public:
    explicit Client(QObject *parent = 0);
    QTcpSocket *tcpSocket;
    friend class MainScreen;
    QString userNameString;
    QString semesterString;
    QString courseString;
    QString taString;
    QString taskString;
    QString evalString;
    QString currentTaskID;
    QString feedback;
    QString rating;
    QDate begDate, dueDate, begDateEd, dueDateEd;
    QString duty;

    //Parsers
    StringList stringToList(QString);
    QString listToString(StringList);

private:
    CourseList *c;
    TaskList taskList;
    string sendLoginRequest(string);
    QString sendSemesterRequest(string);
    QString sendCoursesRequest(string);
    QString sendTAsRequest(string);
    QString sendTaskRequest(string);
    QString sendCreateTaskRequest(string);
    QString sendEditEvalRequest(string);
    QString sendEditTaskRequest(string);
    QString sendDeleteTaskRequest(string);

};

#endif

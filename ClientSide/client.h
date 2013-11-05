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
    QString sendRequest(QString, QString);
};

#endif

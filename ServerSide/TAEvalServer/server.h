 #ifndef SERVER_H
 #define SERVER_H

 #include <QObject>
 #include <QDebug>
 #include <QtNetwork>
 #include <QMessageBox>
 #include <stdlib.h>
 #include "taeval.h"

 class Server : public QObject
 {
     Q_OBJECT

 public:
    explicit Server(QObject *parent = 0);

 public slots:
     void goToNewConnection();
     void readyToRead();
     void goToDisconnected();

 private:
     QTcpServer *tcpServer;
     QTcpSocket *tcpSocket;
     TAEval model;
     CourseList *c;
     TAList *t;
     TaskList *taskList;
     void loginRequest(QString);
     void semesterRequest(QString);
     void coursesRequest(QString);
     void tasRequest(QString);
     void taskRequest(QString);
 };

 #endif

#ifndef SERVERCONNECTION_H
#define SERVERCONNECTION_H
#include <QObject>
#include <QDebug>
#include <QtNetwork>
#include <QMessageBox>
#include <stdlib.h>
#include <QTcpServer>

class Server : public QObject
{
    Q_OBJECT

public:
   explicit Server(QObject *parent = 0);

   QString getRequest();
   void sendDataBack(QString data);

//   QTcpSocket *tcpSocket;

public slots:
    void goToNewConnection();
//    void goToDisconnected();


private:
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;
    bool takeNext;
signals:
    void getMsg();


};

#endif // SERVERCONNECTION_H

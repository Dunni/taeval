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

public slots:
    void goToNewConnection();

private:
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;
    QHostAddress  ipAddress;
    int port;
    bool takeNext;
signals:
    void getMsg();


};

#endif // SERVERCONNECTION_H

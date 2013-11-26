#include "serverconnection.h"

Server::Server(QObject *parent) :  QObject(parent), tcpServer(0)
{
    tcpServer = new QTcpServer(this);
    takeNext = true;
    tcpServer->setMaxPendingConnections(100);
    QHostAddress ipAddress = QHostAddress("192.168.43.22");

    qDebug()<< tcpServer->maxPendingConnections() << endl;

    if(tcpServer->listen(ipAddress,2000))//QHostAddress::LocalHost, 2000))
    {
        qDebug() << "Server connected locally and to port 2000" << endl;
    }
    else
    {
        qDebug() << "Server could not connect locally and to port 2000" << endl;
    }

    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(goToNewConnection()));
}

 void Server::goToNewConnection()
 {
     if(takeNext)
     {
     tcpSocket = tcpServer->nextPendingConnection();
     takeNext = false;

     qDebug() << "new connection established" << endl;

     if(tcpSocket->state() == QTcpSocket::ConnectedState)
     {
        qDebug() << "Client is connected now!" <<endl;
       tcpSocket->flush(); //To clear the socket
     }
     else
     {
         qDebug() << "socket failed" << endl;
     }

     if(tcpSocket->waitForReadyRead())
     {
        emit getMsg();
     }
     }
     else
     {
         qDebug() << "Wait till previous request is done" << endl;
     }




 }

QString Server::getRequest()
{
    QString receivedInfo;
    receivedInfo = tcpSocket->readLine().constData();
    qDebug() << "receivedInfo: " << receivedInfo << endl;
    return receivedInfo;

}

void Server::sendDataBack(QString data)
{
    tcpSocket->write(data.toUtf8());
    tcpSocket->disconnectFromHost();
    takeNext = true;
}

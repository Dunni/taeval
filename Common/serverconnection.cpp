#include "serverconnection.h"

Server::Server(QObject *parent) :  QObject(parent), tcpServer(0)
{
    tcpServer = new QTcpServer(this);
    takeNext = true;
    tcpServer->setMaxPendingConnections(100);

    /*Get info from the config file*/
    QSettings *config = new QSettings("../../Common/config.ini",QSettings::IniFormat);
    config->setIniCodec("UTF8");
    config->beginGroup("information");
    QString ip=config->value("ServerIp").toString();
    int portNumber = config->value("portNumber").toInt();
    ipAddress = QHostAddress(QHostAddress::LocalHost);
    port = 2001;
    config->endGroup();
    qDebug()<< tcpServer->maxPendingConnections() << endl;
    if(tcpServer->listen(ipAddress,port))
    {
        qDebug() << ipAddress << portNumber << endl;
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

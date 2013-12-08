#include "serverconnection.h"

ServerConnection::ServerConnection(QObject *parent) :  QObject(parent), tcpServer(0)
{
    tcpServer = new QTcpServer(this);
    takeNext = true;
    tcpServer->setMaxPendingConnections(100);

    /*Get info from the config file*/
    QSettings *config = new QSettings("../Common/Communication/config.ini",QSettings::IniFormat);
    config->setIniCodec("UTF8");
    config->beginGroup("information");
    QString ip=config->value("ServerIp").toString();
    int portNumber = config->value("portNumber").toInt();
    ipAddress = QHostAddress(ip);
    port = portNumber;
    config->endGroup();
    qDebug()<< tcpServer->maxPendingConnections() << endl;
    if(tcpServer->listen(ipAddress,port))
    {
        qDebug() << ipAddress << portNumber << endl;
    }
    else
    {
        qDebug() << "Server could not listen" << endl;
    }

    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(goToNewConnection()));
}

ServerConnection::~ServerConnection(){
    delete tcpServer;
    delete tcpSocket;
}

 void ServerConnection::goToNewConnection()
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

QString ServerConnection::getRequest()
{
    QString receivedInfo;
    receivedInfo = tcpSocket->readLine().constData();
    qDebug() << "receivedInfo: " << receivedInfo << endl;
    return receivedInfo;

}

void ServerConnection::sendDataBack(QString data)
{
    tcpSocket->write(data.toUtf8());
    tcpSocket->disconnectFromHost();
    takeNext = true;
}

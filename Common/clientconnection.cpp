#include "clientconnection.h"

Client::Client(QObject *parent) :
    QObject(parent)
{
    tcpSocket = new QTcpSocket(this);
    /*Get info from the config file*/
    QSettings *config = new QSettings("../Common/config.ini",QSettings::IniFormat);
    config->setIniCodec("UTF8");
    config->beginGroup("information");
    QString ip=config->value("HostAddress").toString();
    int portNumber = config->value("portNumber").toInt();
    ipAddress = QHostAddress(QHostAddress::LocalHost);
    port = 2001;
    config->endGroup();

    connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(errorReport()));

}

QString Client::sendRequest(QString request, QString data)
{
    tcpSocket->abort();
    tcpSocket->connectToHost(ipAddress, port);
    qDebug()<<"Sending request" << endl;
    if(!tcpSocket->waitForConnected())
    {
        qDebug() << "gg" <<endl;
    }
    request.append(QString(data));

    tcpSocket->write(request.toUtf8());
    qDebug()<<"Request sent" << endl;
    if(!tcpSocket->waitForReadyRead())
    {
        qDebug() << request << endl;
        qDebug() << "read gg" <<endl;


    }

    QString reply = tr(tcpSocket->readAll().constData());

    qDebug() << reply << endl;

    return reply;

}

 void Client::errorReport()
{
     //because we are put all the requests in the pending list and handle them
     //one by one, so we will tolerate the RemoteHostClosedError
     if(tcpSocket->error() != QAbstractSocket::RemoteHostClosedError)
     {
        emit errorOccurs();
     }
}

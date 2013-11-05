#include "client.h"

Client::Client(QObject *parent) :
    QObject(parent)
{
    tcpSocket = new QTcpSocket(this);
    tcpSocket->connectToHost(QHostAddress::LocalHost, 2000);
}

QString Client::sendRequest(QString request, QString data){

    request.append(QString(data));

    tcpSocket->write(request.toUtf8());

    tcpSocket->waitForReadyRead();
    QString reply = tr(tcpSocket->readAll().constData());

    qDebug() << reply << endl;

    return reply;

}

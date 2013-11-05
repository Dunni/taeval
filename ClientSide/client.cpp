#include <QtGui>
#include <QtNetwork>
#include "client.h"
#include "mainscreen.h"

Client::Client(QObject *parent) :
    QObject(parent)
{
    tcpSocket = new QTcpSocket(this);
    tcpSocket->connectToHost(QHostAddress::LocalHost, 2000);
}

//QString Client::sendTaskRequest(string str){

//    QString sendRequest = "taskRequest|";

//    sendRequest.append(QString(str.c_str()));

//    tcpSocket->write(sendRequest.toUtf8());

//    tcpSocket->waitForReadyRead();
//    QString taskString = tr(tcpSocket->readAll().constData());

//    qDebug() << taskString << endl;

//    return taskString;

//}

//QString Client::sendCreateTaskRequest(string str){

//    QString sendRequest = "taskCreateRequest|";

//    sendRequest.append(QString(str.c_str()));

//    tcpSocket->write(sendRequest.toUtf8());

//    tcpSocket->waitForReadyRead();
//    QString taskString = tr(tcpSocket->readAll().constData());

//    qDebug() << taskString << endl;

//    return taskString;

//}


//QString Client::sendEditEvalRequest(string str){

//    QString sendRequest = "editEvalRequest|";

//    sendRequest.append(QString(str.c_str()));

//    tcpSocket->write(sendRequest.toUtf8());

//    tcpSocket->waitForReadyRead();
//    QString taskString = tr(tcpSocket->readAll().constData());

//    qDebug() << taskString << endl;

//    return taskString;

//}


QString Client::sendRequest(QString request, QString data){

    request.append(QString(data));

    tcpSocket->write(request.toUtf8());

    tcpSocket->waitForReadyRead();
    QString reply = tr(tcpSocket->readAll().constData());

    qDebug() << reply << endl;

    return reply;

}

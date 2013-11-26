//#include "client.h"

//Client::Client(QObject *parent) :
//    QObject(parent)
//{

//    tcpSocket = new QTcpSocket(this);
//}

//QString Client::sendRequest(QString request, QString data)
//{
//    tcpSocket = new QTcpSocket(this);
//    tcpSocket->connectToHost(QHostAddress::LocalHost, 2000);
//    if(!tcpSocket->waitForConnected())
//    {
//        qDebug() << "gg" <<endl;
//    }
//    request.append(QString(data));

//    tcpSocket->write(request.toUtf8());

//    if(!tcpSocket->waitForReadyRead())
//    {
//        qDebug() << request << endl;
//        qDebug() << "read gg" <<endl;
//    }

//    QString reply = tr(tcpSocket->readAll().constData());

//    tcpSocket->disconnectFromHost();
//    tcpSocket->deleteLater();

//    qDebug() << reply << endl;

//    return reply;

//}

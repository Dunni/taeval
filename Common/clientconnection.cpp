#include "clientconnection.h"

Client::Client(QObject *parent) :
    QObject(parent)
{
    tcpSocket = new QTcpSocket(this);
    ipAddress = QHostAddress("172.20.10.6");


}

QString Client::sendRequest(QString request, QString data)
{
    tcpSocket->abort();
    tcpSocket->connectToHost(ipAddress, 2000);
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
//QString Client::errorReport(){

//    if(tcpSocket->error() == QAbstractSocket::ConnectionRefusedError){
//        QMessageBox::critical(this,
//                              tr("Connection Error!"),
//                              tr("The server is not running."));
//    } else if (tcpSocket->error() == QAbstractSocket::RemoteHostClosedError){
//        QMessageBox::critical(this,
//                              tr("Connection Error!"),
//                              tr("Server closed the connection."));
//    } else {
//        QMessageBox::critical(this,
//                              tr("Connection Error!"),
//                              tr("Complex error occurred, try connecting again."));
//    }
//    this->close();

//}

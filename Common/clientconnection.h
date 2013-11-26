#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QtNetwork>

class Client : public QObject
{
    Q_OBJECT

public:
    explicit Client(QObject *parent = 0);
    QString sendRequest(QString, QString);
private:
    QHostAddress ipAddress;
    QTcpSocket *tcpSocket;

};

#endif // CLIENTCONNECTION_H

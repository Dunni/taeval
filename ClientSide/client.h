#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QtNetwork>

class Client : public QObject
{
    Q_OBJECT

public:
    explicit Client(QObject *parent = 0);
    QTcpSocket *tcpSocket;
    QString sendRequest(QString, QString);
};

#endif

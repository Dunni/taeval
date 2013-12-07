#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QtNetwork>

class ClientConnection : public QObject
{
    Q_OBJECT

public:
    explicit ClientConnection(QObject *parent = 0);
    QString sendRequest(QString, QString);
    ~ClientConnection();

private slots:
        void errorReport();
private:
    QHostAddress ipAddress;
    int          port;
    QTcpSocket   *tcpSocket;

signals:
    //Use this signal if you want to handle the connection error on the
    //GUI side.
    void errorOccurs();


};

#endif // CLIENTCONNECTION_H

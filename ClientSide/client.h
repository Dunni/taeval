#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QTcpSocket>
#include "instructorscreen.h"

namespace Ui {
class Client;
}

class Client : public QMainWindow
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = 0);
    QTcpSocket *tcpSocket;
    ~Client();

private slots:
    void sendRequest();
    void loginClicked();
    void goToReadyRead();

private:
    Ui::Client *ui;
    InstructorScreen *is;

    quint16 blockSize;
};

#endif

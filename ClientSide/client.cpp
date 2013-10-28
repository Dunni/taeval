#include <QtGui>
#include <QtNetwork>
#include "client.h"
#include "ui_client.h"

Client::Client(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::Client)
{
    ui->setupUi(this);
    tcpSocket = new QTcpSocket(this);
    is = new InstructorScreen();

    //For the future
    //tas = new TAScreen();
    //as = new AdminScreen();

    tcpSocket->connectToHost(QHostAddress::LocalHost, 2000);

    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(goToReadyRead()));
    connect(ui->loginButton, SIGNAL(clicked()), this, SLOT(loginClicked())) ;

    setWindowTitle(tr("TAEval"));
}

Client::~Client()
{
    delete ui;
}

void Client::sendRequest()
{
//    qDebug() << tcpSocket->readLine() << endl;
}

void Client::goToReadyRead()
{
    if(tcpSocket->readLine().toInt() == 1000){
        this->hide();
        is->show();
    } else {
        QMessageBox::information(this, tr("TAEval"), tr("The username is invalid! Please try again."));
    }

}

void Client::loginClicked()
{
    std::string s = ui->userName->text().toStdString();
    tcpSocket->write(s.c_str());
}

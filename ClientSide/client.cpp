#include <QtGui>
#include <QtNetwork>
#include "client.h"
#include "../Common/course.h"
#include "mainscreen.h"


Client::Client(QObject *parent) :
    QObject(parent)
{
    tcpSocket = new QTcpSocket(this);
    tcpSocket->connectToHost(QHostAddress::LocalHost, 2000);
}

string Client::sendLoginRequest(string str){

    QString sendRequest = "loginRequest|";

    sendRequest.append(QString(str.c_str()));

    tcpSocket->write(sendRequest.toUtf8());
    //To synchronize the reads, it will wait for 30 seconds until there's a reply.
    tcpSocket->waitForReadyRead();

    QString successFail = tr(tcpSocket->readLine().constData());

    if(successFail.compare("true") == 0){
        qDebug() << "Retrieving user name " << endl;
        return "true";
    } else {
        qDebug() << "It failed! :'(" << endl;
        return "false";
    }

    return "false";
}

QString Client::sendSemesterRequest(string str){

    QString sendRequest = "semesterRequest|";

    sendRequest.append(QString(str.c_str()));

    tcpSocket->write(sendRequest.toUtf8());
    //To synchronize the reads, it will wait for 30 seconds until there's a reply.
    tcpSocket->waitForReadyRead();

    semesterString = tr(tcpSocket->readLine().constData());

    qDebug() << semesterString << endl;

    return semesterString;

}

QString Client::sendCoursesRequest(string str){

    QString sendRequest = "coursesRequest|";

    sendRequest.append(QString(str.c_str()));

    tcpSocket->write(sendRequest.toUtf8());

    tcpSocket->waitForReadyRead();
    QString coursesString = tr(tcpSocket->readAll().constData());

    qDebug() << coursesString << endl;

    return coursesString;

}

QString Client::sendTAsRequest(string str){

    QString sendRequest = "tasRequest|";

    sendRequest.append(QString(str.c_str()));

    tcpSocket->write(sendRequest.toUtf8());

    tcpSocket->waitForReadyRead();
    QString tasString = tr(tcpSocket->readAll().constData());

    qDebug() << tasString << endl;

    return tasString;

}

QString Client::sendTaskRequest(string str){

    QString sendRequest = "taskRequest|";

    sendRequest.append(QString(str.c_str()));

    tcpSocket->write(sendRequest.toUtf8());

    tcpSocket->waitForReadyRead();
    QString taskString = tr(tcpSocket->readAll().constData());

    qDebug() << taskString << endl;

    return taskString;

}


//Converters
QString Client::listToString(StringList list){ // Parse a list of QStrings to a single QString
    QString result = QString("");
    for(int i=0; i<list.length();i++)
    {
        if(i==0) result += "~`";
        result += list[i];
        if(i==list.length()-1) result += "`~";
        else result += "~~";
    }
    return result;
}

StringList Client::stringToList(QString aString){  // Convert a single QString to a list of QStrings
    StringList result = StringList();
    QList<QString> list = aString.split(QRegExp("(~`|`~|~~)"));
    for(int i=1;i<list.length()-1;i++) result += list[i];
    //qDebug() << list;
    return result;
}

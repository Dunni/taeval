#include "server.h"
#include "stdio.h"

Server::Server(QObject *parent)
    :QObject(parent)
{
    s = new ServerConnection();
    connect(s,SIGNAL(getMsg()),this,SLOT(getRequest()));
}

Server::~Server()
{
    delete s;
}

void Server::getRequest()
{
    QString receivedInfo = s->getRequest();

    QStringList infoSplit = receivedInfo.split("|");
    QString request = infoSplit.at(0);
    QString data;
    for(int i = 1; i < infoSplit.length(); i++){
       data.append(infoSplit.at(i));
       if(!(i == infoSplit.length()-1)){   //If the last one then don't add the delimiter
           data.append("|");
       }
    }

    s->sendDataBack(model.serveRequest(request,data).toUtf8());
}

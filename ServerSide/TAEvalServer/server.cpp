#include "server.h"
#include "stdio.h"

ServerApp::ServerApp(QObject *parent)
    :QObject(parent)
{
//    connectionService = new Connection();
//    connectionService->runServer();
    s = new Server();
    connect(s,SIGNAL(getMsg()),this,SLOT(getRequest()));


}

void ServerApp::getRequest()
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

    qDebug() << "Client says to Server: " << request << " and " << data << endl;


//    connectionService
    s->sendDataBack(model.serveRequest(request,data).toUtf8());
//    qDebug() << "What is thiss" << model.serveRequest(request,data) << endl;
}

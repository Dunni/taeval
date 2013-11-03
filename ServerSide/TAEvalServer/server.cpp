 #include "server.h"

 Server::Server(QObject *parent) :  QObject(parent), tcpServer(0)
 {
     tcpServer = new QTcpServer(this);

     if(tcpServer->listen(QHostAddress::LocalHost, 2000)){
         qDebug() << "Server connected locally and to port 2000" << endl;
     } else {
         qDebug() << "Server could not connect locally and to port 2000" << endl;
     }

     connect(tcpServer, SIGNAL(newConnection()), this, SLOT(goToNewConnection()));
 }

 void Server::goToNewConnection() {

     tcpSocket = tcpServer->nextPendingConnection();

     if(tcpSocket->state() == QTcpSocket::ConnectedState){
         qDebug() << "Client is connected now!" <<endl;
        tcpSocket->flush(); //To clear the socket
     }

     //Set up the signals for the new connection
     connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(goToDisconnected()));
     connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readyToRead()));
 }

 void Server::goToDisconnected()
 {
     printf("Connection disconnected.\n");
     disconnect(tcpSocket, SIGNAL(disconnected()));
     disconnect(tcpSocket, SIGNAL(readyRead()));
     tcpSocket->deleteLater();
 }


 void Server::readyToRead()
 {
     //Delimit the data here into two QStrings so to separate the requestID and data
     //associated with the request.
     //NOTE: If needed later, do if statement to see if the received info is 1 or 2 or 3 and act accordingly
     QString receivedInfo = tcpSocket->readLine().constData();
     QStringList infoSplit = receivedInfo.split("|");


     //First, read the type of request from the client.
     QString request = infoSplit.at(0);

     //Then read the data that's associated with the request
     QString data;

     //Put back the rest of the data with the delimiter.
     for(int i = 1; i < infoSplit.length(); i++){
        data.append(infoSplit.at(i));
        if(!(i == infoSplit.length()-1)){   //If the last one then don't add the delimiter
            data.append("|");
        }
     }

     qDebug() << "Client says to Server: " << request << " and " << data << endl;

     tcpSocket->write(model.serveRequest(request,data).toUtf8());


 }



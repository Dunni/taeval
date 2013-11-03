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


     //Then find the request here and delegate the data to the SQL to query the required info.
     if(request.compare("loginRequest") == 0){
        loginRequest(data);
     } else if (request.compare("semesterRequest") == 0) {
        semesterRequest(data);
     } else if (request.compare("coursesRequest") == 0) {
        coursesRequest(data);
     } else if (request.compare("tasRequest") == 0) {
        tasRequest(data);
     } else if (request.compare("taskRequest") == 0) {
        taskRequest(data);
     } else if (request.compare("taskCreateRequest") == 0) {
        createTaskRequest(data);
     } else if (request.compare("taskEditRequest") == 0) {
        editTaskRequest(data);
     } else if (request.compare("taskDeleteRequest") == 0){
        deleteTaskRequest(data);
     } else if (request.compare("editEvalRequest") == 0){
        editEvalRequest(data);
     }else {
         qDebug() << "Request will not be made" << endl;
     }

 }

 void Server::loginRequest(QString username){

     qDebug() << "Login Request being processed" << endl;

     if(model.logIn(username)){
         tcpSocket->write("true");
     } else {
        tcpSocket->write("false");
     }

 }

 void Server::semesterRequest(QString username){

     qDebug() << "Semester Request being processed" << endl;

     StringList *semesters;

     model.getSemesters(username, semesters);

     tcpSocket->write(model.listToString(*semesters).toUtf8());

 }

 void Server::coursesRequest(QString usernameSemester){

     qDebug() << "Courses Request being processed" << endl;

     QStringList info = usernameSemester.split("|");

     qDebug() <<  info.at(0) + " Semester is " + info.at(1) << endl;

     model.getCourses(info.at(0), c, info.at(0), info.at(1));
     QString stringOfCourses = Course::listToString(*c);

     qDebug() << stringOfCourses << endl;

     tcpSocket->write(stringOfCourses.toUtf8());

 }

 void Server::tasRequest(QString taKey){

     qDebug() << "TAs Request being processed" << endl;

     QStringList info = taKey.split("|");

     qDebug() << "Inst here is: " + info.at(0) + " and " +
                 info.at(1) << endl;

     model.getTAs(info.at(0), info.at(1), t);
     QString stringOfTAs = TA::listToString(*t);

     qDebug() << stringOfTAs << endl;

     if(stringOfTAs.isEmpty()){
         tcpSocket->write("<empty>");
     } else {
        tcpSocket->write(stringOfTAs.toUtf8());
     }

 }

 void Server::taskRequest(QString courseKeyTaKey){

     qDebug() << "Task Request being processed" << endl;

     QStringList info = courseKeyTaKey.split("|");

     qDebug() << "Inst here is: " + info.at(0) + " and " +
                 info.at(1) << " and " << info.at(2) << endl;

     model.getTasks(info.at(0), info.at(1), info.at(2), taskList);
     QString stringOfTask = Task::listToString(*taskList);

     qDebug() << stringOfTask << endl;

     if(stringOfTask.isEmpty()){
         tcpSocket->write("<empty>");
     } else {
        tcpSocket->write(stringOfTask.toUtf8());
     }

 }


 void Server::createTaskRequest(QString instTaKeyCourseKeyPlusData){

     qDebug() << "CREATE Task Request being processed" << endl;

     QStringList info = instTaKeyCourseKeyPlusData.split("|");

     qDebug() << "Inst here is: " + info.at(0) + " and " +
                 info.at(1) << " and " << info.at(2) + " and " +
              info.at(3) << " and " << info.at(4) << + " and " +
              info.at(5) << endl;

     QStringList begDateList = info.at(4).split("-");
     QStringList dueDateList = info.at(5).split("-");

     QDate begDate(begDateList.at(0).toInt(), begDateList.at(1).toInt(), begDateList.at(2).toInt());
     QDate dueDate(dueDateList.at(0).toInt(), dueDateList.at(1).toInt(), dueDateList.at(2).toInt());


     bool success = model.createTask(info.at(0), info.at(1), info.at(2), info.at(3), begDate, dueDate);


//     Task task1(info.at(1), info.at(2), info.at(3), info.at(4), info.at(5));
//     QString stringOfTask = task1.toString();

//     qDebug() << stringOfTask << endl;

     if(!success){
         tcpSocket->write("FAIL!");
     } else {
         tcpSocket->write("SUCCESS!");
     }

 }


 void Server::editEvalRequest(QString instTaKeyCourseKeyPlusData){

     qDebug() << "Edit Evail Request being processed" << endl;

     QStringList info = instTaKeyCourseKeyPlusData.split("|");

     qDebug() << "Inst here is: " + info.at(0) + " and " +
                 info.at(1) << endl;

     bool success = model.enterEvaluation(info.at(0), info.at(1).toInt(), info.at(2), info.at(3).toInt());


//     Task task1(info.at(1), info.at(2), info.at(3), info.at(4), info.at(5));
//     QString stringOfTask = task1.toString();

//     qDebug() << stringOfTask << endl;

     if(!success){
         tcpSocket->write("FAIL!");
     } else {
         tcpSocket->write("SUCCESS!");
     }

 }

 void Server::editTaskRequest(QString instTaKeyCourseKeyPlusData){

     qDebug() << "Edit Task Request being processed" << endl;

     QStringList info = instTaKeyCourseKeyPlusData.split("|");

     qDebug() << "Inst here is: " + info.at(0) + " and " +
                 info.at(1) + " and " + info.at(2) + " and " +
                 info.at(3) + "and " + info.at(4) << endl;

     QStringList begDateList = info.at(2).split("-");
     QStringList dueDateList = info.at(3).split("-");

     QDate begDate(begDateList.at(0).toInt(), begDateList.at(1).toInt(), begDateList.at(2).toInt());
     QDate dueDate(dueDateList.at(0).toInt(), dueDateList.at(1).toInt(), dueDateList.at(2).toInt());


     bool success = model.editTask(info.at(0), info.at(1), begDate, dueDate, QString(info.at(4)));

     if(!success){
         tcpSocket->write("FAIL!");
     } else {
         tcpSocket->write("SUCCESS!");
     }

 }

 void Server::deleteTaskRequest(QString instTaKeyCourseKeyPlusData){

     qDebug() << "DELETE Task Request being processed" << endl;

     QStringList info = instTaKeyCourseKeyPlusData.split("|");

     qDebug() << "Inst here is: " + info.at(0) + " and " +
                 info.at(1) << endl;

     bool success = model.deleteTask(info.at(0), QString(info.at(1)));

     if(!success){
         tcpSocket->write("FAIL!");
     } else {
         tcpSocket->write("SUCCESS!");
     }

 }

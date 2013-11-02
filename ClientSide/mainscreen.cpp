#include "mainscreen.h"
#include "ui_mainscreen.h"

MainScreen::MainScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainScreen)
{
    ui->setupUi(this);

    //Initialize client here ...
    c = new Client();

    //Connect the socket to a signal for error in connection and send a clear message to user what the error is.
    connect(c->tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(sendError()));

    //Connect all the buttons to a slot in order to apply the appropriate response
    connect(ui->loginButton, SIGNAL(clicked()), this, SLOT(instructorPage()));
    connect(ui->viewCoursesButton, SIGNAL(clicked()), this, SLOT(viewCoursesPage()));
    connect(ui->viewTAs, SIGNAL(clicked()), this, SLOT(viewTAsPage()));
    connect(ui->viewTask, SIGNAL(clicked()), this, SLOT(viewTaskPage()));
    connect(ui->backToCoursesPage, SIGNAL(clicked()), this, SLOT(viewCoursesPage()));
    connect(ui->backToSemesterButton, SIGNAL(clicked()), this, SLOT(viewSemesterPage()));
    connect(ui->backToTAsPage, SIGNAL(clicked()), this, SLOT(viewTAsPage()));
    connect(ui->logOutButton, SIGNAL(clicked()), this, SLOT(viewLoginScreen()));
    connect(ui->viewTask, SIGNAL(clicked()), this, SLOT(viewTaskPage()));
    connect(ui->viewEvaluation, SIGNAL(clicked()), this, SLOT(viewEvaluationPage()));
    connect(ui->createTaskButton, SIGNAL(clicked()), this, SLOT(viewCreateTaskPage()));
    connect(ui->editTaskButton, SIGNAL(clicked()), this, SLOT(viewEditTaskPage()));

    //For these ones, might have to add new functions because I have to save/cancel
    connect(ui->evalSave, SIGNAL(clicked()), this, SLOT(viewTaskPage()));
    connect(ui->evalCancel, SIGNAL(clicked()), this, SLOT(viewTaskPage()));
    connect(ui->taskSave, SIGNAL(clicked()), this, SLOT(viewTaskPage()));
    connect(ui->taskCancel, SIGNAL(clicked()), this, SLOT(viewTaskPage()));
    connect(ui->editTaskSave, SIGNAL(clicked()), this, SLOT(viewTaskPage()));
    connect(ui->editTaskCancel, SIGNAL(clicked()), this, SLOT(viewTaskPage()));

    //Stuff
//    connect(ui->semesterBox, SIGNAL(currentIndexChanged(int)), this, SLOT(storeSemester(int)));
}

MainScreen::~MainScreen()
{
    delete ui;
}

void MainScreen::sendError(){

    if(c->tcpSocket->error() == QAbstractSocket::ConnectionRefusedError){
        QMessageBox::critical(this,
                              tr("Connection Error!"),
                              tr("The server is not running."));
    } else if (c->tcpSocket->error() == QAbstractSocket::RemoteHostClosedError){
        QMessageBox::critical(this,
                              tr("Connection Error!"),
                              tr("Server closed the connection."));
    } else {
        QMessageBox::critical(this,
                              tr("Connection Error!"),
                              tr("Complex error occurred, try connecting again."));
    }
    this->close();

}

void MainScreen::viewLoginScreen(){

    qDebug() << "Viewing Login Screen" << endl;

    //Clear username textfield
    ui->userName->clear();

    //Then go to the login page
    ui->MainWidget->setCurrentIndex(0);

}

void MainScreen::instructorPage()
{
    //Username textfield can't be empty!
    if(!ui->userName->text().isEmpty()){
        qDebug() << "Viewing Instructor" << endl;

        //Send a login request with the entered username
        string success = c->sendLoginRequest(ui->userName->text().toStdString());

        //Check if the login request was successful otherwise send an error message saying user doesn't exist.
        if(success.compare("true") == 0){

            //Change to the instructor page and call the semester page
            ui->MainWidget->setCurrentIndex(1);
            viewSemesterPage();

        } else {
            QMessageBox::critical(this, tr("TAEval"), tr("The username is invalid! Please try again."));
        }
    } else {
        QMessageBox::critical(this, tr("TAEval"), tr("The username is blank! Please enter an username."));
    }
}

void MainScreen::viewSemesterPage()
{
    qDebug() << "Viewing Semesters" << endl;

    //Reset all data on the semester page
    ui->semesterBox->clear();
    ui->semesterBox->addItem("<Please select one>");

    //Request the list of semesters
    QString semester = (c->sendSemesterRequest(ui->userName->text().toStdString()));

    //Split the string with a delimiter
    StringList semesterList = c->stringToList(semester);

    //Load the all of the relevant information onto the page
    ui->nameLabel->setText(ui->userName->text());
    ui->semesterBox->addItems(semesterList);

    //Turn to the semester page
    ui->InfoWidget->setCurrentIndex(0);

}

void MainScreen::viewCoursesPage()
{
    qDebug() << "Viewing Courses" << endl;

    //Send a Courses request with the entered username
    QString courses = (c->sendCoursesRequest(ui->userName->text().toStdString() + "|" +
                                         (ui->semesterBox->itemText(ui->semesterBox->currentIndex())).toStdString()));


//    if(string false){
//        return "Not success";
//        QMessageBox
//    } else {

//    }

    qDebug() << "The items are: " + courses << endl;

    //Split the string with a delimiter
    CourseList coursesStringList = Course::stringToList(courses);

    //Creating model for the QList viewer
    QStandardItemModel *coursesModel = new QStandardItemModel();

    //Appending all the course names to the list
    for(int i = 0; i < coursesStringList.length(); i++){
        Course c1(coursesStringList.at(i));
        coursesModel->appendRow(new QStandardItem(c1.getName() + c1.getNumber()));
    }

    //Load the all of the courses onto the combobox
    ui->coursesList->setModel(coursesModel);

    //Change to the courses page
    ui->InfoWidget->setCurrentIndex(1);

}

void MainScreen::viewTAsPage()
{
    qDebug() << "Viewing TAs" << endl;

    //Send a TAs request with the entered username
    QString tas = (c->sendTAsRequest(ui->userName->text().toStdString() + "|" +
                                      ui->coursesList->currentIndex().data().toString().toStdString()
                                     + (ui->semesterBox->itemText(ui->semesterBox->currentIndex())).toStdString()));

    qDebug() << "The items are: " + tas << endl;

    //Split the string with a delimiter
    TAList tasStringList = TA::stringToList(tas);

    QStandardItemModel *tasModel = new QStandardItemModel();

    if(tas.compare("<empty>") == 0){
        tasModel->appendRow(new QStandardItem(QString("<empty>")));
    } else {
        for(int i = 0; i < tasStringList.length(); i++){
            TA ta(tasStringList.at(i));
            tasModel->appendRow(new QStandardItem(ta.getId()));
        }
    }

    //Load the all of the TAs onto the combobox
    ui->TAsList->setModel(tasModel);

    //Change to the TAs page
    ui->InfoWidget->setCurrentIndex(2);
}

void MainScreen::viewTaskPage()
{
    qDebug() << "Viewing Tasks" << endl;

    //Send a TAs request with the entered username
    QString task = (c->sendTaskRequest(ui->userName->text().toStdString() + "|" +
                                      ui->coursesList->currentIndex().data().toString().toStdString()
                                     + (ui->semesterBox->itemText(ui->semesterBox->currentIndex())).toStdString()
                                       + "|" +ui->TAsList->currentIndex().data().toString().toStdString()));

    qDebug() << "The tasks are: " + task << endl;

    //Split the string with a delimiter
    TaskList taskStringList = Task::stringToList(task);

    QStandardItemModel *taskModel = new QStandardItemModel();

    if(task.compare("<empty>") == 0){
        taskModel->appendRow(new QStandardItem(QString("<empty>")));
    } else {
         qDebug() << taskStringList.length() << endl;
        for(int i = 0; i < taskStringList.length(); i++){
            qDebug() << "The tasks are executed" << endl;
            Task task1(task1.getTaID(),
                       task1.getCourseID(),
                       task1.getDescription(),
                       task1.getStartDate(),
                       task1.getDueDate(),
                       task1.getId(),
                       qint32(1), QString("Hello"));
            taskModel->appendRow(new QStandardItem(task1.getTaID()));
        }
    }

    //Load the all of the Tasks onto the combobox
    ui->TAsList->setModel(taskModel);

    ui->InfoWidget->setCurrentIndex(3);
}

void MainScreen::viewEvaluationPage()
{
    qDebug() << "Viewing Evaluation" << endl;
    ui->InfoWidget->setCurrentIndex(4);
}

void MainScreen::viewCreateTaskPage()
{
    qDebug() << "Viewing Create Evaluation" << endl;
    ui->InfoWidget->setCurrentIndex(5);
}

void MainScreen::viewEditTaskPage()
{
    qDebug() << "Viewing Edit Evaluation" << endl;
    ui->InfoWidget->setCurrentIndex(6);
}

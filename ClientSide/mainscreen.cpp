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
    connect(ui->loginButton, SIGNAL(clicked()), this, SLOT(viewInstructorPage()));

//    connect(ui->logOutButton, SIGNAL(clicked()), this, SLOT(viewLoginScreen()));
//    connect(ui->viewTask, SIGNAL(clicked()), this, SLOT(viewTaskPage()));
//    connect(ui->viewEvaluation, SIGNAL(clicked()), this, SLOT(viewEvaluationPage()));
//    connect(ui->createTaskButton, SIGNAL(clicked()), this, SLOT(viewCreateTaskPage()));
//    connect(ui->editTaskButton, SIGNAL(clicked()), this, SLOT(viewEditTaskPage()));

    //For these ones, might have to add new functions because I have to save/cancel
//    connect(ui->evalSave, SIGNAL(clicked()), this, SLOT(viewTaskPage()));
//    connect(ui->evalCancel, SIGNAL(clicked()), this, SLOT(viewTaskPage()));
//    connect(ui->taskSave, SIGNAL(clicked()), this, SLOT(viewTaskPage()));
//    connect(ui->taskCancel, SIGNAL(clicked()), this, SLOT(viewTaskPage()));
//    connect(ui->editTaskSave, SIGNAL(clicked()), this, SLOT(viewTaskPage()));
//    connect(ui->editTaskCancel, SIGNAL(clicked()), this, SLOT(viewTaskPage()));

    ui->userName->setText("Christine");

//    ui->loginButton->animateClick();
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

void MainScreen::viewInstructorPage()
{
    //Username textfield can't be empty!
    if(!ui->userName->text().isEmpty()){
        qDebug() << "Viewing Instructor" << endl;

        //Send a login request with the entered username
        string success = c->sendLoginRequest(ui->userName->text().toStdString());

        //Check if the login request was successful otherwise send an error message saying user doesn't exist.
        if(success.compare("Instructor") == 0){
            c->userNameString = ui->userName->text();

            //Change to the instructor page and call the semester page
            ui->MainWidget->setCurrentIndex(1);
            getSemesters();
        } else {
            QMessageBox::critical(this, tr("TAEval"), tr("The username is invalid! Please try again."));
        }
    } else {
        QMessageBox::critical(this, tr("TAEval"), tr("The username is blank! Please enter an username."));
    }
}

void MainScreen::viewTaskPage()
{
    qDebug() << "Viewing Tasks" << endl;

    //Send a TAs request with the entered username
    QString task = (c->sendTaskRequest(c->userNameString.toStdString() + "|" +
                                       c->courseString.toStdString()
                                     + (c->semesterString.toStdString()
                                       + "|" +c->taString.toStdString())));

    qDebug() << "The tasks are: " + task << endl;

    //Split the string with a delimiter
    TaskList taskStringList = Task::stringToList(task);

    QStandardItemModel *taskModel;

    taskModel = new QStandardItemModel;

    taskModel->setColumnCount(4);
    taskModel->setRowCount(taskStringList.length());

    c->taskList = taskStringList;

    if(task.compare("<empty>") == 0){
        taskModel->appendRow(new QStandardItem(QString("<empty>")));
    } else {
         qDebug() << taskStringList.length() << endl;
        for(int i = 0; i < taskStringList.length(); i++){
            qDebug() << "The tasks are executed" << endl;

            Task task1(taskStringList.at(i).getTaID(),
                       taskStringList.at(i).getCourseID(),
                       taskStringList.at(i).getDescription(),
                       taskStringList.at(i).getStartDate(),
                       taskStringList.at(i).getDueDate(),
                       taskStringList.at(i).getId(),
                       taskStringList.at(i).getEvaluation()->getRating(),
                       taskStringList.at(i).getEvaluation()->getFeedback());

            taskModel->setItem(i, 0, new QStandardItem(QString::number(i)));
            taskModel->setItem(i, 1, new QStandardItem(task1.getDescription()));
            taskModel->setItem(i, 2, new QStandardItem((task1.getStartDate().toString(Qt::ISODate))));
            taskModel->setItem(i, 3, new QStandardItem((task1.getDueDate()).toString(Qt::ISODate)));
        }
    }

    //Load the all of the Tasks onto the combobox
    ui->taskList->setModel(taskModel);
    ui->InfoWidget->setCurrentIndex(1);
}

void MainScreen::viewEvaluationPage()
{
    qDebug() << "Viewing Evaluation" << endl;
    ui->InfoWidget->setCurrentIndex(4);
}

void MainScreen::viewCreateTaskPage()
{
    qDebug() << "Viewing Create Task" << endl;


    //Split the string with a delimiter
//    TaskList taskStringList = Task::stringToList(task);
    ui->InfoWidget->setCurrentIndex(3);
}

void MainScreen::viewEditTaskPage()
{
    qDebug() << "Viewing Edit Evaluation" << endl;
    ui->InfoWidget->setCurrentIndex(6);
}

void MainScreen::getSemesters()
{
    qDebug() << "Viewing Semesters" << endl;

    //Reset all data on the semester page
    ui->semesterBox->clear();
    ui->semesterBox->addItem("<Please select one>");

    //Request the list of semesters
    QString semester = (c->sendSemesterRequest(c->userNameString.toStdString()));

    qDebug() << "Semesters now are: " + semester << endl;

    //Split the string with a delimiter
    StringList semesterList = c->stringToList(semester);

    //Load the all of the relevant information onto the page
    ui->nameLabel->setText(c->userNameString);
    ui->semesterBox->addItems(semesterList);

    qDebug() << ui->semesterBox->currentText() << endl;

    c->semesterString = ui->semesterBox->currentText();

    //Turn to the semester page
    ui->InfoWidget->setCurrentIndex(0);

}

void MainScreen::getCourses()
{
    qDebug() << "Viewing Courses" << endl;

    ui->coursesBox->clear();
    ui->coursesBox->addItem("<Please select one>");

    //Send a Courses request with the entered username and semester
    QString courses = (c->sendCoursesRequest(c->userNameString.toStdString() + "|" +
                                         (c->semesterString.toStdString())));

    qDebug() << "The items are: " + courses << endl;

    //Split the string with a delimiter
    CourseList coursesStringList = Course::stringToList(courses);

    //Appending all the course names to the list
    for(int i = 0; i < coursesStringList.length(); i++){
        Course c1(coursesStringList.at(i));
        ui->coursesBox->addItem(QString(c1.getName() + c1.getNumber()));
    }

    c->courseString = ui->coursesBox->currentText();

}

void MainScreen::getTAs()
{
    qDebug() << "Viewing TAs" << endl;

    ui->tasBox->clear();
    ui->tasBox->addItem("<Please select one>");

    //Send a TAs request with the entered username
    QString tas = (c->sendTAsRequest(c->userNameString.toStdString() + "|" +
                                      c->courseString.toStdString()
                                     + c->semesterString.toStdString()));

    qDebug() << "The items are: " + tas << endl;

    //Split the string with a delimiter
    TAList tasStringList = TA::stringToList(tas);

    if(tas.compare("<empty>") == 0){
        ui->tasBox->clear();
        ui->tasBox->addItem(QString("<N/A>"));
        ui->tasBox->setEnabled(false);
    } else {
        for(int i = 0; i < tasStringList.length(); i++){
            TA ta(tasStringList.at(i));
            ui->tasBox->addItem(QString(ta.getId()));
            ui->tasBox->setEnabled(true);
        }
    }

    c->taString = ui->tasBox->currentText();
}

void MainScreen::getTasks(){

}


void MainScreen::on_semesterBox_currentIndexChanged()
{
    c->semesterString = ui->semesterBox->currentText();

    if(ui->semesterBox->currentIndex() == 0){
        ui->coursesBox->setVisible(false);
        ui->coursesLabel->setVisible(false);
        ui->coursesNavig->setVisible(false);
        ui->tasBox->setVisible(false);
        ui->tasLabel->setVisible(false);
        ui->TAsNavig->setVisible(false);
        ui->viewTaskListButton->setVisible(false);
    }else if (ui->semesterBox->currentIndex() == -1){


    } else {
        ui->coursesNavig->setVisible(true);
        ui->coursesLabel->setVisible(true);
        ui->coursesBox->setVisible(true);
        getCourses();
    }

}

void MainScreen::on_coursesBox_currentIndexChanged()
{
    c->courseString = ui->coursesBox->currentText();
    if(ui->coursesBox->currentIndex() == 0){
        ui->tasBox->setVisible(false);
        ui->tasLabel->setVisible(false);
        ui->TAsNavig->setVisible(false);
        ui->viewTaskListButton->setVisible(false);
    }else if (ui->semesterBox->currentIndex() == -1){


    } else {
        ui->TAsNavig->setVisible(true);
        ui->tasLabel->setVisible(true);
        ui->tasBox->setVisible(true);
        getTAs();
    }
}

void MainScreen::on_tasBox_currentIndexChanged()
{
    c->taString = ui->tasBox->currentText();
    if(ui->coursesBox->currentIndex() == 0 || ui->tasBox->currentText().compare("<N/A>") == 0){
        ui->viewTaskListButton->setVisible(false);
    }else if (ui->semesterBox->currentIndex() == -1){

    } else {
        ui->viewTaskListButton->setVisible(true);
        qDebug() << c->taString << endl;
    }
}

void MainScreen::on_viewTaskListButton_clicked()
{
    viewTaskPage();
//    ui->InfoWidget->setCurrentIndex(1);
}

void MainScreen::on_backToTAsPage_clicked()
{
    ui->InfoWidget->setCurrentIndex(0);
}

void MainScreen::on_logOutButton_clicked()
{
    //Don't forget to connect this to Server too since sql has a logout button for some reason
    viewLoginScreen();
}

void MainScreen::on_createTaskButton_clicked()
{
    viewCreateTaskPage();
}

void MainScreen::on_taskSave_clicked()
{
    //Send a TAs request with the entered username
//    QString task = (c->sendCreateTaskRequest(c->userNameString.toStdString() + "|" +
//                                       c->taString.toStdString() + "|" +
//                                       c->courseString.toStdString() +
//                                       c->semesterString.toStdString()));

    QString task = c->sendCreateTaskRequest(c->userNameString.toStdString() + "|" +
                                       c->taString.toStdString() + "|" +
                                       c->courseString.toStdString() +
                                            c->semesterString.toStdString() + "|" +
                                            ui->dutyText->toPlainText().toStdString() + "|" +
                                            ui->beginDate->text().toStdString() + "|" +
                                            ui->dueDate->text().toStdString());

    qDebug() << task << endl;

    QMessageBox::critical(this,
                          tr("Task created!"),
                          tr("Success!"));

    viewTaskPage();
//    qDebug() << "The tasks are: " + task << endl;

}

void MainScreen::on_evalSave_clicked()
{

    c->feedback = ui->feedbackText->toPlainText();
    c->rating = ui->taskRating->currentText();

    QString eval = c->sendEditEvalRequest(c->userNameString.toStdString() + "|" +
                                          c->currentTaskID.toStdString() + "|" +
                                          c->feedback.toStdString() + "|" +
                                          c->rating.toStdString() );

    qDebug() << eval << endl;

    QMessageBox::critical(this,
                          tr("Task created!"),
                          tr("Success!"));

    viewTaskPage();
//    qDebug() << "The tasks are: " + task << endl;

}

void MainScreen::on_taskList_clicked(const QModelIndex &index)
{
    int i = ui->taskList->currentIndex().data().toInt();
    c->currentTaskID = c->taskList.at(i).getId();

    qDebug() << c->currentTaskID << endl;

}

void MainScreen::on_viewEvaluation_clicked()
{
    ui->InfoWidget->setCurrentIndex(2);

    ui->taskID->setText(ui->taskList->currentIndex().data().toString());
    ui->taskRating->setCurrentIndex(ui->taskList->currentIndex().data().toInt());
//    ui->taskRating->setItemText(c->taskList.at(.data().toInt().getEvaluation()->getRating());
//    ui->feedbackText->setPlainText(c->taskList.at(ui->taskList->currentIndex().data().toInt().getEvaluation()->getFeedback());

}

void MainScreen::on_editTaskButton_clicked()
{
    ui->InfoWidget->setCurrentIndex(4);

    ui->taskID->setText(ui->taskList->currentIndex().data().toString());
    ui->beginDateEd->setDate(c->taskList.at(ui->taskList->currentIndex().data().toInt()).getStartDate());
    ui->dueDateEd->setDate(c->taskList.at(ui->taskList->currentIndex().data().toInt()).getDueDate());
    ui->dutyTextEd->setPlainText(c->taskList.at(ui->taskList->currentIndex().data().toInt()).getDescription());

}

void MainScreen::on_editTaskSave_clicked()
{
    ui->InfoWidget->setCurrentIndex(1);

    c->begDateEd = ui->beginDateEd->date();
    c->dueDateEd = ui->dueDateEd->date();
    c->duty = ui->dutyTextEd->toPlainText();

    QString editTask = c->sendEditTaskRequest(c->userNameString.toStdString() + "|" +
                                              c->duty.toStdString() + "|" +
                                              ui->beginDateEd->text().toStdString() + "|" +
                                              ui->dueDateEd->text().toStdString() + "|" +
                                              c->currentTaskID.toStdString());
    qDebug() << editTask << endl;

    QMessageBox::critical(this,
                          tr("Task created!"),
                          tr("Success!"));

    viewTaskPage();
//    qDebug() << "The tasks are: " + task << endl;

}

void MainScreen::on_deleteTaskButton_clicked()
{

    QString deleteTask = c->sendDeleteTaskRequest(c->userNameString.toStdString() + "|" +
                                              c->currentTaskID.toStdString());
    viewTaskPage();
}

#include "mainscreen.h"
#include "ui_mainscreen.h"

/**
  Constructor.
  **/

MainScreen::MainScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainScreen)
{
    ui->setupUi(this);

    //Initialize client here ...
    c = new Client();

    ui->userName->setFocus();

    //Connect the socket to a signal for error in connection and send a clear message to user what the error is.
    connect(c->tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(sendError()));

    //Connect all the buttons to a slot in order to apply the appropriate response
    connect(ui->loginButton, SIGNAL(clicked()), this, SLOT(viewInstructorPage()));
    connect(ui->switchToTestMode,SIGNAL(clicked()),this,SLOT(switchToTestMode()));
    connect(ui->testCaseNo_1,SIGNAL(clicked()),this,SLOT(testCase_1()));
    connect(ui->testCaseNo_2,SIGNAL(clicked()),this,SLOT(testCase_2()));
    connect(ui->testCaseNo_3,SIGNAL(clicked()),this,SLOT(testCase_3()));
    connect(ui->testCaseNo_4,SIGNAL(clicked()),this,SLOT(testCase_4()));
    connect(ui->testCaseNo_5,SIGNAL(clicked()),this,SLOT(testCase_5()));
    connect(ui->testCaseNo_6,SIGNAL(clicked()),this,SLOT(testCase_6()));
    connect(ui->testCaseNo_7,SIGNAL(clicked()),this,SLOT(testCase_7()));
    connect(ui->testCaseNo_8,SIGNAL(clicked()),this,SLOT(testCase_8()));
    connect(ui->testCaseNo_9,SIGNAL(clicked()),this,SLOT(testCase_9()));
    connect(ui->testCaseNo_10,SIGNAL(clicked()),this,SLOT(testCase_10()));
    connect(ui->testCaseNo_11,SIGNAL(clicked()),this,SLOT(testCase_11()));
}

/**
  Deconstructor.
  **/
MainScreen::~MainScreen()
{
    delete ui;
}

/**
  Error message function, this is to display any error messages to the user during run time.
  **/
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

/**
  To go to the login screen and clear the previous username text.
  **/
void MainScreen::viewLoginScreen(){

    qDebug() << "Viewing Login Screen" << endl;

    //Clear username textfield
    ui->userName->clear();

    //Set focus on the textfield for easier access
    ui->userName->setFocus();

    //Then go to the login page
    ui->MainWidget->setCurrentIndex(0);

}


/**
  To go to the instructor's page.
  **/
void MainScreen::viewInstructorPage()
{
    //Username textfield can't be empty!
    if(!ui->userName->text().isEmpty()){
        qDebug() << "Viewing Instructor" << endl;

        //Send a login request with the entered username
        QString role = c->sendRequest("loginRequest|", ui->userName->text());

        //Check if the login request was successful otherwise send an error message saying user doesn't exist.
        if(role.compare("Instructor") == 0){
            userNameString = ui->userName->text();
            //Change to the instructor page and call the semester page
            ui->MainWidget->setCurrentIndex(1);
            getSemesters();
        } else if (role.compare("TA") == 0){
            QMessageBox::critical(this, tr("TAEval"), tr("The username is invalid or inactive! Please try again."));
        }else {
            QMessageBox::critical(this, tr("TAEval"), tr("The username is invalid or inactive! Please try again."));
        }
    } else {
        QMessageBox::critical(this, tr("TAEval"), tr("The username is blank! Please enter an username."));
    }
}

/**
  To go to the tasks page and process all the task and display them on the table.
  **/
void MainScreen::viewTaskPage()
{
    qDebug() << "Viewing Tasks" << endl;

    //Send a Task request with the entered username
    QString task = c->sendRequest("taskRequest|", userNameString + "|" +
                                       courseString + semesterString
                                       + "|" +taString);

    qDebug() << "The tasks are: " + task << endl;

    //Split the string with a delimiter
    TaskList taskStringList = Task::stringToList(task);

    taskModel = new QStandardItemModel;
    taskModel->setColumnCount(4);
    taskModel->setRowCount(taskStringList.length());

    taskList = taskStringList;

    if(task.compare("false") == 0){
        ui->taskList->setEnabled(false);
        ui->viewEvaluation->setEnabled(false);
        ui->editTaskButton->setEnabled(false);
        ui->deleteTaskButton->setEnabled(false);
    } else {
        ui->taskList->setEnabled(true);
        ui->viewEvaluation->setEnabled(true);
        ui->editTaskButton->setEnabled(true);
        ui->deleteTaskButton->setEnabled(true);
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


/**
  Function call to get all the semesters applicable for the instructor
  **/
void MainScreen::getSemesters()
{
    qDebug() << "Viewing Semesters" << endl;

    //Reset all data for the semester combo box
    ui->semesterBox->clear();
    ui->semesterBox->addItem("<Please select one>");

    //Request the list of semesters
    QString semester = c->sendRequest("semesterRequest|", userNameString);

    qDebug() << "Semesters now are: " + semester << endl;

    //Split the string with a delimiter
    StringList semesterList = stringToList(semester);

    if(semester.compare("false") == 0){
        ui->semesterBox->clear();
        ui->semesterBox->addItem(QString("<N/A>"));
        ui->semesterBox->setEnabled(false);
    } else {
        ui->semesterBox->addItems(semesterList);
        ui->semesterBox->setEnabled(true);
    }

    //Load the all of the relevant information onto the page
    ui->nameLabel->setText(userNameString);

    //Save the current selected semester locally
    semesterString = ui->semesterBox->currentText();

    //Turn to the semester page
    ui->InfoWidget->setCurrentIndex(0);

}

/**
  Function call to get all the courses applicable for the instructor at a given semester
  **/
void MainScreen::getCourses()
{
    qDebug() << "Viewing Courses" << endl;

    //Reset all data for the courses combo box
    ui->coursesBox->clear();
    ui->coursesBox->addItem("<Please select one>");

    //Send a Courses request with the entered username and semester
    QString courses = c->sendRequest("coursesRequest|", userNameString + "|" +
                                         semesterString);

    qDebug() << "The items are: " + courses << endl;

    //Split the string with a delimiter
    CourseList coursesStringList = Course::stringToList(courses);

    if(courses.compare("false") == 0){
        ui->coursesBox->clear();
        ui->coursesBox->addItem(QString("<N/A>"));
        ui->coursesBox->setEnabled(false);
    } else {
        //Appending all the course names to the list
        for(int i = 0; i < coursesStringList.length(); i++){
            Course c1(coursesStringList.at(i));
            ui->coursesBox->addItem(QString(c1.getName() + c1.getNumber()));
            ui->coursesBox->setEnabled(true);
        }
    }

    //Save the current selected course locally
    courseString = ui->coursesBox->currentText();
}

/**
  Function call to get all the tas applicable for the instructor at a given semester for a selected course
  **/
void MainScreen::getTAs()
{
    qDebug() << "Viewing TAs" << endl;

    //Reset all data for the TAs combo box
    ui->tasBox->clear();
    ui->tasBox->addItem("<Please select one>");

    //Send a TAs request with the entered username
    QString tas = c->sendRequest("tasRequest|", userNameString + "|" + courseString + semesterString);

    qDebug() << "The items are: " + tas << endl;

    //Split the string with a delimiter
    TAList tasStringList = TA::stringToList(tas);

    if(tas.compare("false") == 0){
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

    //Save the current selected TA locally
    taString = ui->tasBox->currentText();
}

/**
  All of the functions below are just event handlers for whatever happens in the GUI such as a
  button click, a text change, list item change and etc.
  **/

void MainScreen::on_semesterBox_currentIndexChanged()
{
    semesterString = ui->semesterBox->currentText();

    if(ui->semesterBox->currentIndex() == 0){
        ui->coursesBox->setVisible(false);
        ui->coursesLabel->setVisible(false);
        ui->coursesNavig->setVisible(false);
        ui->tasBox->setVisible(false);
        ui->tasLabel->setVisible(false);
        ui->TAsNavig->setVisible(false);
        ui->viewTaskListButton->setVisible(false);
    }else if (ui->semesterBox->currentIndex() == -1){ //If there's no current index then do nothing


    } else {
        ui->coursesNavig->setVisible(true);
        ui->coursesLabel->setVisible(true);
        ui->coursesBox->setVisible(true);
        getCourses();
    }

}

void MainScreen::on_coursesBox_currentIndexChanged()
{
    courseString = ui->coursesBox->currentText();
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
    taString = ui->tasBox->currentText();

    if(ui->tasBox->currentIndex() == 0 || ui->tasBox->currentText().compare("<N/A>") == 0){
        ui->viewTaskListButton->setVisible(false);
    }else if (ui->semesterBox->currentIndex() == -1){

    } else {
        ui->viewTaskListButton->setVisible(true);
        qDebug() << taString << endl;
    }
}

void MainScreen::on_viewTaskListButton_clicked()
{
    viewTaskPage();
}

void MainScreen::on_logOutButton_clicked()
{
    QString logOut = c->sendRequest("logOutRequest|", userNameString);

    if(logOut.compare("true") == 0){
        QMessageBox::critical(this,
                              tr("Logged off!"),
                              tr("Successfully logged off."));
    } else {
        QMessageBox::critical(this,
                              tr("Logged off Error!"),
                              tr("Unsuccessfully logged off."));
    }

    qDebug() << logOut << endl;
    viewLoginScreen();
}

/**
  Go to the create task page when create task button is clicked.
  **/
void MainScreen::on_createTaskButton_clicked()
{
    ui->taskIDCr->setText(QString::number(taskModel->rowCount()));
    ui->beginDate->setDate(QDate(2013, 9, 2));
    ui->dueDate->setDate(QDate(2013, 9, 2));
    ui->dutyText->clear();
    ui->InfoWidget->setCurrentIndex(3);
}

void MainScreen::on_taskSave_clicked()
{
    //Send a save task request with the required and entered data
    QString taskSuccess = c->sendRequest("taskCreateRequest|", userNameString + "|" +
                                       taString + "|" + courseString + semesterString + "|" +
                                            ui->dutyText->toPlainText() + "|" +
                                            ui->beginDate->text() + "|" +
                                            ui->dueDate->text());

    qDebug() << "The tasks are: " <<  taskSuccess << endl;


    if(taskSuccess.compare("true") == 0){
        QMessageBox::critical(this,
                              tr("Task created!"),
                              tr("Successfully created a task."));
    } else {
        QMessageBox::critical(this,
                              tr("Task not created!"),
                              tr("Creation of task failed."));
    }

    viewTaskPage();
}

void MainScreen::on_editTaskButton_clicked()
{
    ui->InfoWidget->setCurrentIndex(4);
    ui->taskIDEd->setText(ui->taskList->currentIndex().data().toString());
    ui->beginDateEd->setDate(taskList.at(ui->taskList->currentIndex().data().toInt()).getStartDate());
    ui->dueDateEd->setDate(taskList.at(ui->taskList->currentIndex().data().toInt()).getDueDate());
    ui->dutyTextEd->setPlainText(taskList.at(ui->taskList->currentIndex().data().toInt()).getDescription());
}

void MainScreen::on_editTaskSave_clicked()
{
    ui->InfoWidget->setCurrentIndex(1);

    begDateEd = ui->beginDateEd->date();
    dueDateEd = ui->dueDateEd->date();
    duty = ui->dutyTextEd->toPlainText();

    QString editTask = c->sendRequest("taskEditRequest|", userNameString + "|" +
                                              duty + "|" + ui->beginDateEd->text() + "|" +
                                              ui->dueDateEd->text() + "|" + currentTaskID);
    qDebug() << editTask << endl;

    if(editTask.compare("true") == 0){
        QMessageBox::critical(this,
                              tr("Task edited!"),
                              tr("Successfully edited the task."));
    } else {
        QMessageBox::critical(this,
                              tr("Task not edited!"),
                              tr("Editing of the task failed."));
    }

    viewTaskPage();

}

void MainScreen::on_deleteTaskButton_clicked()
{

    QString deleteTask = c->sendRequest("taskDeleteRequest|", userNameString + "|" +currentTaskID);

    if(deleteTask.compare("true") == 0){
        QMessageBox::critical(this,
                              tr("Task deleted!"),
                              tr("Successfully deleted the task."));
    } else {
        QMessageBox::critical(this,
                              tr("Task not deleted!"),
                              tr("Couldn't delete the task."));
    }

    viewTaskPage();
}

void MainScreen::on_taskList_clicked()
{
    int i = ui->taskList->currentIndex().data().toInt();
    currentTaskID = taskList.at(i).getId();

    qDebug() << currentTaskID << endl;
}

void MainScreen::on_viewEvaluation_clicked()
{
    ui->InfoWidget->setCurrentIndex(2);
    ui->taskID->setText(ui->taskList->currentIndex().data().toString());
    ui->taskRating->setCurrentIndex(taskList.at(ui->taskList->currentIndex().data().toInt()).getEvaluation()->getRating());
    ui->feedbackText->setPlainText(taskList.at(ui->taskList->currentIndex().data().toInt()).getEvaluation()->getFeedback());
}

void MainScreen::on_evalSave_clicked()
{
    feedback = ui->feedbackText->toPlainText();
    rating = ui->taskRating->currentText();

    QString eval = c->sendRequest("editEvalRequest|", userNameString + "|" +
                                          currentTaskID + "|" + feedback + "|" + rating);

    qDebug() << "The evaluations are: " << eval << endl;


    if(eval.compare("true") == 0){
        QMessageBox::critical(this,
                              tr("Evaluation saved!"),
                              tr("Successfully saved the evaluation data."));
    } else {
        QMessageBox::critical(this,
                              tr("Evaluation not saved!"),
                              tr("Couldn't save the evaluation data."));
    }

    viewTaskPage();

}

void MainScreen::on_backToInstructorsPage_clicked()
{
    ui->InfoWidget->setCurrentIndex(0);
}

void MainScreen::on_evalCancel_clicked()
{
    ui->InfoWidget->setCurrentIndex(1);
}

void MainScreen::on_taskCancel_clicked()
{
    ui->InfoWidget->setCurrentIndex(1);
}

void MainScreen::on_editTaskCancel_clicked()
{
    ui->InfoWidget->setCurrentIndex(1);
}

//Converters
QString MainScreen::listToString(StringList list){ // Parse a list of QStrings to a single QString
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

StringList MainScreen::stringToList(QString aString){  // Convert a single QString to a list of QStrings
    StringList result = StringList();
    QList<QString> list = aString.split(QRegExp("(~`|`~|~~)"));
    for(int i=1;i<list.length()-1;i++) result += list[i];
    return result;
}


/*************************************************/
/************Test Cases***************************/
/*************************************************/

//Test Case 1 : Login
void MainScreen::testCase_1()
{
    ui->testCaseContent->clear();
    ui->testCaseDescription->clear();
    ui->testCaseShow->clear();
    //Welcome Info
    ui->testCaseContent->appendPlainText("TestCase_No.1: Login ");
    ui->testCaseDescription->appendPlainText("The testCase will: ");
    ui->testCaseDescription->appendPlainText("1.Login Christine who is a Instructor");
    ui->testCaseDescription->appendPlainText("2.Login  as Abdallah who is a TA");
    ui->testCaseDescription->appendPlainText("3.Login as Nobody which is not stored in database");
    ui->testCaseDescription->appendPlainText("4.Login  without input any username");

    QString role = c->sendRequest("loginRequest|",tr("Christine"));
    //Test Login
    //Case1
    ui->testCaseShow->appendPlainText("Case1_1:\nExpect: Instructor Loged in");
    verifyUser(role);
//    ui->testCaseShow->appendPlainText("The user has logged in as Christine, Verified by viewInstrutctorPage() successully");


/******************* For Failed Case_2:\nThe User is not a Instructor   ***************/
    ui->testCaseShow->appendPlainText("Case1_2:\n Expect: User is verifed as TA");
    role = c->sendRequest("loginRequest|","Abdallah");
    verifyUser(role);

/******************* For Failed Case_3:\n No such user   ***************/
    ui->testCaseShow->appendPlainText("Case1_3:\nExpect: The username is invalid!");
    role = c->sendRequest("loginRequest|","Nobody");
    ui->userName->setText("Nobody");
    verifyUser(role);


/******************* For Failed Case_4: Empty   ***************/
    ui->testCaseShow->appendPlainText("Case1_4:\nExpect: The username invalid!");
    role = c->sendRequest("loginRequest|", "");

    verifyUser(role);


}


//Test Case 2: View Semesters

void MainScreen::testCase_2()
{
   ui->testCaseContent->clear();
    ui->testCaseDescription->clear();
    ui->testCaseShow->clear();
    ui->testCaseContent->appendPlainText("TestCase_No.2: ViewSemesters ");
    ui->testCaseDescription->appendPlainText("The testCase will: ");
    ui->testCaseDescription->appendPlainText("1.Show all the semesters that Instructor Christine erolled");
    c->sendRequest("loginRequest|","Christine");

    QString semester = c->sendRequest("semesterRequest|", "Christine");

//    ui->testCaseShow->appendPlainText("The user has logged in as Christine, Verified by viewInstrutctorPage() successully");

    //Test select Semester
    ui->testCaseShow->clear();
    ui->testCaseShow->appendPlainText("TestCase_No.2_1\nExpect: List Of Semesters");
    ui->testCaseShow->appendPlainText("Result:");
    StringList semesterList = stringToList(semester);
    verifySemester(semesterList);

}

//Test Case 3: View Couses

void MainScreen::testCase_3()
{
    ui->testCaseContent->clear();
    ui->testCaseDescription->clear();
    ui->testCaseShow->clear();

    ui->testCaseContent->appendPlainText("TestCase_No.3: ViewCourses ");
    ui->testCaseDescription->appendPlainText("The testCase will: ");
    ui->testCaseDescription->appendPlainText("1.Show all the courses that Instructor Christine teaches in a given semester");
    ui->testCaseDescription->appendPlainText("2.Instructor changed to Nobody, who is not a Instructor");
    //Test Login
    c->sendRequest("loginRequest|","Christine");
    QString key = "Christine|F2013";
    QString courses = c->sendRequest("coursesRequest|", key);
    ui->testCaseShow->appendPlainText("Case3_1\nExpect: Pass all courses taught by Christine in Fall 2013");
    //Test view Course Info
    ui->testCaseShow->appendPlainText("Result:");
    verifyCourse(courses);

    key = "Nobody|F2013";
    courses = c->sendRequest("coursesRequest|", key);
    ui->testCaseShow->appendPlainText("Case3_2\nExpect: Fail");
    //Test view Course Info
    ui->testCaseShow->appendPlainText("Result:");
    verifyCourse(courses);

}

//Test Case 4: View TAs

void MainScreen::testCase_4()
{
    ui->testCaseContent->clear();
    ui->testCaseDescription->clear();
    ui->testCaseShow->clear();

    ui->testCaseContent->appendPlainText("TestCase_No.4: ViewTAs ");
    ui->testCaseDescription->appendPlainText("The testCase will: ");
    ui->testCaseDescription->appendPlainText("1.Show all the TAs for a course that Instructor Christine teaches in a given semester");
    ui->testCaseDescription->appendPlainText("2.No TAs are assigned to the course will be taught by Chrintine in that given semester");

    c->sendRequest("loginRequest|",tr("Christine"));
    QString key = "Christine|COMP3004F2013";
    QString tas = c->sendRequest("tasRequest|", key);
    ui->testCaseShow->appendPlainText("Case4_1:\nExpect: List of TAs");
    ui->testCaseShow->appendPlainText("Result:");
    verifyTa(tas);

    ui->testCaseShow->appendPlainText("Case4_2:\nExpect: Failed");
    ui->testCaseShow->appendPlainText("Result:");
    key = "Christine|COMP2404F2013";
    tas = c->sendRequest("tasRequest|", key);
    verifyTa(tas);

}

//Test Case 5: View Tasks

void MainScreen::testCase_5()
{
    ui->testCaseContent->clear();
    ui->testCaseDescription->clear();
    ui->testCaseShow->clear();

    ui->testCaseContent->appendPlainText("TestCase_No.5: ViewTasks ");
    ui->testCaseDescription->appendPlainText("The testCase will: ");
    ui->testCaseDescription->appendPlainText("1.Show all the Tasks assigned to a TA of a course that Instructor Christine teaches in a given semester");
    ui->testCaseDescription->appendPlainText("2.No Tasks assigned to the TA assigned to the course");

    ui->testCaseShow->clear();
    ui->tasBox->setCurrentIndex(1);
    ui->testCaseShow->appendPlainText("Case5_1:");
    ui->testCaseShow->appendPlainText("Expect: Pass");
    ui->testCaseShow->appendPlainText("Result:");
    c->sendRequest("loginRequest|",tr("Christine"));
    QString key = "Christine|COMP3004F2013|Abdallah";
    QString task = c->sendRequest("taskRequest|", key);
    ui->testCaseShow->appendPlainText("Case5_1:\nExpect: List of Tasks");
    ui->testCaseShow->appendPlainText("Result:");
    verifyTask(task);

    key = "Nobody|COMP3004F2013|Abdallah";
    task = c->sendRequest("taskRequest|", key);
    ui->testCaseShow->appendPlainText("Case5_2:\nExpect: Fail");
    ui->testCaseShow->appendPlainText("Result:");
    verifyTask(task);

}

//Test Case 6 : Create Task
void MainScreen::testCase_6()
{
    ui->testCaseContent->clear();
    ui->testCaseDescription->clear();
    ui->testCaseShow->clear();

    ui->testCaseContent->appendPlainText("TestCase_No.6: CreateTask ");
    ui->testCaseDescription->appendPlainText("The testCase will: ");
    ui->testCaseDescription->appendPlainText("1.A Task assigned to a TA of a course that Instructor Christine teaches in a given semester is created succeffuly");
    ui->testCaseDescription->appendPlainText("2.Failed to assign a Task to the TA assigned to the course");

    ui->testCaseShow->clear();
    ui->tasBox->setCurrentIndex(1);
    ui->testCaseShow->appendPlainText("Case6_1:");
    ui->testCaseShow->appendPlainText("Expect:\nPass");
    ui->testCaseShow->appendPlainText("Result:");

    c->sendRequest("loginRequest|",tr("Christine"));    
    QString key ="Christine|COMP3004F2013|Abdallah";

    ui->testCaseShow->appendPlainText("Before Create Task:");
    QString task = c->sendRequest("taskRequest|", key);
    verifyTask(task);

    key = "Christine|Abdallah|COMP3004F2013|Whatever|2013-09-02|2013-09-11";
    QString taskSuccess = c->sendRequest("taskCreateRequest|",key );
    verifyCreateTask(taskSuccess,"Christine","Abdallah","COMP3004F2013");


    ui->testCaseShow->appendPlainText("Case6_2:");
    ui->testCaseShow->appendPlainText("Expect:\nFalse");

    key ="Pat|Abdallah|COMP3004F2013|Whatever|2013-09-02|2013-09-11";
    taskSuccess = c->sendRequest("taskCreateRequest|",key );
    verifyCreateTask(taskSuccess,"Pat","Abdallah","COMP3004F2013");

}

//Test Case 7: Edit Task

void MainScreen::testCase_7()
{
    ui->testCaseContent->clear();
    ui->testCaseDescription->clear();
    ui->testCaseShow->clear();

    ui->testCaseContent->appendPlainText("TestCase_No.7: EditTask ");
    ui->testCaseDescription->appendPlainText("The testCase will: ");
    ui->testCaseDescription->appendPlainText("1.Edit Task assigned to a TA of a course that Instructor Christine teaches in a given semester is created succeffuly");
    ui->testCaseDescription->appendPlainText("2.Failed to assign a Task to the TA assigned to the course");

    ui->testCaseShow->clear();
    ui->tasBox->setCurrentIndex(1);
    ui->testCaseShow->appendPlainText("Case7_1:");
    ui->testCaseShow->appendPlainText("Expect:\n Pass");
    ui->testCaseShow->appendPlainText("Result:");
    c->sendRequest("loginRequest|",tr("Christine"));
    QString key1 = "Christine|COMP3004F2013|Abdallah";
    QString task = c->sendRequest("taskRequest|", key1);
    QString choosenTaskId;
    ui->testCaseShow->appendPlainText("Before Edit");
    choosenTaskId = getChoosenID(task);

    QString key = "Christine|ThisIsUsedToTestEditTaskGG|2013-09-09|2013-11-11";
    QString editTask = c->sendRequest("taskEditRequest|", key + "|" + choosenTaskId);

    verifyEditTask(editTask,choosenTaskId,"Christine|COMP3004F2013|Abdallah");

    key = "Nobody|ThisIsUsedToTestEditTaskGG|2013-09-09|2013-11-11";
    editTask = c->sendRequest("taskEditRequest|", key + "|" + choosenTaskId);

    ui->testCaseShow->appendPlainText("\nCase7_2:");
    ui->testCaseShow->appendPlainText("Expect:\n Fail");
    ui->testCaseShow->appendPlainText("Result:");
    editTask = c->sendRequest("taskEditRequest|", key + "|" + choosenTaskId);
    verifyEditTask(editTask,choosenTaskId,"Christine|COMP3004F2013|Abdallah");

}

//Test Case 8: Delete a Task
void MainScreen::testCase_8()
{
    ui->testCaseContent->clear();
    ui->testCaseDescription->clear();
    ui->testCaseShow->clear();
    ui->testCaseContent->appendPlainText("TestCase_No.8: DeleteTask ");
    ui->testCaseDescription->appendPlainText("The testCase will: ");
    ui->testCaseDescription->appendPlainText("1.Delete a Task assigned to a TA of a course that Instructor Christine teaches in a given semester is created succeffuly");
    ui->testCaseDescription->appendPlainText("2.Failed to delete a Task to the TA assigned to the course");

    ui->testCaseShow->clear();
    ui->testCaseShow->appendPlainText("Case8_1:");
    ui->testCaseShow->appendPlainText("Expect:\n The Task is deleted Succefully");
    ui->testCaseShow->appendPlainText("Result:");
    ui->testCaseShow->appendPlainText("Before:");
    c->sendRequest("loginRequest|",tr("Christine"));
    QString key = "Christine|COMP3004F2013|Abdallah";
    QString task = c->sendRequest("taskRequest|", key);
    verifyTask(key);
    QString choosenTaskId;
    choosenTaskId = getChoosenID(task);

    key = "Christine";
    QString deleteTask = c->sendRequest("taskDeleteRequest|", key + "|" + choosenTaskId);
    key = "Christine|COMP3004F2013|Abdallah";
    verifyDeleteTask(deleteTask,key);

    ui->testCaseShow->appendPlainText("Case8_2:");
    ui->testCaseShow->appendPlainText("Expect:\n Fail");
    ui->testCaseShow->appendPlainText("Result:");
    key = "Nobody";
    deleteTask = c->sendRequest("taskDeleteRequest|", key + "|" + choosenTaskId);
    key = "Christine|COMP3004F2013|Abdallah";
    verifyDeleteTask(deleteTask,key);
}


//test case 9 Open evaluation
void MainScreen::testCase_9()
{
    ui->testCaseContent->clear();
    ui->testCaseDescription->clear();
    ui->testCaseShow->clear();

    ui->testCaseContent->appendPlainText("TestCase_No.9: Open Evaluation ");
    ui->testCaseDescription->appendPlainText("The testCase will: ");
    ui->testCaseDescription->appendPlainText("1.Return the evaltion of a Task succeffuly");
    ui->testCaseShow->appendPlainText("Case9_1:\nExpect: The Evaluation of the Task");
    ui->testCaseShow->appendPlainText("Result:");

    c->sendRequest("loginRequest|",tr("Christine"));
    QString key1 = "Christine|COMP3004F2013|Abdallah";
    QString task = c->sendRequest("taskRequest|", key1);
    QString choosenTaskId;
    //Split the string with a delimiter
    choosenTaskId = verifyOpenEval(task);

}


//Test Case 10: Edit Evaluation
void MainScreen::testCase_10()
{
    ui->testCaseContent->clear();
    ui->testCaseDescription->clear();
    ui->testCaseShow->clear();

    ui->testCaseShow->clear();
    ui->testCaseContent->appendPlainText("TestCase_No.10: Edit Evaluation ");
    ui->testCaseDescription->appendPlainText("The testCase will: ");
    ui->testCaseDescription->appendPlainText("1.Return Pass if the evaluation of a Task is submited succeffuly");
    ui->testCaseDescription->appendPlainText("2.Return False if the evaluation is failed to submit");
    ui->testCaseShow->appendPlainText("Case10_1:\nExpect: Pass. The evaluation is updated succesfully");
    ui->testCaseShow->appendPlainText("Result:");

    c->sendRequest("loginRequest|",tr("Christine"));
    QString key1 = "Christine|COMP3004F2013|Abdallah";
    QString task = c->sendRequest("taskRequest|", key1);
    QString choosenTaskId;
    choosenTaskId = verifyOpenEval(task);

    QString key = "Christine";
    QString feedB = "OMGGGSMD";
    QString rt = QString::number(3);
    QString eval = c->sendRequest("editEvalRequest|",  key + "|" +
                                  choosenTaskId + "|" + feedB + "|" + rt);

    qDebug() <<"Check this out!!!"<< endl;
    ui->testCaseShow->appendPlainText("Before: ");
    key = "Christine|COMP3004F2013|Abdallah";
    verifyEditEval(eval,key,choosenTaskId);

    ui->testCaseShow->appendPlainText("\n\nCase10_2:\nExpect: Fail");
    ui->testCaseShow->appendPlainText("Result:");
    key = "Nobody";
    feedB = "OMGGGSMD";
    rt = QString::number(4);
    eval = c->sendRequest("editEvalRequest|",  key + "|" +
                                  choosenTaskId + "|" + feedB + "|" + rt);
    key = "Christine|COMP3004F2013|Abdallah";
    verifyEditEval(eval,key,choosenTaskId);

}

//Test case 11: Log out a User
void MainScreen::testCase_11()
{
    c->sendRequest("loginRequest|",tr("Christine"));
    QString logOut = c->sendRequest("logOutRequest|", "Christine");
    ui->testCaseShow->appendPlainText("User Logged In is Christine:");
    ui->testCaseShow->appendPlainText("Case11_1:");
    ui->testCaseShow->appendPlainText("Expect:\nPass");
    verifyLogOut(logOut);

    ui->testCaseShow->appendPlainText("Case11_2:");
    ui->testCaseShow->appendPlainText("Expect:\nFail");
    logOut = c->sendRequest("logOutRequest|", "Pat");
    verifyLogOut(logOut);

}

void MainScreen::switchToTestMode()
{
    ui->MainWidget->setCurrentIndex(2);
    testMode = true;
}

void MainScreen::verifyUser(QString role)
{

    if(role.isEmpty())
    {
        ui->testCaseShow->appendPlainText("Result: The username is blank! Please enter an username.");
    }
        //Check if the login request was successful otherwise send an error message saying user doesn't exist.
    else if(role.compare("Instructor") == 0)
    {
        ui->testCaseShow->appendPlainText("Result: Instructor Loged in");

    }
    else if(role.compare("TA") == 0)
    {
        qDebug() << "TA" <<endl;
        ui->testCaseShow->appendPlainText("Result: User is verifed as TA");
    }
    else
    {
        ui->testCaseShow->appendPlainText("Result: The username is invalid!");
    }
}

void MainScreen::verifySemester(QStringList semesterList)
{
    for(int i=0; i<semesterList.size(); i++)
    {
       ui->testCaseShow->appendPlainText(semesterList.at(i));
    }
}

void MainScreen::verifyCourse(QString courses)
{
    CourseList coursesStringList = Course::stringToList(courses);

    if(courses.compare("false") == 0)
    {
        ui->testCaseShow->appendPlainText("Fail to get the CourseList");
    }
    else
    {
        for(int i = 0; i < coursesStringList.length(); i++)
        {
            Course c1(coursesStringList.at(i));
            ui->testCaseShow->appendPlainText(QString(c1.getName() + c1.getNumber() + "\nInstructor: " +c1.getInstructorId() + "\nSemester: "+c1.getSemester()));

        }
        ui->testCaseShow->appendPlainText("Pass");
        ui->testCaseShow->appendPlainText("________________________________");
    }
}

void MainScreen::verifyTa(QString tas)
{
    //Split the string with a delimiter
    TAList tasStringList = TA::stringToList(tas);
    if(tas.compare("false") == 0)
    {
        ui->testCaseShow->appendPlainText("Fail to get the TA List");
    }
    else
    {
        for(int i = 0; i < tasStringList.length(); i++)
        {
            TA ta(tasStringList.at(i));
            ui->testCaseShow->appendPlainText(QString(ta.getId()));

        }
        ui->testCaseShow->appendPlainText("Pass");
        ui->testCaseShow->appendPlainText("____________________________________");
    }
}

void MainScreen::verifyTask(QString task)
{
    //Split the string with a delimiter
    TaskList taskStringList = Task::stringToList(task);

    taskList = taskStringList;

    if(task.compare("false") == 0)
    {
 //       ui->testCaseShow->clear();
        ui->testCaseShow->appendPlainText("No Tasks, all tests return false");
    }
    else
    {
        for(int i = 0; i < taskStringList.length(); i++)
        {
            qDebug() << "The tasks are executed" << endl;

            Task task1(taskStringList.at(i).getTaID(),
                       taskStringList.at(i).getCourseID(),
                       taskStringList.at(i).getDescription(),
                       taskStringList.at(i).getStartDate(),
                       taskStringList.at(i).getDueDate(),
                       taskStringList.at(i).getId(),
                       taskStringList.at(i).getEvaluation()->getRating(),
                       taskStringList.at(i).getEvaluation()->getFeedback());
            ui->testCaseShow->appendPlainText("TaskID: " + task1.getId() + "\n" +
                                              "CourseID:" + task1.getCourseID() + "\n" +
                                              "Task Descprtion: " + task1.getDescription()
                                              );
            ui->testCaseShow->appendPlainText("________________________________");

        }
        ui->testCaseShow->appendPlainText("Pass");
        ui->testCaseShow->appendPlainText("________________________________");
    }
}

void MainScreen::verifyCreateTask(QString taskSuccess,QString user,QString ta,QString semester)
{
    if(taskSuccess.compare("true") == 0)
    {
        ui->testCaseShow->appendPlainText("After Create Tasks:");
        QString key = user + "|" + semester + "|" +ta;
        QString task = c->sendRequest("taskRequest|", key);
        //Split the string with a delimiter
        verifyTask(task);
        ui->testCaseShow->appendPlainText("____________________________________");
    }
    else
    {
        ui->testCaseShow->appendPlainText("Fail to Create Task");
    }
}

QString MainScreen::getChoosenID(QString task)
{
    QString choosenTaskId;
    //Split the string with a delimiter
    TaskList taskStringList = Task::stringToList(task);

    taskList = taskStringList;

    if(task.compare("false") == 0)
    {
        ui->testCaseShow->clear();
        ui->testCaseShow->appendPlainText("No Tasks, Can't' Edit, all tests return false");
    }
    else
    {
        choosenTaskId = taskList.at(0).getId();
        for(int i = 0; i < taskStringList.size(); i++)
        {
            if(taskStringList.at(i).getId().compare(choosenTaskId)==0)
            {
                ui->testCaseShow->appendPlainText("Task ID: " + taskStringList.at(i).getId() + "\n" +
                                                  "CourseID: " + taskStringList.at(i).getCourseID() + "\n"+
                                                  "Description: " + taskStringList.at(i).getDescription() + "\n"+
                                                  "StartDate: " + taskStringList.at(i).getStartDate().toString() +"\n" +
                                                  "DueDate:" + taskStringList.at(i).getDueDate().toString());
            }
        }
    }
    return choosenTaskId;
}
void MainScreen::verifyEditTask(QString editTask,QString choosenTaskId,QString key)
{
    if(editTask.compare("true") == 0)
    {
        QString task = c->sendRequest("taskRequest|", key);
        //Split the string with a delimiter
        TaskList taskStringList = Task::stringToList(task);
        ui->testCaseShow->appendPlainText("After Edit");
        for(int i = 0; i < taskStringList.size(); i++)
        {
            if(taskStringList.at(i).getId().compare(choosenTaskId)==0)
            {
                ui->testCaseShow->appendPlainText("Task ID: " + taskStringList.at(i).getId() + "\n" +
                                                  "CourseID: " + taskStringList.at(i).getCourseID() + "\n"+
                                                  "Description: " + taskStringList.at(i).getDescription() + "\n"+
                                                  "StartDate: " + taskStringList.at(i).getStartDate().toString() +"\n" +
                                                  "DueDate:" + taskStringList.at(i).getDueDate().toString());
            }
        }
        ui->testCaseShow->appendPlainText("Pass");
        ui->testCaseShow->appendPlainText("____________________________________");

    }
    else
    {
        ui->testCaseShow->appendPlainText("Fail to Edit Task");
    }
}

void MainScreen::verifyDeleteTask(QString deleteTask,QString key)
{
    if(deleteTask.compare("true") == 0)
    {

        QString task = c->sendRequest("taskRequest|", key);
        ui->testCaseShow->appendPlainText("After Delete");
        verifyTask(task);
    }
    else
    {
        ui->testCaseShow->appendPlainText("Fail to Delete Task");
    }
}

QString MainScreen::verifyOpenEval(QString task)
{
    QString choosenTaskId;
    //Split the string with a delimiter
    TaskList taskStringList = Task::stringToList(task);

    taskList = taskStringList;

    if(task.compare("false") == 0)
    {
        ui->testCaseShow->clear();
        ui->testCaseShow->appendPlainText("No Tasks, Can't' Edit, all tests return false");
    }
    else
    {
        choosenTaskId = taskList.at(0).getId();
        for(int i = 0; i < taskStringList.size(); i++)
        {
            if(taskStringList.at(i).getId().compare(choosenTaskId)==0)
            {
                ui->testCaseShow->appendPlainText("Task ID: " + taskStringList.at(i).getId() + "\n" +
                                                  "CourseID: " + taskStringList.at(i).getCourseID() + "\n"+
                                                  "Description: " + taskStringList.at(i).getDescription() + "\n"+
                                                  "StartDate: " + taskStringList.at(i).getStartDate().toString() +"\n" +
                                                  "DueDate: " + taskStringList.at(i).getDueDate().toString() + "\n" +
                                                  "EvalRate: " + QString::number(taskStringList.at(i).getEvaluation()->getRating()) + "\n" +
                                                  "EvalFeedBack: " + taskStringList.at(i).getEvaluation()->getFeedback());
            }
        }
    }
    return choosenTaskId;
}


void MainScreen::verifyEditEval(QString eval,QString key,QString choosenTaskId)
{
    if(eval.compare("true") == 0)
    {
        QString task = c->sendRequest("taskRequest|", key);
        TaskList taskStringList = Task::stringToList(task);
        ui->testCaseShow->appendPlainText("After Edit the Evaluation: ");
        for(int i = 0; i < taskStringList.size(); i++)
        {
            if(taskStringList.at(i).getId().compare(choosenTaskId)==0)
            {
                ui->testCaseShow->appendPlainText("Task ID: " + taskStringList.at(i).getId() + "\n" +
                                                  "CourseID: " + taskStringList.at(i).getCourseID() + "\n"+
                                                  "Description: " + taskStringList.at(i).getDescription() + "\n"+
                                                  "StartDate: " + taskStringList.at(i).getStartDate().toString() +"\n" +
                                                  "DueDate: " + taskStringList.at(i).getDueDate().toString() + "\n" +
                                                  "EvalRate: " + QString::number(taskStringList.at(i).getEvaluation()->getRating()) + "\n" +
                                                  "EvalFeedBack: " + taskStringList.at(i).getEvaluation()->getFeedback());
            }
        }
        ui->testCaseShow->appendPlainText("Pass");
        ui->testCaseShow->appendPlainText("________________________________");
    }
    else
    {
        ui->testCaseShow->appendPlainText("Fail to Edit the Evaluation Data");
    }
}

void MainScreen::verifyLogOut(QString logOut)
{
    if(logOut.compare("true") == 0)
    {
        ui->testCaseShow->appendPlainText("User logout Successfully");
        ui->testCaseShow->appendPlainText("Pass");
    }
    else
    {
        ui->testCaseShow->appendPlainText("Fail to logout");
    }
}


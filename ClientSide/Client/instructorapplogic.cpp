#include "instructorapplogic.h"

/***********************************************
 ***********************************************
 ***********************************************
                  Constructor
 ***********************************************
 ***********************************************
 ***********************************************/

InstructorAppLogic::InstructorAppLogic(QObject *parent, Ui::MainScreen *uiScreen, ClientConnection *cConnection) :
    QObject(parent)
{
    //Just to set up the message box settings, I needed a spacer item to resize the windows of the
    //message box.
    QSpacerItem* spacer = new QSpacerItem(200, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGridLayout* layout = (QGridLayout*)newMessage.layout();
    layout->addItem(spacer, layout->rowCount(), 0, 1, layout->columnCount());
    newMessage.setWindowTitle("TAEval");

    //Assign the GUI and connection objects to this object's pointers
    ui = uiScreen;
    c = cConnection;

    //Connect all the Instructor related GUI components to this Application layer
    connect(ui->semesterBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_semesterBox_currentIndexChanged()));
    connect(ui->coursesBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_coursesBox_currentIndexChanged()));
    connect(ui->tasBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_tasBox_currentIndexChanged()));
    connect(ui->viewTaskListButton, SIGNAL(clicked()), this, SLOT(on_viewTaskListButton_clicked()));
    connect(ui->taskList, SIGNAL(clicked(QModelIndex)), this, SLOT(on_taskList_clicked()));
    connect(ui->backToInstructorsPage, SIGNAL(clicked()), this, SLOT(on_backToInstructorsPage_clicked()));
    connect(ui->createTaskButton, SIGNAL(clicked()), this, SLOT(on_createTaskButton_clicked()));
    connect(ui->editTaskButton, SIGNAL(clicked()), this, SLOT(on_editTaskButton_clicked()));
    connect(ui->deleteTaskButton, SIGNAL(clicked()), this, SLOT(on_deleteTaskButton_clicked()));
    connect(ui->viewEvaluation, SIGNAL(clicked()), this, SLOT(on_viewEvaluation_clicked()));
    connect(ui->taskSave, SIGNAL(clicked()), this, SLOT(on_taskSave_clicked()));
    connect(ui->editTaskSave, SIGNAL(clicked()), this, SLOT(on_editTaskSave_clicked()));
    connect(ui->evalSave, SIGNAL(clicked()), this, SLOT(on_evalSave_clicked()));
    connect(ui->taskCancel, SIGNAL(clicked()), this, SLOT(on_taskCancel_clicked()));
    connect(ui->editTaskCancel, SIGNAL(clicked()), this, SLOT(on_editTaskCancel_clicked()));
    connect(ui->evalCancel, SIGNAL(clicked()), this, SLOT(on_evalCancel_clicked()));

}

/***********************
 *****Deconstructor*****
 ***********************/

InstructorAppLogic::~InstructorAppLogic(){}

/***********************************************
 ***********************************************
 ***********************************************
             QString/QList Converters
 ***********************************************
 ***********************************************
 ***********************************************/

// Parse a list of QStrings to a single QString
QString InstructorAppLogic::listToString(StringList list){
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

// Convert a single QString to a list of QStrings
StringList InstructorAppLogic::stringToList(QString aString){
    StringList result = StringList();
    QList<QString> list = aString.split(QRegExp("(~`|`~|~~)"));
    for(int i=1;i<list.length()-1;i++) result += list[i];
    return result;
}


/***********************************************
 ***********************************************
 ***********************************************
          API Calls Instructor Side
 ***********************************************
 ***********************************************
 ***********************************************/

/**
  Function call to initialize the instructor's screen
  **/
void InstructorAppLogic::initialize(QString userName){

    //Save the current username temporarily
    userNameString = userName;

    //Retrieve all semesters for this instructor
    getSemesters();

    //Go to the instructor page
    ui->MainWidget->setCurrentIndex(1);
}


/**
  Function call to get all the semesters applicable for the instructor
  **/
void InstructorAppLogic::getSemesters(){

    //Reset all data for the semester combo box
    ui->semesterBox->clear();
    ui->semesterBox->addItem("<Please select one>");

    //Request the list of semesters
    QString semester = c->sendRequest("semesterRequest|", userNameString);

    //Split the string with a delimiter
    StringList semesterList = stringToList(semester);

    //Adding some items to the semester comboBox if there are any
    if(semester.compare("false") == 0){
        ui->semesterBox->clear();
        ui->semesterBox->addItem(QString("<N/A>"));
        ui->semesterBox->setEnabled(false);
    } else {
        ui->semesterBox->addItems(semesterList);
        ui->semesterBox->setEnabled(true);
    }

    //Set the username GUI label to the user's name
    ui->nameLabel->setText(userNameString);

    //Save the current selected semester temporarily
    semesterString = ui->semesterBox->currentText();

    //Turn to the semester page
    ui->InfoWidget->setCurrentIndex(0);

}

/**
  Function call to get all the courses applicable for the instructor at a given semester
  **/
void InstructorAppLogic::getCourses(){

    //Reset all data for the courses combo box
    ui->coursesBox->clear();
    ui->coursesBox->addItem("<Please select one>");

    //Send a Courses request with the entered username and semester
    QString courses = c->sendRequest("coursesRequest|", userNameString + "|" +
                                         semesterString);

    //Split the string with a delimiter
    CourseList coursesStringList = Course::stringToList(courses);

    //If there are no courses for the given semester then just put N/A for the courses Box
    //otherwise add the courses to the course Box
    if(courses.compare("false") == 0){
        ui->coursesBox->clear();
        ui->coursesBox->addItem(QString("<N/A>"));
        ui->coursesBox->setEnabled(false);
    } else {

        //Appending all the course names to the coursesBox
        for(int i = 0; i < coursesStringList.length(); i++){

            //Creating temporary course object so it's easier to access the course information
            Course c1(coursesStringList.at(i));
            ui->coursesBox->addItem(QString(c1.getName() + c1.getNumber()));
            ui->coursesBox->setEnabled(true);

        }
    }

    //Save the current selected course temporarily
    courseString = ui->coursesBox->currentText();
}

/**
  Function call to get all the tas applicable for the instructor at a given semester for a selected course in a7
  given semester
  **/
void InstructorAppLogic::getTAs(){

    //Reset all data for the TAs combo box
    ui->tasBox->clear();
    ui->tasBox->addItem("<Please select one>");

    //Send a TAs request with the entered username
    QString tas = c->sendRequest("tasRequest|", userNameString + "|" + courseString + semesterString);

    //Split the string with a delimiter
    TAList tasStringList = TA::stringToList(tas);

    //If there are no TAs for the given course in a given semester then just put N/A for the TAs Box
    //otherwise add the TAs to the TAs Box
    if(tas.compare("false") == 0){
        ui->tasBox->clear();
        ui->tasBox->addItem(QString("<N/A>"));
        ui->tasBox->setEnabled(false);
    } else {

        //Appending all the course names to the tasBox
        for(int i = 0; i < tasStringList.length(); i++){

            //Creating temporary TA object so it's easier to access the TA information
            TA ta(tasStringList.at(i));
            ui->tasBox->addItem(QString(ta.getId()));
            ui->tasBox->setEnabled(true);

        }
    }

    //Save the current selected TA temporarily
    taString = ui->tasBox->currentText();

}

/**
  To go to the tasks page and process all the task and display them on the table.
  **/
void InstructorAppLogic::viewTaskPage(){

    //Send a Task request with the entered username
    QString task = c->sendRequest("taskRequest|", userNameString + "|" +
                                       courseString + semesterString
                                       + "|" +taString);

    //Update the GUI's labels with some relevant information
    ui->navig4->setText(semesterString + ">" + courseString + ">" + taString + ">Tasks");
    ui->assignedLabel->setText("Tasks assigned to " + ui->tasBox->currentText());

    //Split the string with a delimiter
    TaskList taskStringList = Task::stringToList(task);

    //Setting up the taskList model to organize it in a certain way
    //such as 4 columns and the number of rows needed
    taskModel = new QStandardItemModel;
    taskModel->setColumnCount(4);
    taskModel->setRowCount(taskStringList.length());

    //Setting up my custom header labels
    QStringList *headerLabel = new QStringList();
    headerLabel->clear();
    headerLabel->append("TaskID");
    headerLabel->append("Description");
    headerLabel->append("Start Date");
    headerLabel->append("Due Date");

    //Assigning the table my default header labels
    taskModel->setHorizontalHeaderLabels(*headerLabel);

    //Configuring my header labels sizes and etc.
    ui->taskList->horizontalHeader()->setVisible(true);
    ui->taskList->setColumnWidth(0,50);
    ui->taskList->setColumnWidth(1,150);
    ui->taskList->setColumnWidth(2,89);
    ui->taskList->setColumnWidth(3,90);
    ui->taskList->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

    //Saving the tasks informations to a temporary variable so that other functions can
    //have access to the tasks when needed
    taskList = taskStringList;

    //If there are no Tasks for the given TA in a given semester then just disable all the relevant components
    //on the tasks page except for create button otherwise upload the tasks to the taskModel
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
        for(int i = 0; i < taskStringList.length(); i++){

            //Create temporary object for easier access to its information
            Task task1(taskStringList.at(i).getTaID(),
                       taskStringList.at(i).getCourseID(),
                       taskStringList.at(i).getDescription(),
                       taskStringList.at(i).getStartDate(),
                       taskStringList.at(i).getDueDate(),
                       taskStringList.at(i).getId(),
                       taskStringList.at(i).getEvaluation()->getRating(),
                       taskStringList.at(i).getEvaluation()->getFeedback());

            taskModel->setItem(i, 0, new QStandardItem(QString::number(i)));
            taskModel->setItem(i, 1, new QStandardItem(task1.getDescription().replace("\r", " \n")));
            taskModel->setItem(i, 2, new QStandardItem((task1.getStartDate().toString(Qt::ISODate))));
            taskModel->setItem(i, 3, new QStandardItem((task1.getDueDate()).toString(Qt::ISODate)));
        }
    }

    //Set the task table's model to taskModel
    ui->taskList->setModel(taskModel);

    //Just to set the default row if there's at least one item
    if(taskModel->rowCount() > 0){
        ui->taskList->selectRow(0);
        on_taskList_clicked();
    }



    //Turn to the tasks page
    ui->InfoWidget->setCurrentIndex(1);

}

/**
  Function call to save a task to the database
  **/
QString InstructorAppLogic::saveATask(){

    //Before sending the dutyTextFixed text, I need to replace \n with \r because the \n
    //character crashes the server when trying to process it.
    QString dutyTextFixed = ui->dutyText->toPlainText().replace("\n", "\r");

    //Send API request to save the task
    QString taskSuccess = c->sendRequest("taskCreateRequest|", userNameString + "|" +
                                       taString + "|" + courseString + semesterString + "|" +
                                            dutyTextFixed + "|" +
                                            ui->beginDate->text() + "|" +
                                            ui->dueDate->text());
    return taskSuccess;
}

/**
  Function call to edit a task from the database
  **/
QString InstructorAppLogic::editATask(){

    //Before sending the dutyTextFixed text, I need to replace \n with \r because the \n
    //character crashes the server when trying to process it.
    QString dutyTextFixed = ui->dutyText->toPlainText().replace("\n", "\r");

    //Saving the dates to temporary variables
    begDateEd = ui->beginDateEd->date();
    dueDateEd = ui->dueDateEd->date();

    //Send API request to edit the task
    QString editTask = c->sendRequest("taskEditRequest|", userNameString + "|" +
                                              dutyTextFixed + "|" + ui->beginDateEd->text() + "|" +
                                              ui->dueDateEd->text() + "|" + currentTaskID);

    return editTask;
}

/**
  Function call to delete a task from the database
  **/
QString InstructorAppLogic::deleteATask(){

    //Send API request to delete the selected task
    QString deleteTask = c->sendRequest("taskDeleteRequest|", userNameString + "|" +currentTaskID);

    return deleteTask;
}

/**
  Function call to evaluate a task from the database
  **/
QString InstructorAppLogic::evaluateATask(){

    feedback = ui->feedbackText->toPlainText();
    rating = ui->taskRating->currentText();

    //Before sending the feedbackFixed text, I need to replace \n with \r because the \n
    //character crashes the server when trying to process it.
    QString feedbackFixed = ui->feedbackText->toPlainText().replace("\n", "\r");

    //Send API request to evaluate the task
    QString eval = c->sendRequest("editEvalRequest|", userNameString + "|" +
                                          currentTaskID + "|" + feedbackFixed + "|" + rating);

    return eval;
}


/***********************************************
 ***********************************************
 ***********************************************
          Signal/Slots Instructor Side
 ***********************************************
 ***********************************************
 ***********************************************/

/**
  Whenever the semesterBox's index changes, this function handles the event
  **/
void InstructorAppLogic::on_semesterBox_currentIndexChanged(){

    //Save the new semester selected to a temporary variable
    semesterString = ui->semesterBox->currentText();

    //If there are no semesters then just set the other boxes below it to invisible
    //Otherwise call the getCourses function to get the courses for the selected semester
    if(ui->semesterBox->currentIndex() == 0){
        ui->navigLabel->setText("");
        ui->coursesBox->setVisible(false);
        ui->coursesLabel->setVisible(false);
        ui->tasBox->setVisible(false);
        ui->tasLabel->setVisible(false);
        ui->viewTaskListButton->setVisible(false);
    }else if (ui->semesterBox->currentIndex() == -1){
        //If there's no current index then do nothing
    } else {
        ui->navigLabel->setText(semesterString);
        ui->coursesLabel->setVisible(true);
        ui->coursesBox->setVisible(true);
        getCourses();
    }

}

/**
  Whenever the coursesBox's index changes, this function handles the event
  **/
void InstructorAppLogic::on_coursesBox_currentIndexChanged(){

    //Save the new course selected to a temporary variable
    courseString = ui->coursesBox->currentText();

    //If there are no courses then just set the other boxes below it to invisible
    //Otherwise call the getTAs function to get the TAs for the selected courses in a given semester
    if(ui->coursesBox->currentIndex() == 0){
        ui->navigLabel->setText(semesterString);
        ui->tasBox->setVisible(false);
        ui->tasLabel->setVisible(false);
        ui->viewTaskListButton->setVisible(false);
    }else if (ui->semesterBox->currentIndex() == -1){
        //If there's no current index then do nothing
    } else {
    ui->navigLabel->setText(semesterString + ">" + courseString);
        ui->tasLabel->setVisible(true);
        ui->tasBox->setVisible(true);
        getTAs();
    }

}

/**
  Whenever the taBox's index changes, this function handles the event
  **/
void InstructorAppLogic::on_tasBox_currentIndexChanged(){

    //Save the new TA selected to a temporary variable
    taString = ui->tasBox->currentText();

    //If there are no TAs then just set the view tasks button to invisible
    //Otherwise set the view tasks button to visible
    if(ui->tasBox->currentIndex() == 0 || ui->tasBox->currentText().compare("<N/A>") == 0){
       ui->navigLabel->setText(semesterString + ">" + courseString);
        ui->viewTaskListButton->setVisible(false);
    }else if (ui->semesterBox->currentIndex() == -1){
        //If there's no current index then do nothing
    } else {
        ui->navigLabel->setText(semesterString + ">" + courseString + ">" + taString);
        ui->viewTaskListButton->setVisible(true);
    }

}

/**
  Whenever the "Get Tasks" button is clicked, this function handles the event
  **/
void InstructorAppLogic::on_viewTaskListButton_clicked(){
    viewTaskPage();
}

/**
  Whenever a table's item has been clicked on, this function handles the event
  **/
void InstructorAppLogic::on_taskList_clicked(){
    //Save the selected index to currentID for other functions to be aware of the newly selected task
    currentRow = ui->taskList->selectionModel()->currentIndex().row();
    currentTaskID = taskList.at(currentRow).getId();
}

/**
  Whenever the "Back" button from the tasks page is clicked, this function handles the event
  **/
void InstructorAppLogic::on_backToInstructorsPage_clicked(){
    //Go back to the semester/course/tas selection page
    ui->InfoWidget->setCurrentIndex(0);
}

/**
  Whenever the "Create Task" button is clicked, this function handles the event
  **/
void InstructorAppLogic::on_createTaskButton_clicked(){
    //Just set up the create a task page with default settings/inputs
    ui->navig6->setText(semesterString + ">" + courseString + ">" + taString + ">Tasks>Create a Task");
    ui->taskIDCr->setText(QString::number(taskModel->rowCount()));
    ui->beginDate->setDate(QDate(2013, 9, 2));
    ui->dueDate->setDate(QDate(2013, 9, 2));
    ui->dutyText->clear();
    ui->InfoWidget->setCurrentIndex(3);
}

/**
  Whenever the "Edit Task" button is clicked, this function handles the event
  **/
void InstructorAppLogic::on_editTaskButton_clicked(){

    QString taskID = QString::number(currentRow);

    //Just set up the edit a task page with default settings/inputs
    ui->navig7->setText(semesterString + ">" + courseString + ">" + taString + ">Tasks>Task #" + taskID + ">Edit Task");
    ui->InfoWidget->setCurrentIndex(4);
    ui->taskIDEd->setText(taskID);
    ui->beginDateEd->setDate(taskList.at(currentRow).getStartDate());
    ui->dueDateEd->setDate(taskList.at(currentRow).getDueDate());
    ui->dutyTextEd->setPlainText(taskList.at(currentRow).getDescription());

}

/**
  Whenever the "Edit Evaluation" button is clicked, this function handles the event
  **/
void InstructorAppLogic::on_viewEvaluation_clicked(){

    QString feedbackFixed = taskList.at(currentRow).getEvaluation()->getFeedback();
    QString taskID = QString::number(currentRow);

    //Just set up the evaluate a task page with default settings/inputs
    ui->navig5->setText(semesterString + ">" + courseString + ">" + taString + ">Tasks>Task #" + taskID + ">Evaluation");
    ui->InfoWidget->setCurrentIndex(2);
    ui->taskID->setText(QString::number(currentRow));
    ui->taskRating->setCurrentIndex(taskList.at(currentRow).getEvaluation()->getRating());
    ui->feedbackText->setPlainText(feedbackFixed.replace("\r", " \n"));

}

/**
  Whenever the "Delete Task" button is clicked, this function handles the event
  **/
void InstructorAppLogic::on_deleteTaskButton_clicked(){

    //If deleting the task is a success then delete the task and acknowledge it to the user
    //Otherwise tell the user the deletion of the task has failed.
    if(deleteATask().compare("true") == 0){
        newMessage.setText("Task deleted.");
        newMessage.setStandardButtons(QMessageBox::Ok);
        newMessage.setIcon(QMessageBox::Information);
    } else {
        newMessage.setText("Task could not be deleted.");
        newMessage.setStandardButtons(QMessageBox::Ok);
        newMessage.setIcon(QMessageBox::Critical);
    }

    newMessage.exec();

    //Update and view the task list
    viewTaskPage();

}

/**
  Whenever the "Save" button from the create task page is clicked, this function handles the event
  **/
void InstructorAppLogic::on_taskSave_clicked(){

    //If creating the task is a success then create the task and acknowledge it to the user
    //Otherwise tell the user the creation of the task has failed.
    if(saveATask().compare("true") == 0){
        newMessage.setText("Task created.");
        newMessage.setStandardButtons(QMessageBox::Ok);
        newMessage.setIcon(QMessageBox::Information);
    } else {
        newMessage.setText("Task could not be created.");
        newMessage.setStandardButtons(QMessageBox::Ok);
        newMessage.setIcon(QMessageBox::Critical);
    }

    newMessage.exec();

    //Update and view the task list
    viewTaskPage();
}

/**
  Whenever the "Save" button from the edit task page is clicked, this function handles the event
  **/
void InstructorAppLogic::on_editTaskSave_clicked(){

    //If editing the task is a success then edit the task and acknowledge it to the user
    //Otherwise tell the user the attempt to edit the task has failed.
    if(editATask().compare("true") == 0){
        newMessage.setText("Changes saved.");
        newMessage.setStandardButtons(QMessageBox::Ok);
        newMessage.setIcon(QMessageBox::Information);
    } else {
        newMessage.setText("Changes could not be saved.");
        newMessage.setStandardButtons(QMessageBox::Ok);
        newMessage.setIcon(QMessageBox::Critical);
    }

    newMessage.exec();

    //Update and view the task list
    viewTaskPage();

}

/**
  Whenever the "Save" button from the evaluation page is clicked, this function handles the event
  **/
void InstructorAppLogic::on_evalSave_clicked(){

    //The task rating has to be assigned a valid number before saving and if the instructor
    //hasn't assigned a rating and tries to saves, then let him/her know with a message box
    if(ui->taskRating->currentText().toInt() > 0){

        //If evaluating the task is a success then evaluate the task and acknowledge it to the user
        //Otherwise tell the user the attempt to evaluate the task has failed.
        if(evaluateATask().compare("true") == 0){
            newMessage.setText("Evaluation saved.");
            newMessage.setStandardButtons(QMessageBox::Ok);
            newMessage.setIcon(QMessageBox::Information);
        } else {
            newMessage.setText("Evaluation could not be saved.");
            newMessage.setStandardButtons(QMessageBox::Ok);
            newMessage.setIcon(QMessageBox::Critical);
        }

        viewTaskPage();

    } else {
        newMessage.setText("You need to assign a rating before saving!");
        newMessage.setStandardButtons(QMessageBox::Ok);
        newMessage.setIcon(QMessageBox::Critical);
    }

    newMessage.exec();

}

/**
  Whenever the "Cancel" button from the create task page is clicked, this function handles the event
  **/
void InstructorAppLogic::on_taskCancel_clicked(){
    //Go back to the task list page
    ui->InfoWidget->setCurrentIndex(1);
}

/**
  Whenever the "Cancel" button from the edit task page is clicked, this function handles the event
  **/
void InstructorAppLogic::on_editTaskCancel_clicked(){
    //Go back to the task list page
    ui->InfoWidget->setCurrentIndex(1);
}

/**
  Whenever the "Cancel" button from the evaluation page is clicked, this function handles the event
  **/
void InstructorAppLogic::on_evalCancel_clicked(){
    //Go back to the task list page
    ui->InfoWidget->setCurrentIndex(1);
}

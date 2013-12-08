#include "taapplogic.h"

/***********************************************
 ***********************************************
 ***********************************************
                  Constructor
 ***********************************************
 ***********************************************
 ***********************************************/

TAAppLogic::TAAppLogic(QObject *parent, Ui::MainScreen *uiScreen, ClientConnection *cConnection) :
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

    //Connect all the TA related GUI components to this Application layer
    connect(ui->TAsemesterBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_TAsemesterBox_currentIndexChanged()));
    connect(ui->TATaskList, SIGNAL(clicked(QModelIndex)), this, SLOT(on_TATaskList_clicked()));
    connect(ui->TAviewTaskListButton, SIGNAL(clicked()), this, SLOT(on_TAviewTaskListButton_clicked()));
    connect(ui->backToTAPage, SIGNAL(clicked()), this, SLOT(on_backToTAPage_clicked()));
    connect(ui->viewTask, SIGNAL(clicked()), this, SLOT(on_viewTask_clicked()));
    connect(ui->TABackToTaskList, SIGNAL(clicked()), this, SLOT(on_TABackToTaskList_clicked()));
    connect(ui->TAViewEvaluation, SIGNAL(clicked()), this, SLOT(on_TAViewEvaluation_clicked()));
    connect(ui->TABackToTaskPage, SIGNAL(clicked()), this, SLOT(on_TABackToTaskPage_clicked()));

}

/***********************
 *****Deconstructor*****
 ***********************/

TAAppLogic::~TAAppLogic(){}

/***********************************************
 ***********************************************
 ***********************************************
             QString/QList Converters
 ***********************************************
 ***********************************************
 ***********************************************/

// Parse a list of QStrings to a single QString
QString TAAppLogic::listToString(StringList list){
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
StringList TAAppLogic::stringToList(QString aString){
    StringList result = StringList();
    QList<QString> list = aString.split(QRegExp("(~`|`~|~~)"));
    for(int i=1;i<list.length()-1;i++) result += list[i];
    return result;
}

/***********************************************
 ***********************************************
 ***********************************************
                API Calls TA Side
 ***********************************************
 ***********************************************
 ***********************************************/

/**
  Function call to initialize the TA's screen
  **/
void TAAppLogic::initialize(QString userName){

    //Save the current username temporarily
    userNameString = userName;

    //Retrieve all semesters for this TA
    getSemestersTA();

    //Go to the TA page
    ui->MainWidget->setCurrentIndex(2);
}

/**
  Function call to get all the semesters applicable for the TA
  **/
void TAAppLogic::getSemestersTA(){

    //Reset all data for the semester combo box
    ui->TAsemesterBox->clear();
    ui->TAsemesterBox->addItem("<Please select one>");

    //Request the list of semesters
    QString semester = c->sendRequest("semesterRequest|", userNameString);

    //Split the string with a delimiter
    StringList semesterList = stringToList(semester);

    //Adding some items to the semester comboBox if there are any
    if(semester.compare("false") == 0){
        ui->TAsemesterBox->clear();
        ui->TAsemesterBox->addItem(QString("<N/A>"));
        ui->TAsemesterBox->setEnabled(false);
    } else {
        ui->TAsemesterBox->addItems(semesterList);
        ui->TAsemesterBox->setEnabled(true);
    }

    //Set the username GUI label to the user's name
    ui->TAnameLabel->setText(userNameString);

    //Save the current selected semester temporarily
    semesterString = ui->TAsemesterBox->currentText();

    //Turn to the semester page
    ui->TAInfoWidget->setCurrentIndex(0);

}

/**
  Function call to get all the courses applicable for the TA at a given semester
  **/
void TAAppLogic::getCourseTA(){

    //Send a Courses request with the entered username and semester
    QString courses = c->sendRequest("coursesRequest|", userNameString + "|" +
                                         semesterString);

    //Split the string with a delimiter
    CourseList coursesStringList = Course::stringToList(courses);

    //Creating temporary course object so it's easier to access the course information
    Course c1(coursesStringList.at(0));

    //Set the course GUI label to the course the TA is assigned for the semester
    ui->TAcourse->setText(QString(c1.getName() + c1.getNumber()));
    ui->TAcourse->setEnabled(true);

    //Save the current selected course temporarily
    courseString = ui->TAcourse->text();
}

/**
  To go to the TA's tasks page and process all the task and display them on the table.
  **/
void TAAppLogic::viewTATaskPage(){

    //Send a Task request with the entered username
    QString task = c->sendRequest("taskRequest|", userNameString + "|" +
                                                  semesterString
                                       + "|" + userNameString);

    //Update the GUI's labels with some relevant information
    ui->TANavig2->setText(semesterString + ">" + courseString + ">Tasks");
    ui->TAAssignedLabel->setText("Tasks assigned to " + ui->TAnameLabel->text());

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
    ui->TATaskList->horizontalHeader()->setVisible(true);
    ui->TATaskList->setColumnWidth(0,50);
    ui->TATaskList->setColumnWidth(1,150);
    ui->TATaskList->setColumnWidth(2,89);
    ui->TATaskList->setColumnWidth(3,90);
    ui->TATaskList->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

    //Saving the tasks informations to a temporary variable so that other functions can
    //have access to the tasks when needed
    taskList = taskStringList;

    //If there are no Tasks for the given TA in a given semester then just disable all the relevant components
    //on the tasks page otherwise upload the tasks to the taskModel
    if(task.compare("false") == 0){
        ui->TATaskList->setEnabled(false);
    } else {
        ui->TATaskList->setEnabled(true);

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
            taskModel->setItem(i, 1, new QStandardItem(task1.getDescription()));
            taskModel->setItem(i, 2, new QStandardItem((task1.getStartDate().toString(Qt::ISODate))));
            taskModel->setItem(i, 3, new QStandardItem((task1.getDueDate()).toString(Qt::ISODate)));
        }
    }

    //Set the task table's model to taskModel
    ui->TATaskList->setModel(taskModel);

    //Just to set the default row if there's at least one item
    if(taskModel->rowCount() > 0){
        ui->TATaskList->selectRow(0);
        on_TATaskList_clicked();
    }

    //Turn to the tasks page
    ui->TAInfoWidget->setCurrentIndex(1);
}


/***********************************************
 ***********************************************
 ***********************************************
              Signal/Slots TA Side
 ***********************************************
 ***********************************************
 ***********************************************/

/**
  Whenever the semesterBox's index changes, this function handles the event
  **/
void TAAppLogic::on_TAsemesterBox_currentIndexChanged(){

    //Save the new semester selected to a temporary variable
    semesterString = ui->TAsemesterBox->currentText();

    //If there are no semesters then just set the other components below it to invisible
    //Otherwise call the getCourses function to get the course for the selected semester
    if(ui->TAsemesterBox->currentIndex() == 0){
        ui->TAcourse->setVisible(false);
        ui->TAcoursesLabel->setVisible(false);
        ui->TANavig->setText("");
        ui->TAviewTaskListButton->setVisible(false);
    }else if (ui->TAsemesterBox->currentIndex() == -1){
        //If there's no current index then do nothing
    } else {
        ui->TAcoursesLabel->setVisible(true);
        ui->TAcourse->setVisible(true);
        ui->TAviewTaskListButton->setVisible(true);
        getCourseTA();
        courseString = ui->TAcourse->text();
        ui->TANavig->setText(semesterString + ">" + courseString);
    }

}

/**
  Whenever the "Get Tasks" button is clicked, this function handles the event
  **/
void TAAppLogic::on_TAviewTaskListButton_clicked(){
    viewTATaskPage();
}

/**
  Whenever a table's item has been clicked on, this function handles the event
  **/
void TAAppLogic::on_TATaskList_clicked(){

    //Save the selected index to currentID for other functions to be aware of the newly selected task
    currentRow = ui->TATaskList->selectionModel()->currentIndex().row();
    currentTaskID = taskList.at(currentRow).getId();
    int rating = taskList.at(currentRow).getEvaluation()->getRating();

    //Disable the view evaluation button if there has been no rating yet, in other words, it hasn't been
    //evaluated yet otherwise enable the button
    if(rating > 0){
        ui->TAViewEvaluation->setEnabled(true);
    } else {
        ui->TAViewEvaluation->setEnabled(false);
    }

}

/**
  Whenever the "View Task" button is clicked, this function handles the event
  **/
void TAAppLogic::on_viewTask_clicked(){

    //Save some info in some temporary variables for shortcuts
    QString dutyTextFixed = taskList.at(currentRow).getDescription();
    QString taskID = QString::number(currentRow);

    //Populate the view task page with the relevant information
    ui->TANavig3->setText(semesterString + ">" + courseString + ">Tasks>Task #" + taskID);
    ui->TATaskID->setText(taskID);
    ui->TABegDate->setText(taskList.at(currentRow).getStartDate().toString("yyyy/MM/dd"));
    ui->TADueDate->setText(taskList.at(currentRow).getDueDate().toString("yyyy/MM/dd"));
    ui->TADuty->setText(dutyTextFixed.replace("\r", " \n"));

    //Go back to the task page
    ui->TAInfoWidget->setCurrentIndex(2);
}

/**
  Whenever the "View Evaluation" button is clicked, this function handles the event
  **/
void TAAppLogic::on_TAViewEvaluation_clicked(){

    QString taskID = QString::number(currentRow);
    int rating = taskList.at(currentRow).getEvaluation()->getRating();
    QString evaluation = taskList.at(currentRow).getEvaluation()->getFeedback();

    //Populate the evaluation page with the relevant information
    ui->TANavig4->setText(semesterString + ">" + courseString + ">Tasks>Task #" + taskID + ">Evaluation");
    ui->TATaskIDEv->setText(taskID);

    //Populate the evaluation data if there is any otherwise just put TBA (never happens though)
    //Because I set view evaluation button to disabled if there are no evaluation yet but this is
    //just to be safe
    if(rating > 0 ){
        ui->TARating->setText(QString::number(rating));
        ui->TAFeedback->setText(evaluation.replace("\r", " \n"));
    } else {
        ui->TARating->setText("TBA");
        ui->TAFeedback->setText("TBA");
    }

    //Go back to the task evaluation page
    ui->TAInfoWidget->setCurrentIndex(3);

}

/**
  Whenever the "Back" button from the *task list* page is clicked, this function handles the event
  **/
void TAAppLogic::on_backToTAPage_clicked(){
    //Go back to the semester selection page
    ui->TAInfoWidget->setCurrentIndex(0);
}

/**
  Whenever the "Back" button from the *task* page is clicked, this function handles the event
  **/
void TAAppLogic::on_TABackToTaskList_clicked(){
    viewTATaskPage();
    //Go back to the task list page
    ui->TAInfoWidget->setCurrentIndex(1);
}

/**
  Whenever the "Back" button from the evaluation page is clicked, this function handles the event
  **/
void TAAppLogic::on_TABackToTaskPage_clicked(){
    //Go back to the task list page
    viewTATaskPage();
    ui->TAInfoWidget->setCurrentIndex(1);
}

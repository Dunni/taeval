#include "taeval.h"

TAEval::TAEval()
    :dataStore(Storage::getInstance()),
     accessControl(AccessControl::getInstance())
{
    dataStore.connectToDB("taevalDBConnection");
//    dataStore.restore();
}

TAEval::~TAEval(){
    dataStore.disconnect();
}


bool TAEval::createTask(QString userID, QString TAUsername, QString CourseID, QString description, QDate startDate, QDate dueDate)
{
    if(accessControl.isLoggedIn(userID) && accessControl.getUser(userID)->getUserType() == "Instructor")
    {
        Task temp = Task(TAUsername,CourseID,description,startDate,dueDate);
        return dataStore.manageTask(userID, QString("create"), temp);
    }
    return false;
}

bool TAEval::editTask(QString userID, QString description, QDate startDate, QDate dueDate, QString taskId)
{
    if(accessControl.isLoggedIn(userID) && accessControl.getUser(userID)->getUserType() == "Instructor")
    {
        Task temp = Task(QString(""),QString(""),description,startDate,dueDate,taskId);
        return dataStore.manageTask(userID,QString("edit"), temp);
    }
    return false;

}

bool TAEval::deleteTask(QString userID, QString taskId)
{
    if(accessControl.isLoggedIn(userID) && accessControl.getUser(userID)->getUserType() == "Instructor")
    {
        Task temp = Task(QString(""),QString(""),QString(""),QDate(),QDate(),taskId);
        return dataStore.manageTask(userID, QString("delete"), temp);
    }
    return false;
}

bool TAEval::getCourses(QString userID, CourseList *&list, QString usr, QString term)
{
    if(accessControl.isLoggedIn(userID) && (accessControl.getUser(userID)->getUserType() == "Instructor" ||
                                           (accessControl.getUser(userID)->getUserType() == "TA" && userID==usr)))
    {
        return dataStore.getCourses(usr,term,list,accessControl.getUser(userID)->getUserType());
    }
    return false;
}

bool TAEval::getSemesters(QString userID, StringList *&list) // User MUST delete list after use as it is dynamically allocated
{
    if(accessControl.isLoggedIn(userID) && (accessControl.getUser(userID)->getUserType() == "Instructor" || accessControl.getUser(userID)->getUserType() == "TA"))
    {
        list = new StringList(dataStore.getSemesters(userID));
        return true;
    }
    return false;
}

bool TAEval::enterEvaluation(QString userID, qint32 taskId, QString feedback, qint32 rating)
{
    if(accessControl.isLoggedIn(userID) && accessControl.getUser(userID)->getUserType() == "Instructor")
    {
        return dataStore.enterEvaluation(taskId,rating,feedback);
    }
    return false;
}

bool TAEval::getEvaluation(QString userID, qint32 taskId, Task::Evaluation*& eval)
{
    if(accessControl.isLoggedIn(userID) && accessControl.getUser(userID)->getUserType() == "Instructor")
    {
        Task* temp;
        if(dataStore.getTask(QString::number(taskId),temp))
        {
            eval = new Task::Evaluation(temp->getEvaluation()->getRating(),temp->getEvaluation()->getFeedback());
            return true;
        }
    }
    return false;
}

bool TAEval::getTAs(QString userID, QString course, TAList *&list)
{
    if(accessControl.isLoggedIn(userID) && accessControl.getUser(userID)->getUserType() == "Instructor")
    {
        return dataStore.getTAsForCourse(course,list);
    }
    return false;
}

bool TAEval::getTasks(QString userID, QString course, QString ta, TaskList *&list)
{
    if(accessControl.isLoggedIn(userID) && (accessControl.getUser(userID)->getUserType() == "Instructor" ||
                                           (accessControl.getUser(userID)->getUserType() == "TA" && userID==ta)))
    {
        return dataStore.getTasksForTA(course,ta,list,accessControl.getUser(userID)->getUserType());
    }
    qDebug() << "no access";
    return false;
}

QString TAEval::listToString(StringList list){ // Parse a list of QStrings to a single QString
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

StringList TAEval::stringToList(QString aString){  // Convert a single QString to a list of QStrings
    StringList result = StringList();
    QList<QString> list = aString.split(QRegExp("(~`|`~|~~)"));
    for(int i=1;i<list.length()-1;i++) result += list[i];
    return result;
}

QString TAEval::serveRequest(QString request, QString data)
{
    //Then find the request here and delegate the data to the SQL to query the required info.
    if(request.compare("loginRequest") == 0){
       qDebug() << "Login Request being processed" << endl;
       QString l = accessControl.logIn(data);
       if(l.contains("Instructor"))
       {
           return QString("Instructor");
       }
       if(l.contains("TA"))
       {
           return QString("TA");
       }
       if(l.contains("loggedin"))
       {
           return QString("loggedin");
       }
       else
           return QString("false");

    }
    else if (request.compare("semesterRequest") == 0) {

        qDebug() << "Semester Request being processed" << endl;
        StringList *semesters;

        if(getSemesters(data, semesters))
        {
            QString result = listToString(*semesters);
            delete semesters;
            return result.isEmpty() ? "false" : result;
        }

    }
    else if (request.compare("coursesRequest") == 0) {

        qDebug() << "Courses Request being processed" << endl;
        QStringList info = data.split("|");
        qDebug() <<  info.at(0) + " " + info.at(1) << endl;
        CourseList* c;

        if(getCourses(info.at(0), c, info.at(0), info.at(1)))
        {
            QString stringOfCourses = Course::listToString(*c);
            qDebug() << stringOfCourses << endl;
            delete c;
            return (stringOfCourses.isEmpty()) ? "false" : stringOfCourses;
        }

    }
    else if (request.compare("tasRequest") == 0) {

        qDebug() << "TAs Request being processed" << endl;
        QStringList info = data.split("|");
        qDebug() << "Inst here is: " + info.at(0) + " and " + info.at(1) << endl;
        TAList* t;

        if(getTAs(info.at(0), info.at(1), t))
        {
            QString stringOfTAs = TA::listToString(*t);delete t;
            qDebug() << stringOfTAs << endl;
            return (stringOfTAs.isEmpty()) ? "false" : stringOfTAs;
        }

    }
    else if (request.compare("taskRequest") == 0) {

        qDebug() << "Task Request being processed" << endl;
        QStringList info = data.split("|");
        qDebug() << "Inst here is: " + info.at(0) + " and " + info.at(1) << " and " << info.at(2) << endl;
        TaskList* taskList;
        if(getTasks(info.at(0), info.at(1), info.at(2), taskList))
        {
            QString stringOfTask = Task::listToString(*taskList);
            delete taskList;
            qDebug() << stringOfTask << endl;
            return (stringOfTask.isEmpty()) ? "false" : stringOfTask;
        }
        else
            return "false";

    }
    else if (request.compare("taskCreateRequest") == 0) {
        qDebug() << "Create Task Request being processed" << endl;

        QStringList info = data.split("|");

        qDebug() << "Inst here is: " + info.at(0) + " and " +
                    info.at(1) << " and " << info.at(2) + " and " +
                 info.at(3) << " and " << info.at(4) << + " and " +
                 info.at(5) << endl;

        QStringList begDateList = info.at(4).split("-");
        QStringList dueDateList = info.at(5).split("-");

        QDate begDate(begDateList.at(0).toInt(), begDateList.at(1).toInt(), begDateList.at(2).toInt());
        QDate dueDate(dueDateList.at(0).toInt(), dueDateList.at(1).toInt(), dueDateList.at(2).toInt());

        return createTask(info.at(0), info.at(1), info.at(2), info.at(3), begDate, dueDate) ? "true" : "false";
    }
    else if (request.compare("taskEditRequest") == 0) {
        qDebug() << "Edit Task Request being processed" << endl;

        QStringList info = data.split("|");

        qDebug() << "Inst here is: " + info.at(0) + " and " +
                    info.at(1) + " and " + info.at(2) + " and " +
                    info.at(3) + "and " + info.at(4) << endl;

        QStringList begDateList = info.at(2).split("-");
        QStringList dueDateList = info.at(3).split("-");

        QDate begDate(begDateList.at(0).toInt(), begDateList.at(1).toInt(), begDateList.at(2).toInt());
        QDate dueDate(dueDateList.at(0).toInt(), dueDateList.at(1).toInt(), dueDateList.at(2).toInt());


        return editTask(info.at(0), info.at(1), begDate, dueDate, QString(info.at(4))) ? "true" : "false";
    }
    else if (request.compare("taskDeleteRequest") == 0){

        qDebug() << "DELETE Task Request being processed" << endl;
        QStringList info = data.split("|");
        qDebug() << "Inst here is: " + info.at(0) + " and " +
                    info.at(1) << endl;

        return deleteTask(info.at(0), QString(info.at(1))) ? "true" : "false";
    }
    else if (request.compare("editEvalRequest") == 0){

        qDebug() << "Edit Evail Request being processed" << endl;
        QStringList info = data.split("|");

        return enterEvaluation(info.at(0), info.at(1).toInt(), info.at(2), info.at(3).toInt()) ? "true" : "false";
    }
    else if (request.compare("logOutRequest") == 0){

        qDebug() << "LogOut Request being processed" << endl;
        QStringList info = data.split("|");
        qDebug() << "Inst here is: " + info.at(0) << endl;

        return accessControl.logOut(info.at(0)) ? "true" : "false";
    }

    qDebug() <<  "Request Failed" << endl;
    return "false";
}

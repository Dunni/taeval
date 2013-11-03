#include "taeval.h"

TAEval::TAEval(){
    dataStore = new Storage();
    dataStore->connectToDB("taevalDBConnection");
}

TAEval::~TAEval(){
    dataStore->disconnect();
    delete dataStore;
    QList<NonAdminUser*> temp = loggedOn.values();
    for(int i=0;i<temp.length();i++)
    {
        delete temp[i];
    }
}

QString TAEval::logIn(QString userid)
{
    QString role = verifyUser(userid);
    if(role == QString("TA"))
    {
        TA* temp = new TA(userid);
        loggedOn.insert(userid,temp);
    }
    else if(role == QString("Instructor"))
    {
        Instructor* temp = new Instructor(userid);
        loggedOn.insert(userid,temp);
    }
    else role = "invalid";
    return role;
}

bool TAEval::logOut(QString userid)
{
    if(loggedOn.contains(userid))
    {
        delete loggedOn.value(userid);
        loggedOn.remove(userid);
        return true;
    }
    return false;
}

bool TAEval::createTask(QString userID, QString TAUsername, QString CourseID, QString description, QDate startDate, QDate dueDate)
{
    if(loggedOn.contains(userID) && loggedOn.value(userID)->getUserType() == "Instructor")
    {
        Task temp = Task(TAUsername,CourseID,description,startDate,dueDate);
        return dataStore->manageTask(QString("create"), temp);
    }
    return false;
}

bool TAEval::editTask(QString userID, QString description, QDate startDate, QDate dueDate, QString taskId)
{
    if(loggedOn.contains(userID) && loggedOn.value(userID)->getUserType() == "Instructor")
    {
        Task temp = Task(QString(""),QString(""),description,startDate,dueDate,taskId);
        return dataStore->manageTask(QString("edit"), temp);
    }
    return false;

}

bool TAEval::deleteTask(QString userID, QString taskId)
{
    if(loggedOn.contains(userID) && loggedOn.value(userID)->getUserType() == "Instructor")
    {
        Task temp = Task(QString(""),QString(""),QString(""),QDate(),QDate(),taskId);
        return dataStore->manageTask(QString("delete"), temp);
    }
    return false;
}

bool TAEval::getCourses(QString userID, CourseList *&list, QString instructor, QString term)
{
    if(loggedOn.contains(userID) && loggedOn.value(userID)->getUserType() == "Instructor")
    {
        return dataStore->getCoursesTeaching(instructor,term,list);
    }
    return false;
}

bool TAEval::getSemesters(QString userID, StringList *&list) // User MUST delete list after use as it is dynamically allocated
{
    if(loggedOn.contains(userID) && loggedOn.value(userID)->getUserType() == "Instructor")
    {
        list = new StringList(dataStore->getSemesters(userID));
        return true;
    }
    return false;
}

bool TAEval::enterEvaluation(QString userID, qint32 taskId, QString feedback, qint32 rating)
{
    if(loggedOn.contains(userID) && loggedOn.value(userID)->getUserType() == "Instructor")
    {
        return dataStore->enterEvaluation(taskId,rating,feedback);
    }
    return false;
}

bool TAEval::getEvaluation(QString userID, qint32 taskId, Task::Evaluation*& eval)
{
    if(loggedOn.contains(userID) && loggedOn.value(userID)->getUserType() == "Instructor")
    {
        Task* temp;
        if(dataStore->getTask(QString::number(taskId),temp))
        {
            eval = new Task::Evaluation(temp->getEvaluation()->getRating(),temp->getEvaluation()->getFeedback());
            return true;
        }
    }
    return false;
}

bool TAEval::getTAs(QString userID, QString course, TAList *&list)
{
    if(loggedOn.contains(userID) && loggedOn.value(userID)->getUserType() == "Instructor")
    {
        return dataStore->getTAsForCourse(course,list);
    }
    return false;
}

bool TAEval::getTasks(QString userID, QString course, QString ta, TaskList *&list)
{
    if(loggedOn.contains(userID) && (loggedOn.value(userID)->getUserType() == "Instructor" || userID==ta))
    {
        return dataStore->getTasksForTA(course,ta,list);
    }
    return false;
}



QString TAEval::verifyUser(QString userid)
{
    QString result;
    dataStore->verifyUser(userid,result);
    return result;
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
    //qDebug() << list;
    return result;
}

QString TAEval::serveRequest(QString request, QString data)
{
    //Then find the request here and delegate the data to the SQL to query the required info.
    if(request.compare("loginRequest") == 0){
       qDebug() << "Login Request being processed" << endl;
       if(logIn(data).contains("Instructor"))
       {
           return QString("Instructor");
       }
       if(logIn(data).contains("TA"))
       {
           return QString("TA");
       }
       else
           return QString("false");

    }
    else if (request.compare("semesterRequest") == 0) {
        qDebug() << "Semester Request being processed" << endl;
        StringList *semesters;
        getSemesters(data, semesters);
        QString result = listToString(*semesters);
        delete semesters;
        return result.isEmpty() ? "false" : result;
    }
    else if (request.compare("coursesRequest") == 0) {
        qDebug() << "Courses Request being processed" << endl;
        QStringList info = data.split("|");
        qDebug() <<  info.at(0) + " " + info.at(1) << endl;
        CourseList* c;
        getCourses(info.at(0), c, info.at(0), info.at(1));
        QString stringOfCourses = Course::listToString(*c);
        qDebug() << stringOfCourses << endl;
        delete c;
        return (stringOfCourses.isEmpty()) ? "false" : stringOfCourses;
    }
    else if (request.compare("tasRequest") == 0) {
        qDebug() << "TAs Request being processed" << endl;

        QStringList info = data.split("|");

        qDebug() << "Inst here is: " + info.at(0) + " and " +
                    info.at(1) << endl;
        TAList* t;
        getTAs(info.at(0), info.at(1), t);
        QString stringOfTAs = TA::listToString(*t);
        delete t;
        qDebug() << stringOfTAs << endl;

        if(stringOfTAs.isEmpty()){
            stringOfTAs = QString("<empty>");
        }
        return (stringOfTAs.isEmpty()) ? "false" : stringOfTAs;

    }
    else if (request.compare("taskRequest") == 0) {
        qDebug() << "Task Request being processed" << endl;

        QStringList info = data.split("|");

        qDebug() << "Inst here is: " + info.at(0) + " and " +
                    info.at(1) << " and " << info.at(2) << endl;

        TaskList* taskList;
        getTasks(info.at(0), info.at(1), info.at(2), taskList);
        QString stringOfTask = Task::listToString(*taskList);
        delete taskList;
        qDebug() << stringOfTask << endl;

        return (stringOfTask.isEmpty()) ? "false" : stringOfTask;

    }
    else if (request.compare("taskCreateRequest") == 0) {
        qDebug() << "CREATE Task Request being processed" << endl;

        QStringList info = data.split("|");

        qDebug() << "Inst here is: " + info.at(0) + " and " +
                    info.at(1) << " and " << info.at(2) + " and " +
                 info.at(3) << " and " << info.at(4) << + " and " +
                 info.at(5) << endl;

        QStringList begDateList = info.at(4).split("-");
        QStringList dueDateList = info.at(5).split("-");

        QDate begDate(begDateList.at(0).toInt(), begDateList.at(1).toInt(), begDateList.at(2).toInt());
        QDate dueDate(dueDateList.at(0).toInt(), dueDateList.at(1).toInt(), dueDateList.at(2).toInt());


        return (createTask(info.at(0), info.at(1), info.at(2), info.at(3), begDate, dueDate)) ? "true" : "false";
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


        return (editTask(info.at(0), info.at(1), begDate, dueDate, QString(info.at(4)))) ? "true" : "false";

    }
    else if (request.compare("taskDeleteRequest") == 0){
        qDebug() << "DELETE Task Request being processed" << endl;

        QStringList info = data.split("|");

        qDebug() << "Inst here is: " + info.at(0) + " and " +
                    info.at(1) << endl;

         deleteTask(info.at(0), QString(info.at(1)));

    }
    else if (request.compare("editEvalRequest") == 0){
        qDebug() << "Edit Evail Request being processed" << endl;

        QStringList info = data.split("|");

        return (enterEvaluation(info.at(0), info.at(1).toInt(), info.at(2), info.at(3).toInt())) ? "true" : "false";
    }
    else if (request.compare("logOutRequest") == 0){
        qDebug() << "LogOut Request being processed" << endl;

        QStringList info = data.split("|");

        qDebug() << "Inst here is: " + info.at(0) << endl;

        return logOut(info.at(0)) ? "true" : "false";
    }

    else {
        qDebug() << "Invalid Request" << endl;
    }
    return "false";
}

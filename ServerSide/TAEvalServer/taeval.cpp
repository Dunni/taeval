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

bool TAEval::logIn(QString userid)
{
    QString role = verifyUser(userid);
    if(role == QString("TA"))
    {
        TA* temp = new TA(userid);
        loggedOn.insert(userid,temp);
        return true;
    }
    if(role == QString("Instructor"))
    {
        Instructor* temp = new Instructor(userid);
        loggedOn.insert(userid,temp);
        return true;
    }
    return false;
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

bool TAEval::getCourses(QString userID, CourseList* list, QString instructor, QString term)
{
    if(loggedOn.contains(userID) && loggedOn.value(userID)->getUserType() == "Instructor")
    {
        return dataStore->getCoursesTeaching(instructor,term,list);
    }
    return false;
}


bool TAEval::getTAs(QString userID, QString course, TAList* list)
{
    if(loggedOn.contains(userID) && loggedOn.value(userID)->getUserType() == "Instructor")
    {
        return dataStore->getTAsForCourse(course,list);
    }
    return false;
}

bool TAEval::getTasks(QString userID, QString course, QString ta, TaskList* list)
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

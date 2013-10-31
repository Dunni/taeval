#include "taeval.h"

TAEval::TAEval(){dataStore = new Storage();}
TAEval::~TAEval(){
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
        return dataStore->manageTask(QString("Create"), temp);
    }
    return false;
}

bool TAEval::editTask(QString userID, QString TAUsername, QString CourseID, QString description, QDate startDate, QDate dueDate){}
bool TAEval::deleteTask(){}
bool TAEval::getCourses( CourseList* list, QString instructor, QString term){}
bool TAEval::getTAs(QString course, TAList* list){}
bool TAEval::getTasks(QString course, QString ta, TaskList* list){}
QString TAEval::verifyUser(QString userid){}

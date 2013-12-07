#include "Storage.h"

bool Storage::connectToDB(QString connection, QString path, QString DBType){
    return Imp->connectToDB(connection,path,DBType);
}

void Storage::disconnect(){
    Imp->disconnect();
}

bool Storage::restore(QString path){
    return Imp->restore(path);
}

bool Storage::verifyUser(QString name, QString &role){
    return Imp->verifyUser(name,role);
}

bool Storage::manageTask(QString user, QString action, Task task){
    return Imp->manageTask(user,action,task);
}

bool Storage::getCourses(QString user, QString term, QList<Course> *&list,QString role){
    return Imp->getCourses(user,term,list,role);
}


bool Storage::getTAsForCourse(QString courseKey, QList<TA> *&list){
    return Imp->getTAsForCourse(courseKey,list);
}


bool Storage::getTasksForTA(QString info, QString TAKey, QList<Task> *&list, QString role){
    return Imp->getTasksForTA(info,TAKey,list,role);
}

QStringList Storage::getSemesters(QString user){
    return Imp->getSemesters(user);
}

bool Storage::getTask(QString taskID, Task *&rv){
    return Imp->getTask(taskID,rv);
}

bool Storage::enterEvaluation(INT TaskID, INT rating, QString feedback){
    return Imp->enterEvaluation(TaskID,rating,feedback);
}

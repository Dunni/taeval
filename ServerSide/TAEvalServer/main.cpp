#include <QtGui/QApplication>
#include "./Storage/Storage.h"
#include <iostream>
#include "taeval.h"
#include "server.h"
using namespace std;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /*
    QDate today = QDate(2013,9,28);
    QDate tomorrow = QDate(2013,10,28);
    QString x = QString("12345");
    Task* task = new Task(x,x,x,today,tomorrow,x);
    Task* task2 = new Task(task->toString());
    qDebug() << task2->toString();

  //  Storage s;
    Course pp = Course(QString("COMP3004W13"),QString("OO Software Engineering"),QString("3004"),QString("FALL2013"),QString("CLAUREND"));
    QList<Course> qw;
    qw.append(pp);
    qw.append(pp);
    qw.append(pp);
    qw.append(pp);
    QString res = Course::listToString(qw);
    QString d = "2013-11-20";
    QStringList dl = d.split("-");
    qDebug() << QDate(dl.at(0).toInt(),dl.at(1).toInt(),dl.at(2).toInt());
    Course::stringToList(res);
*/
    //TAEval s = TAEval();
   // QString r;
    //CourseList *l;
    //qDebug() << s.logIn("Christine");
    //qDebug() << s.logIn("Y");

//    QList<Task> ll;
//    Task t = Task("1","2","3",QDate(),QDate(),"4",100,"NAa");
//    ll += t;
//    ll.append(Task("11","2","3",QDate(),QDate(),"4",101,"NAb"));
//    ll.append(Task("111","2","3",QDate(),QDate(),"4",102,"NAc"));
//    QString re = Task::listToString(ll);

//    qDebug() << re;

  /*  Storage s;
    s.connectToDB("OO");
    qDebug() << s.createTask("Christine","Abdallah","COMP3004F2013","whatever","2013-10-10","2013-12-12");
    qDebug() << s.createTask("Christine1","Abdallah","COMP3004F2013","whatever","2013-10-10","2013-12-12");
    qDebug() << s.createTask("Christine","Abdallahahahahah","COMP3004F2013","whatever","2013-10-10","2013-12-12");
    s.disconnect();

    TaskList *l;
    Task *tt;
    s.getTasksForTA(QString("COMP3004F2012"),QString("Abdallah"),l);
    s.getTask("7",tt);
    qDebug() << tt->toString();
    s.disconnect();
    qDebug() << QDate(2013,10,19).toString(Qt::ISODate);
*/
    Server s;
    return a.exec();

}

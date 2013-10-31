#include <QtGui/QApplication>
#include "mainwindow.h"
#include "task.h"
#include "course.h"
#include "nonadminuser.h"
#include "./Storage/Storage.h"
#include "taeval.h"
#include <iostream>
using namespace std;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
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

    TAEval s = TAEval();
    QString r;
    CourseList *l;
    s.logIn("Christine");
    qDebug() << s.getCourses("Christine",l,"Christine","F2013");
    //s.disconnect();

    //return a.exec();
}

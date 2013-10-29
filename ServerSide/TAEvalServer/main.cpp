#include <QtGui/QApplication>
#include "mainwindow.h"
#include "task.h"
#include "course.h"
#include "nonadminuser.h"
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
    Task* task = new Task(x,x,x,x, today, tomorrow);
    cout << task->getCourseID().toStdString();

    return a.exec();
}

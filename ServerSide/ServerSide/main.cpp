#include <QtGui/QApplication>
#include "qmlapplicationviewer.h"
#include "Task.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    QDate today = QDate(2013,9,28);
    QDate tomorrow = QDate(2013,10,28);
    QString a = QString("12345");
    Task* task = new Task(a,a,a,a, today, tomorrow);
    cout << task->getCourseID().toStdString();
}

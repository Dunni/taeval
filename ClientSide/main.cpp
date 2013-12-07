#include <QtGui/QApplication>
#include "mainapplogic.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainAppLogic w;
    w.show();
    return a.exec();
}

#include <QtGui/QApplication>
#include "mainscreen.h"
#include "client.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainScreen w;
    w.show();
    return a.exec();
}

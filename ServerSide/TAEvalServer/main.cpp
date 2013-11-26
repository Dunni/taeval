#include <QtGui/QApplication>
#include "./Storage/Storage.h"
#include <iostream>
#include "taeval.h"
#include "server.h"
using namespace std;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ServerApp s;
    return a.exec();

}

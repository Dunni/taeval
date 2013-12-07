#include <QtGui/QApplication>
#include "./Storage/Storage.h"
#include <iostream>
#include "./Server/taeval.h"
#include "./Server/server.h"
using namespace std;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Server s;
    return a.exec();

}

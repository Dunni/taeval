#ifndef SERVER_H
#define SERVER_H
//#include "../../Common/connection.h"
#include "../../Common/serverconnection.h"
#include "taeval.h"
class ServerApp : public QObject
{
    Q_OBJECT
public:
    ServerApp(QObject *parent = 0);
private slots:
    void getRequest();
private:
//    Connection *connectionService;
    Server *s;
    TAEval model;


};

#endif

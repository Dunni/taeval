#ifndef SERVER_H
#define SERVER_H
#include "../Common/Communication/serverconnection.h"
#include "taeval.h"

class Server : public QObject
{
    Q_OBJECT
public:
    Server(QObject *parent = 0);
    ~Server();

private slots:
    void getRequest();

private:
    ServerConnection *s;
    TAEval model;


};

#endif

#include "accesscontrol.h"

AccessControl::AccessControl()
    :dataStore(Storage::getInstance())
{
}

AccessControl::~AccessControl()
{
    QList<NonAdminUser*> temp = loggedOn.values();
    for(int i=0;i<temp.length();i++)
    {
        delete temp[i];
    }
}

QString AccessControl::logIn(QString userid)
{
    if(isLoggedIn(userid)) return "invalid";
    QString role;
    dataStore.verifyUser(userid,role);
    if(role == QString("TA"))
    {
        TA* temp = new TA(userid);
        loggedOn.insert(userid,temp);
    }
    else if(role == QString("Instructor"))
    {
        Instructor* temp = new Instructor(userid);
        loggedOn.insert(userid,temp);
    }
    else role = "invalid";
    return role;
}

bool AccessControl::logOut(QString userid)
{
    if(loggedOn.contains(userid))
    {
        delete loggedOn.value(userid);
        loggedOn.remove(userid);
        return true;
    }
    return false;
}

bool AccessControl::isLoggedIn(QString userid)
{
    return loggedOn.contains(userid);
}

NonAdminUser* AccessControl::getUser(QString userid)
{
    return loggedOn.value(userid);
}

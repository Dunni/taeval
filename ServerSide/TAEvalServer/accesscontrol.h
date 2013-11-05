#ifndef ACCESSCONTROL_H
#define ACCESSCONTROL_H
#include "../../Common/nonadminuser.h"
#include "Storage/Storage.h"

typedef QMap<QString, NonAdminUser*> UserList;

class AccessControl
{
public:
    static AccessControl& getInstance(){
        static AccessControl ac;
        return ac;
    }
    ~AccessControl();

    QString logIn(QString);
    bool logOut(QString);
    bool isLoggedIn(QString);
    NonAdminUser* getUser(QString);

private:
    AccessControl();
    UserList loggedOn;
    Storage& dataStore;
};

#endif // ACCESSCONTROL_H

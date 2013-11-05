#include "ta.h"

TA::TA(QString id)
    :NonAdminUser(id)
{}

QString TA::getUserType(){return QString("TA");}

QList<TA> TA::stringToList(QString aString){  // Convert a string to a list of NonAdminUser objects
    TAList result = TAList();
    QList<QString> list = aString.split(QRegExp("(~`|`~|~~)"));
    for(int i=1;i<list.length()-1;i++) result += TA(list[i]);
    ////qDebug() << list;
    return result;
}

QString TA::listToString(TAList list)
{
    QString result = QString("");
    for(int i=0; i<list.length();i++)
    {
        if(i==0) result += "~`";
        result += list[i].toString();
        if(i==list.length()-1) result += "`~";
        else result += "~~";
    }
    return result;
}

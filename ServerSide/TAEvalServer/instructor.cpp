#include "instructor.h"

Instructor::Instructor(QString id)
    :NonAdminUser(id)
{}

QString Instructor::getUserType(){return QString("Instructor");}

QList<Instructor> Instructor::stringToList(QString aString){  // Convert a string to a list of NonAdminUser objects
    InstructorList result = InstructorList();
    QList<QString> list = aString.split(QRegExp("(~`|`~|~~)"));
    for(int i=1;i<list.length()-1;i++) result += Instructor(list[i]);
    //qDebug() << list;
    return result;
}

QString Instructor::listToString(InstructorList list)
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

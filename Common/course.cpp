#include "course.h"
using namespace std;

typedef QList<Course> CourseList;

Course::Course(QString aId, QString aName, QString aNumber, QString aSemester, QString aInstructorId)
    : id(aId),
      name(aName),
      number(aNumber),
      semester(aSemester),
      instructorId(aInstructorId)
{}

Course::Course(QString courseString)
{
    QList<QString> list = courseString.split(QRegExp("(:` |\\n`)"));
    if(list.length() != 10 ||
       list[0].compare(QString("CourseID")) != 0 ||
       list[2].compare(QString("CourseName")) != 0 ||
       list[4].compare(QString("CourseNumber")) != 0 ||
       list[6].compare(QString("CourseInstructor")) != 0 ||
       list[8].compare(QString("CourseSemester")) != 0 )
    {
        //qDebug() << "Parse Error! Invalid Input";
        return;
    }
    id = list[1];
    name = list[3];
    number = list[5];
    instructorId = list[7];
    semester = list[9];

}

QString Course::getId(){return id;}
QString Course::getName(){return name;}
QString Course::getNumber(){return number;}
QString Course::getSemester(){return semester;}
QString Course::getInstructorId(){return instructorId;}

void Course::setId(QString aId){this->id = aId;}
void Course::setName(QString aName){this->name = aName;}
void Course::setNumber(QString aNumber){this->number = aNumber;}
void Course::setSemester(QString aSemester){this->semester = aSemester;}
void Course::setInstructorId(QString aInstructorId){this->instructorId = aInstructorId;}

QString Course::toString(){
    return QString(QString() + "CourseID:` " + id + "\n`" +
                   "CourseName:` " + name + "\n`" +
                   "CourseNumber:` " + number + "\n`" +
                   "CourseInstructor:` " + instructorId + "\n`" +
                   "CourseSemester:` " + semester);
}

QString Course::listToString(CourseList list){ // Parse a list of Courses to a QString
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

CourseList Course::stringToList(QString aString){  // Convert a string to a list of course objects
    CourseList result = CourseList();
    QList<QString> list = aString.split(QRegExp("(~`|`~|~~)"));
    for(int i=1;i<list.length()-1;i++) result += Course(list[i]);
    ////qDebug() << list;
    return result;
}



























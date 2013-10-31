#include "course.h"
using namespace std;

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
        qDebug() << list;
        cout << "Parse Error! Invalid Input";
        return;
    }
    id = list[1];
    name = list[3];
    number = list[5];
    semester = list[7];
    instructorId = list[9];

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

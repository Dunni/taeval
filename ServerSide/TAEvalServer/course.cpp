#include "course.h"

Course::Course(QString aId, QString aName, QString aNumber, QString aSemester, QString aInstructorId)
    : id(aId),
      name(aName),
      number(aNumber),
      semester(aSemester),
      instructorId(aInstructorId)
{}

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
    return QString(QString() + "Course ID: " + id + "\n" +
                   "Course Name: " + name + "\n" +
                   "Course Number: " + number + "\n" +
                   "Course Instructor: " + instructorId + "\n" +
                   "Course Semester: " + semester);
}

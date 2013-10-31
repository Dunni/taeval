#ifndef COURSE_H
#define COURSE_H
#include <QtCore>
#include "iostream"


class Course
{
    typedef QList<Course> CourseList;
public:
    //Constructors
    Course(QString aId,QString aName, QString aNumber, QString aSemester, QString aInstructorId);
    Course(QString courseString);

    //Getters
    QString getId();
    QString getName();
    QString getNumber();
    QString getSemester();
    QString getInstructorId();

    //Setters
    void setId(QString);
    void setName(QString);
    void setNumber(QString);
    void setSemester(QString);
    void setInstructorId(QString);

    //Parsers
    QString toString(); //Parse single Course Object to QString
    static QString listToString(CourseList); // Parse a list of Courses to a QString
    static CourseList stringToList(QString); // Convert a string to a list of course objects

private:
    QString id, name, number, semester, instructorId;
};

#endif // COURSE_H

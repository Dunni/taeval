#ifndef COURSE_H
#define COURSE_H
#include <QtCore>

class Course
{
public:
    Course(QString aId,QString aName, QString aNumber, QString aSemester, QString aInstructorId);

    QString getId();
    QString getName();
    QString getNumber();
    QString getSemester();
    QString getInstructorId();

    void setId(QString);
    void setName(QString);
    void setNumber(QString);
    void setSemester(QString);
    void setInstructorId(QString);

private:
    QString id, name, number, semester, instructorId;
};

#endif // COURSE_H

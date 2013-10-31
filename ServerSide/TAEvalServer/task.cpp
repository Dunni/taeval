#include "task.h"

Task::Task(QString aTaID, QString aCourseID, QString aDescription, QDate start, QDate due, QString aTaskID, qint32 aRating, QString aFeedback)
    : id(aTaskID),
      description(aDescription),
      taID(aTaID),
      courseID(aCourseID),
      startDate(start),
      dueDate(due),
      eval(new Evaluation(aRating, aFeedback))
{}

Task::~Task()
{
    delete eval;
}

Task::Evaluation::Evaluation(qint32 aRating, QString aFeedback)
    : feedback(aFeedback),
      rating(aRating)
{}

Task::Evaluation::~Evaluation(){}

//Getters
QString Task::getId(){return id;}
QString Task::getCourseID(){return courseID;}
QString Task::getDescription(){return description;}
QString Task::getTaID(){return taID;}
QDate Task::getStartDate(){return startDate;}
QDate Task::getDueDate(){return dueDate;}
Task::Evaluation* Task::getEvaluation(){return eval;}
QString Task::Evaluation::getFeedback(){return this->feedback;}
qint32 Task::Evaluation::getRating(){return this->rating;}


//Setters
void Task::setCourseID(QString courseID){this->courseID = courseID;}
void Task::setId(QString id){this->id = id;}
void Task::setDescription(QString description){this->description = description;}
void Task::setTaID(QString taID){this->taID = taID;}
void Task::setStartDate(QDate startDate){this->startDate = startDate;}
void Task::setDueDate(QDate dueDate){this->dueDate = dueDate;}
void Task::setEvaluation(QString feedback, qint32 rating){
    this->eval->setFeedback(feedback);
    this->eval->setRating(rating);
}
void Task::Evaluation::setFeedback(QString aFeedback){this->feedback = aFeedback;}
void Task::Evaluation::setRating(qint32 aRating){this->rating = aRating;}

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


Task::Task(const Task& t)
    :id(t.getId()),
      description(t.getDescription()),
      taID(t.getTaID()),
      courseID(t.getCourseID()),
      startDate(t.getStartDate()),
      dueDate(t.getDueDate()),
      eval(new Evaluation(t.eval->getRating(),t.eval->getFeedback()))
{}

Task::Task(QString taskString)
{
    QList<QString> list = taskString.split(QRegExp("(:` |\\n`)"));
    if(list.length() != 16 ||
       list[0].compare(QString("Course ID")) != 0 ||
       list[2].compare(QString("Description")) != 0 ||
       list[4].compare(QString("TA")) != 0 ||
       list[6].compare(QString("Start Date")) != 0 ||
       list[8].compare(QString("Due Date")) != 0 ||
       list[10].compare(QString("Task ID")) != 0 ||
       list[12].compare(QString("Evaluation Feedback")) != 0 ||
       list[14].compare(QString("Evaluation Rating")) != 0)
    {
        //qDebug() << "Parse Error! Invalid Input";
        return;
    }
    courseID = list[1];
    description = list[3];
    taID = list[5];
    startDate = QDate::fromString(list[7]);
    dueDate = QDate::fromString(list[9]);
    id = list[11];
    eval = new Task::Evaluation(list[15].toInt(), list[13]);
}

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
QString Task::getId() const {return id;}
QString Task::getCourseID() const {return courseID;}
QString Task::getDescription() const{return description;}
QString Task::getTaID() const{return taID;}
QDate Task::getStartDate() const{return startDate;}
QDate Task::getDueDate() const{return dueDate;}
Task::Evaluation* Task::getEvaluation() const{return eval;}
QString Task::Evaluation::getFeedback() const{return this->feedback;}
qint32 Task::Evaluation::getRating() const{return this->rating;}


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


//Parsers
QString Task::toString()
{
    return QString(QString() + "Course ID:` " + courseID + "\n`" +
                   "Description:` " + description + "\n`" +
                   "TA:` " + taID + "\n`" +
                   "Start Date:` " + startDate.toString() + "\n`" +
                   "Due Date:` " + dueDate.toString() + "\n`" +
                   "Task ID:` " + id + "\n`" +
                   "Evaluation Feedback:` " + eval->getFeedback() + "\n`" +
                   "Evaluation Rating:` " + QString::number(eval->getRating()));
}

QString Task::listToString(TaskList list)
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

QList<Task> Task::stringToList(QString aString){  // Convert a string to a list of Task objects
    TaskList result = TaskList();
    QList<QString> list = aString.split(QRegExp("(~`|`~|~~)"));
    for(int i=1;i<list.length()-1;i++) result += Task(list[i]);
    ////qDebug() << list;
    return result;
}


#ifndef TASK_H
#define TASK_H
#include <QtCore>
using namespace Qt;


class Task
{
    typedef QList<Task> TaskList;
public:
    class Evaluation
    {
    public:
        Evaluation(qint32 aRating, QString aFeedback);
        ~Evaluation();
        QString getFeedback();
        qint32 getRating();
        void setRating(qint32 aRating);
        void setFeedback(QString aFeedback);

    private:
        QString feedback;
        qint32 rating;
    };

    //Constructors and Destructor
    Task(QString aTaID, QString aCourseID, QString aDescription, QDate start, QDate due, QString aTaskID="", qint32 aRating=-1, QString aFeedback="");
    Task(QString taskString);
    ~Task();

    //Getters
    QString getId();
    QString getDescription();
    QString getCourseID();
    QString getTaID();
    QDate getStartDate();
    QDate getDueDate();
    Evaluation* getEvaluation();

    //Setters
    void setId(QString id);
    void setDescription(QString);
    void setCourseID(QString);
    void setTaID(QString);
    void setStartDate(QDate);
    void setDueDate(QDate);
    void setEvaluation(QString, qint32);

   //Parsers
    QString toString();
    static QString listToString(TaskList);
    static TaskList stringToList(QString);

private:
    QString id;
    QString description, taID, courseID;
    QDate startDate, dueDate;
    Evaluation* eval;
};

#endif // TASK_H

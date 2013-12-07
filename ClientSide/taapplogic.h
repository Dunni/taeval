#ifndef TAAPPLOGIC_H
#define TAAPPLOGIC_H

#include "../Common/course.h"
#include "../Common/task.h"
#include "../Common/nonadminuser.h"
#include "../Common/ta.h"
#include "../Common/clientconnection.h"
#include "ui_mainscreen.h"

#include <QHBoxLayout>
#include <QMessageBox>
#include <QStandardItem>

typedef QList<TA> TAList;
typedef QList<Course> CourseList;
typedef QList<Task> TaskList;
typedef QList<QString> StringList;

namespace Ui {
class MainScreen;
}

class TAAppLogic : public QObject
{
    Q_OBJECT
    
public:
    explicit TAAppLogic(QObject *parent = 0, Ui::MainScreen *uiScreen = 0, Client *cConnection = 0);
    void initialize(QString userName);
    ~TAAppLogic();

private slots:

    //Slots
    void on_TAsemesterBox_currentIndexChanged();

    void on_TAviewTaskListButton_clicked();
    void on_TATaskList_clicked();
    void on_viewTask_clicked();
    void on_TAViewEvaluation_clicked();

    void on_backToTAPage_clicked();
    void on_TABackToTaskList_clicked();
    void on_TABackToTaskPage_clicked();

private:
    //Objects and Pointers
    Client *c;
    Ui::MainScreen *ui;
    QStandardItemModel *taskModel;
    TaskList taskList;
    QString userNameString, semesterString, courseString, currentTaskID, feedback, rating, duty;
    QDate begDate, dueDate, begDateEd, dueDateEd;
    QMessageBox newMessage;
    int currentRow;

    //API calls
    void viewTATaskPage();
    void getSemestersTA();
    void getCourseTA();

    //Converters
    StringList stringToList(QString);
    QString listToString(StringList);
};

#endif // TAAPPLOGIC_H

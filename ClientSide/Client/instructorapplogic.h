#ifndef INSTRUCTORAPPLOGIC_H
#define INSTRUCTORAPPLOGIC_H

#include "../Common/course.h"
#include "../Common/task.h"
#include "../Common/nonadminuser.h"
#include "../Common/ta.h"
#include "../Common/Communication/clientconnection.h"
#include "../Client/ui_mainscreen.h"

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

class InstructorAppLogic : public QObject
{
    Q_OBJECT
    
public:
    explicit InstructorAppLogic(QObject *parent = 0, Ui::MainScreen *uiScreen = 0, ClientConnection *cConnection = 0);
    void initialize(QString userName);
    ~InstructorAppLogic();
    
private slots:

    //Slots
    void on_semesterBox_currentIndexChanged();
    void on_coursesBox_currentIndexChanged();
    void on_tasBox_currentIndexChanged();

    void on_viewTaskListButton_clicked();
    void on_taskList_clicked();
    void on_backToInstructorsPage_clicked();

    void on_createTaskButton_clicked();
    void on_editTaskButton_clicked();
    void on_viewEvaluation_clicked();
    void on_deleteTaskButton_clicked();


    void on_taskSave_clicked();
    void on_editTaskSave_clicked();
    void on_evalSave_clicked();

    void on_taskCancel_clicked();
    void on_editTaskCancel_clicked();
    void on_evalCancel_clicked();

private:

    //Objects and Pointers
    ClientConnection *c;
    Ui::MainScreen *ui;
    QStandardItemModel *taskModel;
    TaskList taskList;
    QString userNameString, semesterString, courseString, taString, currentTaskID, feedback, rating, duty;
    QDate begDate, dueDate, begDateEd, dueDateEd;
    QMessageBox newMessage;
    int currentRow;

    //API calls
    void viewTaskPage();
    void getTAs();
    void getSemesters();
    void getCourses();
    QString saveATask();
    QString editATask();
    QString deleteATask();
    QString evaluateATask();

    //Converters
    StringList stringToList(QString);
    QString listToString(StringList);

};

#endif // INSTRUCTORAPPLOGIC_H

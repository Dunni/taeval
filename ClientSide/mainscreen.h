#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include <QDialog>
#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QStackedWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QDebug>
#include <QMessageBox>
#include <QStandardItem>
#include <QSplitter>
#include <QTreeView>
#include "../Common/course.h"
#include "../Common/task.h"
#include "../Common/nonadminuser.h"
#include "../Common/ta.h"
#include "../Common/clientconnection.h"

typedef QList<TA> TAList;
typedef QList<Course> CourseList;
typedef QList<Task> TaskList;
typedef QList<QString> StringList;

namespace Ui {
class MainScreen;
}

class MainScreen : public QDialog
{
    Q_OBJECT

public:
    explicit MainScreen(QWidget *parent = 0);
    ~MainScreen();

private slots:
    void sendError();
    void viewLoginScreen();
    void viewInstructorPage();
    void viewTaskPage();

    void getSemesters();
    void getCourses();
    void getTAs();

    void on_semesterBox_currentIndexChanged();
    void on_coursesBox_currentIndexChanged();
    void on_tasBox_currentIndexChanged();
    void on_viewTaskListButton_clicked();
    void on_logOutButton_clicked();
    void on_createTaskButton_clicked();
    void on_taskSave_clicked();
    void on_evalSave_clicked();
    void on_taskList_clicked();
    void on_viewEvaluation_clicked();
    void on_editTaskButton_clicked();
    void on_editTaskSave_clicked();
    void on_deleteTaskButton_clicked();
    void on_backToInstructorsPage_clicked();
    void on_evalCancel_clicked();
    void on_taskCancel_clicked();
    void on_editTaskCancel_clicked();

    //Test cases
    void switchToTestMode();
    void testCase_1();
    void testCase_2();
    void testCase_3();
    void testCase_4();
    void testCase_5();
    void testCase_6();
    void testCase_7();
    void testCase_8();
    void testCase_9();
    void testCase_10();

    //Parsers
    StringList stringToList(QString);
    QString listToString(StringList);


    //Test Cases Extracted Functions
    bool verifyUser(QString);
    bool verifySemester(QString);
    bool verifyCourse(QString);
    bool verifyTa(QString);
    bool verifyTask(QString);
    bool verifyCreateTask(QString,QString,QString,QString);
    bool verifyEditTask(QString,QString,QString);
    QString getChoosenID(QString);
    bool verifyDeleteTask(QString, QString);
    QString verifyOpenEval(QString);
    void printPass(bool);
    void verifyEditEval(QString,QString,QString);

    void printPassEval(QString);
    void verifyLogOut(QString);

private:
//    Connection *connectionService;
    Client *c;
    Ui::MainScreen *ui;
    QStandardItemModel *taskModel;
    TaskList taskList;
    QString userNameString;
    QString semesterString;
    QString courseString;
    QString taString;
    QString currentTaskID;
    QString feedback;
    QString rating;
    QDate begDate, dueDate, begDateEd, dueDateEd;
    QString duty;
};

#endif // MAINSCREEN_H

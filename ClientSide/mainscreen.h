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
#include "client.h"

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
    void viewEvaluationPage();
    void viewCreateTaskPage();
    void viewEditTaskPage();
    void getSemesters();
    void getCourses();
    void getTAs();
    void getTasks();

    void on_semesterBox_currentIndexChanged();
    void on_coursesBox_currentIndexChanged();
    void on_tasBox_currentIndexChanged();
    void on_viewTaskListButton_clicked();
    void on_backToTAsPage_clicked();
    void on_logOutButton_clicked();

    void on_createTaskButton_clicked();

    void on_taskSave_clicked();

    void on_evalSave_clicked();

    void on_taskList_clicked(const QModelIndex &index);

    void on_viewEvaluation_clicked();

    void on_editTaskButton_clicked();

    void on_editTaskSave_clicked();

    void on_deleteTaskButton_clicked();

private:
    Client *c;
    Ui::MainScreen *ui;
};

#endif // MAINSCREEN_H

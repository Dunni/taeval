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
    void instructorPage();
    void viewCoursesPage();
    void viewTAsPage();
    void viewTaskPage();
    void viewLoginScreen();
    void viewSemesterPage();
    void viewEvaluationPage();
    void viewCreateTaskPage();
    void viewEditTaskPage();

private:
    Client *c;
    Ui::MainScreen *ui;
};

#endif // MAINSCREEN_H

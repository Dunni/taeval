#ifndef MAINAPPLOGIC_H
#define MAINAPPLOGIC_H

#include <QDialog>
#include <QWidget>

#include "../Common/course.h"
#include "../Common/task.h"
#include "../Common/nonadminuser.h"
#include "../Common/ta.h"
#include "../Common/clientconnection.h"
#include "instructorapplogic.h"
#include "taapplogic.h"

namespace Ui {
class MainScreen;
}

class MainAppLogic : public QDialog
{
    Q_OBJECT

public:
    explicit MainAppLogic(QWidget *parent = 0);
    ~MainAppLogic();

private slots:
    void on_loginButton_clicked();
    void on_logOutButton_clicked();

private:
    Client *c;
    InstructorAppLogic *instructorApp;
    TAAppLogic *taApp;
    Ui::MainScreen *ui;
    QString userNameString;

};

#endif // MAINAPPLOGIC_H

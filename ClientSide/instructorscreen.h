#ifndef INSTRUCTORSCREEN_H
#define INSTRUCTORSCREEN_H

#include <QDialog>
#include <QListView>
#include <QStandardItemModel>
#include <QStandardItem>
#include "client.h"

namespace Ui {
class InstructorScreen;
}

class InstructorScreen : public QDialog
{
    Q_OBJECT
    
public:
    explicit InstructorScreen(QWidget *parent = 0);
    ~InstructorScreen();
    
private slots:
    void on_viewCourses_clicked();

private:
    Ui::InstructorScreen *ui;
};

#endif // INSTRUCTORSCREEN_H

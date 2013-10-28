#include "instructorscreen.h"
#include "ui_instructorscreen.h"


InstructorScreen::InstructorScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InstructorScreen)
{
    ui->setupUi(this);

    connect(ui->viewCourses, SIGNAL(clicked()), parent, SLOT(on_viewCourses_clicked()));

}

InstructorScreen::~InstructorScreen()
{
    delete ui;
}


void InstructorScreen::on_viewCourses_clicked()
{
    QStandardItem *item = new QStandardItem(tr("COMP 4004"));
    QStandardItemModel *model = new QStandardItemModel();

    model->appendRow(item);
    ui->courseList->setModel(model);
}

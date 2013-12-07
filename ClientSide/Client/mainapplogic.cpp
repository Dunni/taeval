#include "mainapplogic.h"
#include "ui_mainscreen.h"

/***********************
 ******Constructor******
 ***********************/

MainAppLogic::MainAppLogic(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainScreen)
{
    ui->setupUi(this);

    //Initialize client, instructorAppLogic, taAppLogic here
    c = new ClientConnection();
    instructorApp = new InstructorAppLogic(parent, ui, c);
    taApp = new TAAppLogic(parent, ui, c);

    //Set the username textfield as the main focus so that it's convenient for the user to type in its name right away
    ui->userName->setFocus();

    //Connecting the TALogoutButton to the on_logOutButton_clicked() slot
    connect(ui->TAlogOutButton, SIGNAL(clicked()), this, SLOT(on_logOutButton_clicked()));
    connect(c, SIGNAL(errorOccurs()), this, SLOT(connectionDisconnected()));
}

/***********************
 *****Deconstructor*****
 ***********************/

MainAppLogic::~MainAppLogic(){
    delete ui;
    delete c;
    delete instructorApp;
    delete taApp;
}

/***********************************************
 ********Slots/Signals for LogOn/LogOff*********
 ***********************************************/

void MainAppLogic::on_loginButton_clicked(){
    //Username textfield can't be empty!
    if(!ui->userName->text().isEmpty()){

        //Send a login request with the entered username
        QString role = c->sendRequest("loginRequest|", ui->userName->text());

        //Check if the login request was successful otherwise send an error message saying user doesn't exist.
        if(!(role.compare("false") == 0)){

            //If user exists then put the username on the top right corner label
            userNameString = ui->userName->text();

            //Find out what kind of user logged on, then call the correct initialize function
            if(role.compare("Instructor") == 0){
                instructorApp->initialize(userNameString);
            } else if (role.compare("TA") == 0){
                taApp->initialize(userNameString);
            }
        } else {
            QMessageBox::critical(this, tr("TAEval"), tr("The username is invalid! Please try again."));
        }
    } else {
        QMessageBox::critical(this, tr("TAEval"), tr("The username is blank! Please enter an username."));
    }
}

void MainAppLogic::on_logOutButton_clicked(){
    //Send a login request with the entered username
    QString logOut = c->sendRequest("logOutRequest|", userNameString);

    //Check if the logout was a fail, then let the user know
    if(logOut.compare("false") == 0){
        QMessageBox::critical(this,
                              tr("Log off Error!"),
                              tr("Couldn't log off."));
    } else {
        QMessageBox::information(this,
                              tr("Log off"),
                              tr("Logged off."));
        //Clear username textfield
        ui->userName->clear();

        //Set focus on the textfield for easier access
        ui->userName->setFocus();

        //Then go to the login page
        ui->MainWidget->setCurrentIndex(0);
    }
}

void MainAppLogic::connectionDisconnected(){

    QMessageBox::critical(this,
                          tr("Error!"),
                          tr("Server closed!"));
}

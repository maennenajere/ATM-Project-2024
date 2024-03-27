#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
    setWindowTitle("Kirjaudu");
}

Login::~Login()
{
    delete ui;
}
















void Login::on_pushButtonLogin_clicked()
{
    qDebug()<<"login.cpp: Painettu kirjaudu-nappia";
}

void Login::on_pushButtonLeave_clicked()
{
    this->close();
}


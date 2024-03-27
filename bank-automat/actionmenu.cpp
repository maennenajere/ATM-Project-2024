#include "actionmenu.h"
#include "ui_actionmenu.h"

Actionmenu::Actionmenu(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Actionmenu)
{
    ui->setupUi(this);
}

Actionmenu::~Actionmenu()
{
    delete ui;
}

#ifndef ACTIONMENU_H
#define ACTIONMENU_H

#include <QDialog>

namespace Ui {
class Actionmenu;
}

class Actionmenu : public QDialog
{
    Q_OBJECT

public:
    explicit Actionmenu(QWidget *parent = nullptr);
    ~Actionmenu();

private:
    Ui::Actionmenu *ui;
};

#endif // ACTIONMENU_H

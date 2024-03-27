#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();
    void setCardNumber(const QString &cardNumber);

private slots:
    void on_pushButtonLeave_clicked();

    void on_pushButtonLogin_clicked();

private:
    Ui::Login *ui;
    QString cardNumber;
    QByteArray pinNumber;
};

#endif // LOGIN_H

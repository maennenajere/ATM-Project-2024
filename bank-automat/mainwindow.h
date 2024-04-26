#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <iostream>
#include <QMessageBox>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "dll_rfid.h"
#include "../DLL_reqs/reqs.hpp"
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void openSerialPort();

public slots:
    void onCardRead(const QByteArray &data);

private:
    Ui::MainWindow *ui;
    QSerialPort *serialPort;
    DLL_rfid *dll_rfid;
    QTimer *logoutTimer;

private slots:
    void login();
    void on_pushButtonQuit_clicked();
    void on_pushButtonEnter_clicked();
    void on_pushButtonWithdraw_clicked();
    void on_pushButtonShowBalance_clicked();
    void on_pushButtonShowTransactions_clicked();
    void on_pushButtonLogOut_clicked();
    void on_pushButtonLogOutOK_clicked();
    void on_pushButtonL1_clicked();
    void on_pushButtonL2_clicked();
    void on_pushButtonL3_clicked();
    void on_pushButtonL4_clicked();
    void on_pushButtonR1_clicked();
    void on_pushButtonR2_clicked();
    void on_pushButtonR3_clicked();
    void on_pushButtonR4_clicked();
    void on_pushButtonMuuSummaOK_clicked();
};

#endif // MAINWINDOW_H
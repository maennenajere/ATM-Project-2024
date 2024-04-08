#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dll_rfid.h"

/*
 * ui->stackedWidget->setCurrentIndex(0); <- stackLogin
 * ui->stackedWidget->setCurrentIndex(1); <- stackMenu
 * ui->stackedWidget->setCurrentIndex(2); <- stackWithdaw
 * ui->stackedWidget->setCurrentIndex(3); <- stackTransactions
 * ui->stackedWidget->setCurrentIndex(4); <- stackBalance
 * ui->stackedWidget->setCurrentIndex(5); <- stackLogOut
 *
 * RFID 1. = -06000641FF
 * RFID 2. = -06000DE344
 * RFID 3. = -06000542DC
 */

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    serialPort(new QSerialPort("COM3")),
    dll_rfid(new DLL_rfid(this, "COM3"))
{
    ui->setupUi(this);
    connect(dll_rfid, &DLL_rfid::cardRead, this, &MainWindow::onCardRead);

    // Asetetaan alkuindeksi pin-koodin syöttämiseen
    ui->stackedWidget->setCurrentIndex(0);

    // Luodaan RFID-kirjaston ja sarjaportin olio
    openSerialPort(); // Avaa sarjaportti

    // PinCode->lineEditPinCode salasanana
    ui->lineEditPinCode->setEchoMode(QLineEdit::Password);
    // Yhdistää numPadin painallukset lineEditPinCode
    for (int i = 0; i <= 9; ++i) {
        QString buttonName = QString("N") + QString::number(i);
        QObject::connect(findChild<QPushButton*>(buttonName), &QPushButton::clicked, [this, i]() {
            ui->lineEditPinCode->setText(ui->lineEditPinCode->text() + QString::number(i));
        });
    }

    connect(ui->pushButtonClear, &QPushButton::clicked, [this]() {
        ui->lineEditPinCode->clear();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
    delete dll_rfid;
    delete serialPort;
}

void MainWindow::openSerialPort()
{
    if (!dll_rfid->serialport->isOpen()) {
        dll_rfid->openSerialPort();
        qDebug() << "Debug: Sarjaportti avattu";
    } else {
        qDebug() << "Debug: Sarjaportti on jo avattu";
    }
}

void MainWindow::onCardRead(const QByteArray &data) {
    QString cleanedData = QString::fromUtf8(data).trimmed();
    qDebug() << "Debug: Received data (hex):" << data.toHex();
    qDebug() << "Debug: Received data (string):" << cleanedData;
    ui->lineEditCardInfo->setText(cleanedData);

    // Lisätään tarkastus, onko lineEditCardInfo:n teksti asetettu oikein
    qDebug() << "Debug: lineEditCardInfo text:" << ui->lineEditPinCode->text();
}

void MainWindow::on_pushButtonQuit_clicked()
{
    // Sulkee sovelluksen
    qDebug() << "Debug: Lopeta-nappia painettu";
    this->close();
}

// 06000641FF = 1111, 06000DE344 = 4444, 06000542DC = 2222
void MainWindow::on_pushButtonEnter_clicked()
{
    // Tarkistaa syötetyn PIN-koodin ja siirtyy seuraavaan näkymään, (stackMenu) jos oikein
    qDebug() << "Debug: Enter-nappia painettu";
    int correctPin = 1111;
    int enteredPin = ui->lineEditPinCode->text().toInt();
    if (enteredPin == correctPin)
    {
        ui->stackedWidget->setCurrentIndex(1);
    }
    else
    {
        QMessageBox::warning(this, "Virheellinen PIN-koodi", "Santeri hakkeri rockyou.txt\n=1111");
        qDebug() << "Debug: Virheellinen PIN-koodi";
        ui->lineEditPinCode->clear();
    }
}

// TODO: 20,40,50,100 ja x-summa nostot (mitä noston jälkeen?)
void MainWindow::on_pushButtonWithdraw_clicked()
{
    qDebug() << "Debug: Nosta rahaa-nappia painettu";
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_pushButtonShowBalance_clicked()
{
    qDebug() << "Debug: Näytä saldo-nappia painettu";
    // TODO: Näyttää käyttäjän ostovoiman.
    // Avaa stackBalance
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::on_pushButtonShowTransactions_clicked()
{
    // TODO: Näyttää käyttäjän tilitapahtumat, (kaksi nuolta selaamiseen uudet ja vanhemmat?)
    // Avaa stackTransactions
    qDebug() << "Debug: Tilitapahtumat-nappia painettu";
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_pushButtonLogOut_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
    // TODO: joku uusi stackLogOut ja ajastus->siirtyy takaisin stackLogin
    // Kirjaa käyttäjän ulos
    qDebug() << "Debug: Kirjaudu ulos-nappia painettu";
}

void MainWindow::on_pushButtonLogOutOK_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->lineEditPinCode->clear();
}


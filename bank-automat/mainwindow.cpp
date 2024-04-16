#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dll_rfid.h"
#include <curl.h>


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
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *data) {
    size_t total_size = size * nmemb;
    data->append((char*)contents, total_size);
    return total_size;
}

void MainWindow::on_pushButtonShowBalance_clicked()
{
    qDebug() << "Debug: Näytä saldo-nappia painettu";
    // TODO: Näyttää käyttäjän ostovoiman.
    // Avaa stackBalance
    ui->stackedWidget->setCurrentIndex(4);

    CURLcode ret;
    struct curl_slist *headers;
    std::string response;
    headers = NULL;
    CURL *hnd;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    hnd = curl_easy_init();

    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &response);

    curl_easy_setopt(hnd, CURLOPT_BUFFERSIZE, 102400L);
    curl_easy_setopt(hnd, CURLOPT_URL, "http://127.0.0.1:3000/login");
    curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, "{\"username\":\"4796977634126925\",\"password\":\"2212\"}");
    curl_easy_setopt(hnd, CURLOPT_POSTFIELDSIZE_LARGE, (curl_off_t)49);
    curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/8.2.1");
    curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(hnd, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_2TLS);
    curl_easy_setopt(hnd, CURLOPT_FTP_SKIP_PASV_IP, 1L);
    curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);

    ret = curl_easy_perform(hnd);

    curl_easy_cleanup(hnd);
    hnd = NULL;
    curl_slist_free_all(headers);
    headers = NULL;

    //return (int)ret;

    ui->labelBalance->setText(QString::fromStdString(response));
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


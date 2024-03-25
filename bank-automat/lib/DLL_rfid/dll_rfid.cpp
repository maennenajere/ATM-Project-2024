#include "dll_rfid.h"
#include <QSerialPortInfo>
#include <QDebug>

DLL_rfid::DLL_rfid(QObject *parent, const QString &comPort)
    : QObject(parent), serialport(new QSerialPort(comPort))
{
    serialport->setPortName(comPort);                                       // asetetaan sarjaportti
    serialport->setBaudRate(QSerialPort::Baud19200);                        // asetetaan baudinopeus
    serialport->setDataBits(QSerialPort::Data8);                            // asetetaan datan bittikoko
    serialport->setParity(QSerialPort::NoParity);                           // asetetaan pariteetti
    serialport->setStopBits(QSerialPort::OneStop);                          // asetetaan stop-bittien määrä

    connect(serialport, SIGNAL(readyRead()), this, SLOT(readData()));       // yhdistetään signaali ja slotti
}

void DLL_rfid::openSerialPort()
{
    // avataan sarjaportti
    serialport->open(QIODevice::ReadWrite);
}

void DLL_rfid::transmitData(const QString& data)
{
    // lähetetään data sarjaportin kautta
    serialport->write(data.toUtf8());
}

void DLL_rfid::returnCardID()
{
    QByteArray cleanedData = data.mid(3, data.length() - 4).trimmed();      // poistetaan tarpeettomat etu- ja takamerkit ID-datasta
    qDebug() << "DLL_rfid: Kortin siistitty ID data: " << cleanedData;       // debuggia
    emit cardRead(cleanedData);                                              // lähetetään signaali siistitystä ID-datasta
}

DLL_rfid::~DLL_rfid()
{
    // poistetaan sarjaportti-olio
    delete serialport;
}

void DLL_rfid::readData()
{
    // luetaan dataa sarjaportilta
    data = serialport->readAll();
    returnCardID();
}

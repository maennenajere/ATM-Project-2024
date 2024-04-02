#include "dll_rfid.h"
#include <QSerialPortInfo>
#include <QDebug>

DLL_rfid::DLL_rfid(QObject *parent, const QString &comPort)
    : QObject(parent), serialport(new QSerialPort(comPort))
{
    serialport->setPortName(comPort);                                       // asetetaan sarjaportti
    serialport->setBaudRate(QSerialPort::Baud9600);                         // asetetaan baudinopeus
    serialport->setDataBits(QSerialPort::Data8);                            // asetetaan datan bittikoko
    serialport->setParity(QSerialPort::NoParity);                           // asetetaan pariteetti
    serialport->setStopBits(QSerialPort::OneStop);                          // asetetaan stop-bittien määrä

    connect(serialport, &QSerialPort::readyRead, this, &DLL_rfid::readData);// yhdistetään signaali ja slotti
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
    QByteArray cleanedData = data.trimmed();
    cleanedData = cleanedData.mid(1, cleanedData.length() - 3);              // poistetaan tarpeettomat etu- ja takamerkit
    qDebug() << "DLL_rfid: Kortin siistitty ID data: " << cleanedData;       // debug
    emit cardRead(cleanedData);                                              // signaali siistitystä ID-datasta
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
    qDebug() << "DLL_rfid: Vastaanotettu data (hex): " << data.toHex();      // debug
    qDebug() << "DLL_rfid: Vastaanotettu data: " << data;                    // debug
    returnCardID();
}

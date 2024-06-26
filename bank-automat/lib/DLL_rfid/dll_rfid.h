#ifndef DLL_RFID_H
#define DLL_RFID_H

#include "DLL_rfid_global.h"
#include <QObject>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>

class DLL_RFID_EXPORT DLL_rfid : public QObject
{
    Q_OBJECT

public:
    DLL_rfid(QObject *parent, const QString &comPort);     // DLL_rfid konstruktori
    ~DLL_rfid();                                            // DLL_rfid destruktori
    QSerialPort * serialport;                               // luodaan olio serialportille

    void openSerialPort();                                  // avataan sarjaportti
    void transmitData(const QString& data);                 // metodi datan lähettämiseksi
    void returnCardID();                                    // metodi kortin ID:n palauttamiseksi

signals:
    void cardRead(const QByteArray &data);                  // signaali kortin lukemisen ilmoittamiseksi
    void readyRead();                                       // signaali sarjaportin lukemisen valmiudesta

private slots:
    void readData();

private:
    QByteArray data;                                        // muuttuja datan tallentamiseksi
};

#endif // DLL_RFID_H

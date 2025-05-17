#ifndef ARDUINO_H
#define ARDUINO_H
#include "Qserialport.h"
#include "Qstring"

class Arduino
{
public:

    Arduino();

    int connect_arduino(); // permet de connecter le PC à Arduino

    int close_arduino(); // permet de femer la connexion

    void write_to_arduino(QByteArray d); // envoyer des données vers arduino

    QByteArray read_from_arduino(); //recevoir des données de la carte Arduino

    QSerialPort* getserial() {
        return &serial;  // Make sure serial is a QSerialPort object or pointer
    }

    // accesseur

    QString getarduino_port_name();


private:

    static const quint16 arduino_uno_vendor_id = 0x2341;  // Standard Arduino Vendor ID
    static const quint16 arduino_uno_producy_id = 0x0043; // Common Arduino Product ID
    QSerialPort serial;
    bool arduino_is_available;
    QString arduino_port_name;

    QByteArray data; // contenant les données lues à partir d'Arduino
};

#endif // ARDUINO_H

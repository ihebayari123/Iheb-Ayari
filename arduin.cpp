#include "arduin.h"
#include <QDebug>

arduin::arduin() : serial(nullptr), arduino_is_available(false) {}

int arduin::connect_arduino(const QString &portName) {
    arduino_is_available = false;

    // Close port if already open
    if (serial && serial->isOpen()) {
        serial->close();
        delete serial;
        serial = nullptr;
    }

    // If specific port requested, try only that one
    if (!portName.isEmpty()) {
        foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
            if (info.portName() == portName) {
                arduino_port_name = portName;
                arduino_is_available = true;
                break;
            }
        }
    }
    else {
        // Original auto-detection logic
        foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
            if (info.hasVendorIdentifier() && info.vendorIdentifier() == 9025 &&
                info.hasProductIdentifier() && info.productIdentifier() == 67) {
                arduino_port_name = info.portName();
                arduino_is_available = true;
                break;
            }
        }
    }

    if (arduino_is_available) {
        serial = new QSerialPort(this);
        serial->setPortName(arduino_port_name);

        if (serial->open(QIODevice::ReadWrite)) {
            serial->setBaudRate(QSerialPort::Baud9600);
            // ... other serial settings ...
            return 0;
        }
        qDebug() << "Failed to open" << arduino_port_name << ":" << serial->errorString();
        return 1;
    }
    return -1;
}

int arduin::close_arduino()
{
    if (serial && serial->isOpen()){
        serial->close();
        return 0;
    }
    return 1;
}

QByteArray arduin::read_from_arduino()
{
    if(serial && serial->isReadable()){
        data = serial->readAll();
        qDebug() << "Received from Arduino:" << data;
        return data;
    }
    return QByteArray();
}

void arduin::write_to_arduino(QByteArray d)
{
    if(serial && serial->isWritable()){
        serial->write(d);
    } else {
        qDebug() << "Port série non disponible en écriture";
    }
}

#include "Arduino.h"
#include "Qserialport.h"
#include "Qstring"
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QByteArray>


int Arduino::close_arduino(){
    if(serial.isOpen()){

        serial.close();
        return 0;
    }
    return 1;
}


Arduino::Arduino() : arduino_is_available(false) {}

int Arduino::connect_arduino() {
    // Reset previous connection
    if(serial.isOpen()) {
        serial.close();
    }

    // Search for Arduino
    foreach (const QSerialPortInfo &serial_port_info, QSerialPortInfo::availablePorts()) {
        qDebug() << "Found port:" << serial_port_info.portName()
        << "VID:" << serial_port_info.vendorIdentifier()
        << "PID:" << serial_port_info.productIdentifier();

        if(serial_port_info.vendorIdentifier() == arduino_uno_vendor_id &&
            serial_port_info.productIdentifier() == arduino_uno_producy_id) {
            arduino_port_name = serial_port_info.portName();
            arduino_is_available = true;
            break;
        }
    }

    if(arduino_is_available) {
        serial.setPortName(arduino_port_name);

        // Add these checks before opening
        if(!serial.isOpen()) {
            if(serial.open(QIODevice::ReadWrite)) {
                serial.setBaudRate(QSerialPort::Baud9600);
                // ... (other settings)
                qDebug() << "Successfully connected to" << arduino_port_name;
                return 0;
            }
            else {
                qDebug() << "Failed to open port. Error:" << serial.errorString();
                return 1;
            }
        }
    }

    qDebug() << "No Arduino found on any ports";
    return -1;
}

QString Arduino::getarduino_port_name() {
    return arduino_port_name;  // Return actual detected port name
}

QByteArray Arduino::read_from_arduino() {
    if(serial.isReadable()) {
        return serial.readAll();
    }
    return QByteArray();  // Return empty array if not readable
}
void Arduino::write_to_arduino(QByteArray d)
{
    if(serial.isOpen() && serial.isWritable()) {
        qint64 bytesWritten = serial.write(d);
        if(bytesWritten == -1) {
            qDebug() << "Failed to write to serial port";
        } else {
            qDebug() << "Wrote" << bytesWritten << "bytes to Arduino";
        }
        serial.flush(); // Ensure data is sent immediately
    } else {
        qDebug() << "Serial port not writable or not open";
    }
}

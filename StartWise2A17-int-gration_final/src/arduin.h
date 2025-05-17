#ifndef ARDUIN_H
#define ARDUIN_H

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QString>

class arduin {
public:
    arduin();
    int connect_arduino(const QString &portName);
    int close_arduino();
    QByteArray read_from_arduino();
    void write_to_arduino(QByteArray d);

private:
    QSerialPort *serial;
    QString arduino_port_name;
    bool arduino_is_available;
    QByteArray data;
};

#endif // ARDUIN_H
#ifndef ARDUIN_H
#define ARDUIN_H

#include <QSerialPort>
#include <QSerialPortInfo>

class arduin : public QObject {
    Q_OBJECT
public:
    arduin();
    int connect_arduino(const QString &portName = "");
    int close_arduino();
    QByteArray read_from_arduino();
    void write_to_arduino(QByteArray d);
    QSerialPort* getserial() { return serial; }
    QString getarduino_port_name() { return arduino_port_name; }

private:
    QSerialPort* serial;
    QString arduino_port_name;
    bool arduino_is_available;
    QByteArray data;
    static const quint16 arduino_uno_vendor_id = 9025;
    static const quint16 arduino_uno_product_id = 67;
};

#endif // ARDUIN_H

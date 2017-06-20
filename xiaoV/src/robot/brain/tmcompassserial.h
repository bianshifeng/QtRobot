#ifndef TMCOMPASSSERIAL_H
#define TMCOMPASSSERIAL_H
#include <QObject>
#include <QSettings>
#include <QTimer>

#include "tmserialwork.h"

class TmCompassSerial : public QObject
{
    Q_OBJECT
public:
    explicit TmCompassSerial(QObject *parent = 0);
    ~TmCompassSerial();

    void initConfig();
    void exit();

    void compass_config_cmd_appliedSensor(bool isOpen);
//    void compass_config_cmd_gyroSensor(bool isOpen);
//    void compass_config_cmd_magneticSensor(bool isOpen);


signals:
    void initSerialPointer(QSerialPort* serialPort);
    void initCompassSerial(QString strCom);
    void readSerialData();
    void writeSerialData(QByteArray bArray);


public slots:
    void slot_serial_composs_data(QString);

private:
    TmSerialWork* m_ptrSerialWork;
    QSettings* m_settings;
    QTimer m_readTimer;
    QSerialPort* m_ptrSerialPort;
};

#endif // TMCOMPASSSERIAL_H

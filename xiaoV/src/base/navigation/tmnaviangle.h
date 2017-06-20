#ifndef TMNAVIANGLE_H
#define TMNAVIANGLE_H

#include <QObject>
#include <QSerialPort>
#include <QTimer>

class TMNaviAngle : public QObject
{
    Q_OBJECT
public:
    explicit TMNaviAngle(QObject *parent = 0);
    bool initConfig();
private:
    QSerialPort* pr_pSerial;
    QByteArray pr_bArray;
    bool m_isRuning;
    QTimer m_timerStart;

    void process_data();
signals:
    void sig_angle(double fYoll, double fPitch, double fYaw);
private slots:
    void slot_serial_data();
    void startTimeout();
public slots:
    bool slot_init(QString strCom);
    void slot_start();
    void slot_function_cotrol(QString strName);

};

#endif // TMNAVIANGLE_H

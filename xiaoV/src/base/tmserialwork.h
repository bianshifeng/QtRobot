#ifndef TMSERIALWORK_H
#define TMSERIALWORK_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QQueue>
#include <QSerialPort>
#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#include <QMap>
#include <QTimer>
class TmSerialWorkThread;

// to work in the thread
class TmSerialWork : public QObject
{
    Q_OBJECT
public:
    explicit TmSerialWork(QObject *parent = 0);
    void threadLoop();
    bool m_bInitOk;
    bool m_bEnableCtrl;
    int m_nPersonDistance[2];
    int m_nHongWaiDistance[4];
    bool m_bIsPersonMoving;
    void exit();
private:
    TmSerialWorkThread* pr_pThreadWork;
    QSerialPort* m_pSer;

    QQueue<QByteArray> m_ctrlList;
    QMutex m_lock;
    QMutex m_lock_readData;
    QByteArray m_serBuff;

    int find_cmd(char* pData, int nLen, char cHead, char cEnd); // 根据头和尾查找是否是一条完整的数据

    int find_cmd_type(char* pData, int nLen, char cHead, char cCMD, char cEnd); // 根据头,指令和尾查找是否是一条要找的特定完整的指令

signals:
    void sig_event_touch(QString strIndex);
    void sig_serial_data(QByteArray btyeArrayData);
    void sig_serial_data_for_wheelcode_query(QByteArray btyeArrayData);
    void sig_serial_data_for_wheel_stop(QByteArray btyeArrayData);

private slots:
    void ser_newdata();
    void paserHeadCmd(QByteArray bArry);
public slots:
    void initSerial(QString strCom);

    void initSerialPointer(QSerialPort* serialPort);
    void writeSerialData(QByteArray bArry);




    void paserIsWheelCmd(QByteArray bArry);
};

// the work thread
class TmSerialWorkThread : public QThread
{
    Q_OBJECT
public:
    explicit TmSerialWorkThread(TmSerialWork* pWork,QObject *parent = 0);
    bool m_bExit;
    void logHex(QByteArray bArry);
private:
    TmSerialWork* pr_pWork;
    virtual void run();

};

#endif // TMSERIALWORK_H

#include "tmcompassserial.h"
#include "tmglobaltool.h"

TmCompassSerial::TmCompassSerial(QObject *parent)
    :QObject(parent),m_settings(0),m_ptrSerialWork(0),m_ptrSerialPort(0)
{
    m_ptrSerialWork = new TmSerialWork();
    connect(this,SIGNAL(initSerialPointer(QSerialPort*)),
            m_ptrSerialWork,SLOT(initSerialPointer(QSerialPort*)));


    connect(this,SIGNAL(initCompassSerial(QString)),
            m_ptrSerialWork,SLOT(initCompassSerial(QString)));

    connect(this,SIGNAL(writeSerialData(QByteArray)),
            m_ptrSerialWork,SLOT(writeSerialData(QByteArray)));
    connect(m_ptrSerialWork,SIGNAL(sig_serial_composs_data(QString)),
            this,SLOT(slot_serial_composs_data(QString)));
}

TmCompassSerial::~TmCompassSerial()
{

    exit();
}

void TmCompassSerial::initConfig()
{
    QJsonObject root = g_configJson.value("Compass").toObject();
    if(root.isEmpty())
    {
        g_log("--------init TmCompassSerial SerialWork---not have config");
        return;
    }
    g_log("--------init TmCompassSerial SerialWork");
    QString t_com = root.value("port").toString();

    emit initCompassSerial(t_com);
    return;

    m_ptrSerialPort = new QSerialPort();
    m_ptrSerialPort->setPortName(t_com);
    m_ptrSerialPort->setBaudRate(QSerialPort::Baud115200);
    m_ptrSerialPort->setDataBits(QSerialPort::Data8);
    m_ptrSerialPort->setStopBits(QSerialPort::OneStop);
    m_ptrSerialPort->setParity(QSerialPort::NoParity);

    m_ptrSerialPort->moveToThread(m_ptrSerialWork->thread());

    emit initSerialPointer(m_ptrSerialPort);
}

void TmCompassSerial::exit()
{
    if(m_ptrSerialWork != Q_NULLPTR)
        m_ptrSerialWork->deleteLater();
    if(m_ptrSerialPort != Q_NULLPTR)
        m_ptrSerialPort->deleteLater();
    if(m_settings != Q_NULLPTR)
        m_settings->deleteLater();
    m_readTimer.stop();


}

void TmCompassSerial::compass_config_cmd_appliedSensor(bool isOpen)
{
    Q_UNUSED(isOpen);

    char buff[] ={0XA5,0X95,0X3A};
    emit writeSerialData(QByteArray(buff,3));
}

void TmCompassSerial::slot_serial_composs_data(QString)
{

}

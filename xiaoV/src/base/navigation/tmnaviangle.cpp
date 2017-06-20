#include "tmnaviangle.h"
#include <QDebug>
#include "tmglobaltool.h"

static  int Static_Compass_Active_Counter = 0;

TMNaviAngle::TMNaviAngle(QObject *parent) : QObject(parent)
{
    m_isRuning = false;
    pr_pSerial = NULL;


    connect(&m_timerStart,SIGNAL(timeout()),SLOT(startTimeout()));
    m_timerStart.setSingleShot(true);
}

bool TMNaviAngle::initConfig()
{
    QJsonObject rootNavi = g_configJson.value("Compass").toObject();
    if(rootNavi.isEmpty())
    {
        g_log("--------init TmCompassSerial SerialWork---not have config");
        return false;
    }

    QString t_com = rootNavi.value("port").toString();
    return this->slot_init(t_com);
}


void TMNaviAngle::slot_start()
{
    if( pr_pSerial->isOpen()){
        uchar pSend[3];
        pSend[0] = (uchar)0x0a5;
        pSend[1] = (uchar)0x045;
        pSend[2] = (uchar)0x0ea;
        pr_pSerial->write((const char*)pSend, 3);
        m_timerStart.start(2000);
    }
}
void TMNaviAngle::startTimeout()
{
    if(Static_Compass_Active_Counter < 50){
        Static_Compass_Active_Counter = 0;


        this->slot_start();
    }else{
        m_isRuning = true;
        Static_Compass_Active_Counter = 0;
        m_timerStart.stop();
    }
}

void TMNaviAngle::process_data()
{
    int nLen = pr_bArray.size();
    char* pData = pr_bArray.data();
    int nIndex = 0;
    while((nLen-10)>nIndex){

        if(!m_isRuning){
            //通过记录是否别执行用来重启设备
            ++Static_Compass_Active_Counter;
        }

        if( (uchar)pData[nIndex] == (uchar)0x5A && (uchar)pData[nIndex+1] == (uchar)0x5A ){
            //qDebug()<<"One Cmd: type "<<(uchar)pData[nIndex+2];
            if( (uchar)pData[nIndex+2] == (uchar)0x45 ){
                double fRoll =(short)( (((uchar)pData[nIndex+4])<<8) | (uchar)pData[nIndex+5] ) *1.0 / 100;
                double fPitch =(short)( (((uchar)pData[nIndex+6])<<8) | (uchar)pData[nIndex+7] ) *1.0 / 100;
                double fYaw =(short)( (((uchar)pData[nIndex+8])<<8) | (uchar)pData[nIndex+9] ) *1.0 / 100;
                //qDebug()<<fRoll<<fPitch<<fYaw;
                emit sig_angle(fRoll, fPitch, fYaw);
            }
            nIndex += 11;
        }else{
            nIndex += 1;
        }
    }
    //qDebug()<<"Remote array left:"<<nIndex;
    pr_bArray.remove(0, nIndex);
}

void TMNaviAngle::slot_serial_data()
{
    pr_bArray += pr_pSerial->readAll();
    process_data();
}


bool TMNaviAngle::slot_init(QString strCom)
{
    if( !pr_pSerial ){
        pr_pSerial = new QSerialPort();
        connect(pr_pSerial, SIGNAL(readyRead()), this, SLOT(slot_serial_data()));
    }

    pr_pSerial->setDataBits(QSerialPort::Data8);
    pr_pSerial->setStopBits(QSerialPort::OneStop);
    pr_pSerial->setBaudRate(QSerialPort::Baud115200);
    pr_pSerial->setPortName(strCom);
    if(!pr_pSerial->open(QIODevice::ReadWrite)){
        g_log("电子罗盘：" +pr_pSerial->portName()+"串口打开失败，请检查接口是否接好或者被其他程序占用!" );
        return false;
    }
    g_log("电子罗盘：" +pr_pSerial->portName()+"串口打开成功!" );

    //进行罗盘信号反馈检查
    Static_Compass_Active_Counter = 0;
    slot_start();
    return true;
}



void TMNaviAngle::slot_function_cotrol(QString strName)
{
    if( pr_pSerial->isOpen() ){
        uchar pSend[3];
        if( strName == "jiasudu" ){
            pSend[0] = (uchar)0x0a5;
            pSend[1] = (uchar)0x051;
            pSend[2] = (uchar)0x0f6;
            pr_pSerial->write((const char*)pSend, 3);
        }
        if( strName == "tuoluo" ){
            pSend[0] = (uchar)0x0a5;
            pSend[1] = (uchar)0x052;
            pSend[2] = (uchar)0x0f7;
            pr_pSerial->write((const char*)pSend, 3);
        }
        if( strName == "cichang" ){
            pSend[0] = (uchar)0x0a5;
            pSend[1] = (uchar)0x053;
            pSend[2] = (uchar)0x0f8;
            pr_pSerial->write((const char*)pSend, 3);
        }
    }
}


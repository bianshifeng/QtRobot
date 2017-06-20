#include "tmserialwork.h"
#include <tmglobaltool.h>

TmSerialWorkThread::TmSerialWorkThread(TmSerialWork *pWork, QObject *parent)
{
    Q_UNUSED(parent);
    pr_pWork = pWork;
    m_bExit = false;
}

void TmSerialWorkThread::run()
{
    qDebug()<<"TmSerialWorkThread :"<<currentThreadId();
    while(!m_bExit){
        QCoreApplication::processEvents(QEventLoop::AllEvents, 20);
        msleep(30);
        pr_pWork->threadLoop();
    }
}

void TmSerialWorkThread::logHex(QByteArray bArry)
{
    if( bArry.size() < 8 ){
        return;
    }
    QString strOut;
    strOut.sprintf("%02X %02X %02X %02X %02X %02X %02X %02X %02X", (uchar)bArry[0],(uchar)bArry[1],
            (uchar)bArry[2],(uchar)bArry[3],(uchar)bArry[4],(uchar)bArry[5],(uchar)bArry[6],(uchar)bArry[7],(uchar)bArry[8]);
    qDebug()<<strOut;
}

//----------------------------------//

TmSerialWork::TmSerialWork(QObject *parent) : QObject(parent)
{
    m_bEnableCtrl = true;
    m_bInitOk = false;
    m_bIsPersonMoving = false;
    m_nPersonDistance[0] = 0;
    m_nPersonDistance[1] = 0;
    m_nHongWaiDistance[0] = 0;
    m_nHongWaiDistance[1] = 0;
    m_nHongWaiDistance[2] = 0;
    m_nHongWaiDistance[3] = 0;

    pr_pThreadWork = new TmSerialWorkThread(this);
    moveToThread(pr_pThreadWork);
    // start thread
    pr_pThreadWork->start();
}

void TmSerialWork::threadLoop()
{

    if( !m_bInitOk ){
        return;
    }

    // queue read
    m_lock.lock();
    if( !m_ctrlList.isEmpty() ){
        QByteArray bArry = m_ctrlList.dequeue();


        if( m_bEnableCtrl ){
           //pr_pThreadWork->logHex(bArry);
           m_pSer->write(bArry); // real write data
        }
    }
    m_lock.unlock();
}

void TmSerialWork::exit()
{
    pr_pThreadWork->m_bExit = true;
    pr_pThreadWork->exit();
    pr_pThreadWork->deleteLater();

    if(m_pSer != Q_NULLPTR)
    {
        g_log("TmSerialWork-------init TmCompassSerial SerialWork---not have config");
        m_pSer->close();
        m_pSer->deleteLater();
    }

}


void TmSerialWork::ser_newdata()
{

    m_serBuff += m_pSer->readAll();
    if( m_serBuff.length() >= 9 ){

//        paserIsWheelCmd(m_serBuff);
        paserHeadCmd(m_serBuff);
    }
}

void TmSerialWork::paserHeadCmd(QByteArray bArry)
{
    int nhead = find_cmd(m_serBuff.data(), m_serBuff.length(), 0x01, 0xAA);
    while( nhead >= 0  ){
        // 处理9个字节的数据
        QByteArray bArry = m_serBuff.mid(nhead, 9);
        if( bArry[1] == 0x02 ){
            if( bArry[2] == 0x13 ){
                if( (bArry[3]&0x01) == 0 ){
                    // head touch
                    emit sig_event_touch("head");
                }
                if( (bArry[3]&0x02) == 0 ){
                    // left hand touch
                    emit sig_event_touch("left_hand");
                }
                if( (bArry[3]&0x04) == 0 ){
                    // right hand touch
                    emit sig_event_touch("right_hand");
                }
                if( (bArry[3]&0x08) == 0 ){
                    // remin checked
                    m_bIsPersonMoving = true;
                }else{
                    m_bIsPersonMoving = false;
                }
            }
            if( bArry[2] == 0x14 ){
                m_nPersonDistance[0] = ((uchar)bArry[3])*256 + (uchar)bArry[4];
                m_nPersonDistance[1] =  ((uchar)bArry[5])*256 + (uchar)bArry[6];
            }
        }
        if( bArry[1] == 0x22 ){
            if( ( bArry[2] == 0x50 || bArry[2] == 0x51 ) && bArry[3] == 0x00 ){
                // error
                QByteArray bReset; // 22 55 00 00 00 00 00 00 aa
                bReset.resize(9);
                bReset.fill(0);
                bReset[0] = 0x22;
                bReset[1] = 0x55;
                bReset[8] = 0xAA;
                writeSerialData(bReset);
            }
            if((bArry[2])== 0x56){
                emit sig_serial_data_for_wheel_stop(bArry);

            }
            if((bArry[2])== 0x59){
                emit sig_serial_data_for_wheelcode_query(bArry);

            }
        }
        if( bArry[1] == 0x92 ){
            if( bArry[2] == 0x03 && bArry[3] == 0x01){
                emit sig_event_touch("panzi");
            }
        }
        if( bArry[1] == 0x95 ){
            if( bArry[2] == 0x01 ){
                m_nHongWaiDistance[0] = (int)(0.5*(int)((uchar)bArry[3]) + 0.5);
                m_nHongWaiDistance[1] = (int)(0.5*(int)((uchar)bArry[4]) + 0.5);
                m_nHongWaiDistance[2] = (int)(0.5*(int)((uchar)bArry[5]) + 0.5);
                m_nHongWaiDistance[3] = (int)(0.5*(int)((uchar)bArry[6]) + 0.5);
            }
        }
        // 处理之后的cmd删除
        m_serBuff.remove(0, nhead+9);
        nhead = find_cmd(m_serBuff.data(), m_serBuff.length(), 0x01, 0xAA);
    }


}

void TmSerialWork::paserIsWheelCmd(QByteArray bArry)
{


    int t_nwheelCmdhead = find_cmd_type(bArry.data(),bArry.length(),0x22,0x59,0xAA);
    if (t_nwheelCmdhead >=0){
        emit sig_serial_data_for_wheelcode_query(bArry);
    }



    int t_nwheelCmdStop = find_cmd_type(bArry.data(),bArry.length(),0x22,0x56,0xAA);
    if(t_nwheelCmdStop >0){

        g_log("bsf");
        QString strOut;
        strOut.sprintf("%02X %02X %02X %02X %02X %02X %02X %02X %02X",
                       (uchar)bArry[0],(uchar)bArry[1],(uchar)bArry[2],(uchar)bArry[3],(uchar)bArry[4],(uchar)bArry[5],(uchar)bArry[6],(uchar)bArry[7],(uchar)bArry[8]);

        qDebug()<<strOut;
        strOut.sprintf("%02X %02X %02X %02X %02X %02X %02X %02X %02X",
                       (uchar)bArry[9],(uchar)bArry[10],(uchar)bArry[11],(uchar)bArry[12],(uchar)bArry[13],(uchar)bArry[14],(uchar)bArry[15],(uchar)bArry[16],(uchar)bArry[17]);

        qDebug()<<strOut;
        qDebug() << bArry.length();
        g_log("bsf");
        emit sig_serial_data_for_wheel_stop(bArry);
    }

}
int TmSerialWork::find_cmd_type(char *pData, int nLen, char cHead, char cCMD, char cEnd)
{
    int nPos = -1;

    for(int i =0; i < nLen-8; i++){

        if(pData[i+1] == cHead && pData[i+2] == cCMD && pData[i+8] == cEnd){
            nPos = i;
            return nPos;
        }
    }
    return nPos;

}


int TmSerialWork::find_cmd(char *pData, int nLen, char cHead, char cEnd)
{
    if( nLen < 9 ){
        return -1;
    }
    int nPos = -1;
    for( int i=0; i<nLen-8;i++ ){
        if( pData[i+0] == cHead && pData[i+8] == cEnd ){
            nPos = i;
            return nPos;
        }
    }
    return nPos;
}






void TmSerialWork::initSerial(QString strCom)
{
    // init serial port
    m_pSer = new QSerialPort();
    m_pSer->setPortName(strCom);
    m_pSer->setBaudRate(QSerialPort::Baud57600);
    m_pSer->setDataBits(QSerialPort::Data8);
    m_pSer->setStopBits(QSerialPort::OneStop);
    m_pSer->setParity(QSerialPort::NoParity);

    m_bInitOk = m_pSer->open(QIODevice::ReadWrite);

    if( !m_bInitOk){
        g_log(strCom+"串口打开失败，请检查接口是否接好或者被其他程序占用!");
        return;
    }
    g_log(strCom+"串口打开成功，请检查接口是否接好或者被其他程序占用!");
    // recv data
    connect(m_pSer, SIGNAL(readyRead()), this, SLOT(ser_newdata()));

}

void TmSerialWork::initSerialPointer(QSerialPort *serialPort)
{

    m_pSer = serialPort;
    m_bInitOk = m_pSer->open(QIODevice::ReadWrite);
    if(!m_bInitOk)
    {
        g_log("Composs serialPort: " + serialPort->portName() + "串口打开失败，请检查接口是否接好或者被其他程序占用!");
        return;
    }
    qDebug()<<m_bInitOk;
    g_log("11111111111111111111Composs serialPort: " + m_pSer->portName() + "串口打开成功，请检查接口是否接好或者被其他程序占用!");

//    connect(m_pSer,SIGNAL(readyRead()),this,SLOT(ser_newdata()));

}

void TmSerialWork::writeSerialData(QByteArray bArry)
{



    if( !m_bEnableCtrl ){
        return;
    }

    m_lock.lock();
    if( m_ctrlList.size() > 32 ){
        m_ctrlList.clear();
    }
    m_ctrlList.enqueue(bArry);
//    qDebug() << m_ctrlList.count();
    m_lock.unlock();

}


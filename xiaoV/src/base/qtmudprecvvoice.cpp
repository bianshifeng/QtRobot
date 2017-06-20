#include "qtmudprecvvoice.h"
#include <QThread>
#include <QCoreApplication>

QTmUDPRecvVoice::QTmUDPRecvVoice(QObject *parent) : QObject(parent)
{
    m_bStartRecvData = false;
    connect(&m_connectTimer,SIGNAL(timeout()),this,SLOT(slot_timer()));
}

bool QTmUDPRecvVoice::init(qint32 nPort)
{
    qDebug()<<"init udp socket";
    m_nPort = nPort;
    m_nTimeOut = 0;
    m_UDPsocket.bind(QHostAddress::Any,nPort,QAbstractSocket::ShareAddress|QAbstractSocket::ReuseAddressHint);
    connect(&m_UDPsocket,SIGNAL(readyRead()),this,SLOT(slot_RecvPackage()));
//    connect(&m_UDPsocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(slot_error(QAbstractSocket::SocketError)));
//    connect(&m_UDPsocket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(slot_stateChanged(QAbstractSocket::SocketState)));
    return true;
}

void QTmUDPRecvVoice::uninit()
{
    qDebug()<<"uninit udp socket";
    m_nTimeOut = 0;
    m_UDPsocket.abort();
    disconnect(&m_UDPsocket,SIGNAL(readyRead()),this,SLOT(slot_RecvPackage()));
}

void QTmUDPRecvVoice::slot_RecvPackage()
{
    m_bStartRecvData = true;
    if(!m_connectTimer.isActive())
        m_connectTimer.start(1000);

    QByteArray fullPackage;
    QByteArray voiceBuf;
    while (m_UDPsocket.hasPendingDatagrams())
    {
        fullPackage.resize(m_UDPsocket.pendingDatagramSize());
        m_UDPsocket.readDatagram(fullPackage.data(), fullPackage.size());
        voiceBuf = m_Package.parse_full_package_audio(fullPackage);
        emit signal_RecvData(voiceBuf,(qint32)voiceBuf.length());

        //QCoreApplication::processEvents(QEventLoop::AllEvents, 10);
//        QThread::usleep(10);
    }
}

void QTmUDPRecvVoice::slot_error(QAbstractSocket::SocketError socketError)
{
    qDebug()<<"socket error!!"<<socketError;
}

void QTmUDPRecvVoice::slot_stateChanged(QAbstractSocket::SocketState socketState)
{
    qDebug()<<"socket state changed!"<<socketState;
}

void QTmUDPRecvVoice::slot_timer()
{
    if(m_bStartRecvData)
    {
        m_bStartRecvData = false;
        m_nTimeOut = 0;
    }
    else if(!m_bStartRecvData)
    {
        m_nTimeOut++;
        if(m_nTimeOut >= 5)
        {
            //5秒内无数据，停止监听
            m_connectTimer.stop();
            uninit();
            //重启监听
            init(m_nPort);
        }
    }
}


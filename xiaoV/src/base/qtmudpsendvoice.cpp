#include "qtmudpsendvoice.h"

QTmUDPSendVoice::QTmUDPSendVoice(QObject *parent) : QObject(parent)
{

}

bool QTmUDPSendVoice::init(QString szAddress,qint32 nPort)
{
    m_szIP = szAddress;
    m_nPort = nPort;

    //m_Package.setUUID("tami");
    return true;
}

bool QTmUDPSendVoice::SendVoicePackage(QByteArray baData,int nLen)
{
    QByteArray sendBuf = m_Package.make_full_package_audio(baData.data(),nLen);
    m_UDPsocket.writeDatagram(sendBuf,sendBuf.length(),QHostAddress(m_szIP),m_nPort);
    //qDebug()<<"Send data len"<<sendBuf.length();
    return true;
}

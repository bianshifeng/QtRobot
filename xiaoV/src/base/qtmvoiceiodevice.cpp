#include "qtmvoiceiodevice.h"
#include <QDebug>

QTmVoiceIODevice::QTmVoiceIODevice()
{
    //open(QIODevice::ReadWrite|QIODevice::Unbuffered);
    open(QIODevice::ReadWrite);
}

//qint64 QTmVoiceIODevice::readData(char *data, qint64 maxlen)
//{
//    qint32 total = m_buffer.length();
//    if(total == 0)
//    {
//        qDebug()<<"read null data:"<<maxlen;
//        memset(data,0,maxlen);
//        //return maxlen;
//        return 640;
//    }

//    if(total > maxlen)
//        total = maxlen;

//    memcpy(data,m_buffer.data(),total);

//    qDebug()<<"Clear buffer--"<<total<<maxlen;
//    m_buffer.clear();

//    return total;
//}

qint64 QTmVoiceIODevice::readData(char *data, qint64 maxlen)
{
    qint32 nCount = m_bufList.size();
    if(nCount == 0)
    {
        qDebug()<<"read null data, need"<<maxlen;
        memset(data,0,640);
        //return maxlen;
        return 640;
    }

    QByteArray buf = m_bufList.at(0);
    qint32 nDataLen = buf.length();
    if(nDataLen > maxlen)
        nDataLen = maxlen;

    memcpy(data,buf.data(),nDataLen);

    m_bufList.pop_front();
    qDebug()<<"Clear buffer--"<<nDataLen<<maxlen<<m_bufList.size();

    return nDataLen;
}

qint64 QTmVoiceIODevice::writeData(const char *data, qint64 len)
{
//    qDebug()<<"Recv data"<<len<<m_bufList.size();
//    m_buffer.append(data,len);

    //
    if(m_bufList.size() > 200)
        m_bufList.clear();
    QByteArray tmp(data,len);
    m_bufList.push_back(tmp);
    qDebug()<<"Recv data"<<len<<m_bufList.size();

    return len;
}


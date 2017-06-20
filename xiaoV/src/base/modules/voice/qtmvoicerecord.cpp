#include "qtmvoicerecord.h"
#include <QDebug>
#include <tmglobaltool.h>

const qint32 CONST_BUFFER_SIZE = 4*1024;

QTmVoiceRecord::QTmVoiceRecord(QObject *parent) : QObject(parent)
{
    m_buffer.fill(0,CONST_BUFFER_SIZE);
    m_InputAudio = NULL;
    m_InputIO = NULL;
}

bool QTmVoiceRecord::init()
{
    QAudioFormat format;
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    m_InputDeviceInfo = QAudioDeviceInfo::defaultInputDevice();
    QString str=m_InputDeviceInfo.deviceName();
    //qDebug()<<str.toLatin1().data()<<str.toStdString().data();
    g_log("use input device:"+str);
    if(!m_InputDeviceInfo.isFormatSupported(format))
    {
        format = m_InputDeviceInfo.nearestFormat(format);
        qWarning()<<"raw audio format not supported by backend, cannot play audio 1.";
        return false;
    }

    //创建录音获取对象
    m_InputAudio = new QAudioInput(m_InputDeviceInfo,format,this);        

    return true;
}

bool QTmVoiceRecord::start_record_voice(QString szIP,qint32 nPort)
{
    //初始化UDP发送
    if(m_InputIO){
        return true;
    }
    m_VoiceUDPSend.init(szIP,nPort);

    m_InputIO = m_InputAudio->start();
    connect(m_InputIO,SIGNAL(readyRead()),this,SLOT(slot_readVoiceData()));
    return true;
}

bool QTmVoiceRecord::stop_record_voice()
{
    if(m_InputIO){
        m_InputAudio->stop();
        disconnect(m_InputIO,SIGNAL(readyRead()),this,SLOT(slot_readVoiceData()));
        m_InputIO = NULL;
    }
    return true;
}

void QTmVoiceRecord::slot_readVoiceData()
{
    if (!m_InputAudio)
        return;

    qint32 len = m_InputAudio->bytesReady();
    if (len > CONST_BUFFER_SIZE)
        len = CONST_BUFFER_SIZE;
    m_buffer.fill(0);
    qint32 l = m_InputIO->read(m_buffer.data(), len);
    if(l>0)
        m_VoiceUDPSend.SendVoicePackage(m_buffer,len);
}

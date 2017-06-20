#include "qtmvoiceplay.h"
#include <QDebug>
#include <QThread>

QTmVoicePlay::QTmVoicePlay(QObject *parent) : QObject(parent)
{
    pr_bOpen = false;
}

bool QTmVoicePlay::init(qint32 nPort)
{
    //////////////////////////
    QAudioFormat format;
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    m_OutputDeviceInfo = QAudioDeviceInfo::defaultOutputDevice();
    if (!m_OutputDeviceInfo.isFormatSupported(format))
    {
        qWarning()<<"raw audio format not supported by backend, cannot play audio 2.";
        return false;
    }

    //初始化播放设备
    m_OutputAudio = new QAudioOutput(m_OutputDeviceInfo,format,this);
    //创建存储数据的IO设备
    m_OutputData = new QTmVoiceIODevice();
    //启动UDP服务，接收声音数据
//    m_VoiceUDPRecv.init(nPort);

    m_nPort = nPort;
    pr_bOpen = false;

    m_VoiceUDPRecv.init(m_nPort);
    connect(&m_VoiceUDPRecv,SIGNAL(signal_RecvData(QByteArray,qint32)),this,SLOT(slot_AddVoiceBuffer(QByteArray,qint32)));

    return true;
}

bool QTmVoicePlay::start_play_voice()
{        
    //启动UDP服务，接收声音数据
    if( pr_bOpen ){
        return true;
    }

//    m_VoiceUDPRecv.init(m_nPort);

//    connect(&m_VoiceUDPRecv,SIGNAL(signal_RecvData(QByteArray,qint32)),this,SLOT(slot_AddVoiceBuffer(QByteArray,qint32)),Qt::QueuedConnection);

    m_OutputAudio->start(m_OutputData);

    return true;
}

bool QTmVoicePlay::stop_play_voice()
{
    if(pr_bOpen){
        //disconnect(&m_VoiceUDPRecv,SIGNAL(signal_RecvData(QByteArray,qint32)),this,SLOT(slot_AddVoiceBuffer(QByteArray,qint32)));
        m_OutputAudio->stop();
        pr_bOpen = false;
    }
    return true;
}

bool QTmVoicePlay::slot_start_play_voice()
{
    //启动UDP服务，接收声音数据
    if( m_OutputAudio ){
        return true;
    }
//    m_VoiceUDPRecv.init(m_nPort);

//    connect(&m_VoiceUDPRecv,SIGNAL(signal_RecvData(QByteArray,qint32)),this,SLOT(slot_AddVoiceBuffer(QByteArray,qint32)),Qt::QueuedConnection);

    m_OutputAudio->start(m_OutputData);

//    qDebug()<<"BEGIN:start recv voice data---";

    return true;
}

bool QTmVoicePlay::slot_stop_play_voice()
{
    if( m_OutputAudio ){
        //disconnect(&m_VoiceUDPRecv,SIGNAL(signal_RecvData(QByteArray,qint32)),this,SLOT(slot_AddVoiceBuffer(QByteArray,qint32)));
        m_OutputAudio->stop();
        m_OutputAudio = NULL;
    }


//    qDebug()<<"END:stop recv voice data-------";

    return true;
}

void QTmVoicePlay::slot_AddVoiceBuffer(QByteArray buf, qint32 nBufLen)
{
    m_OutputData->write(buf.data(),nBufLen);
//    qDebug()<<"recv voice";
}

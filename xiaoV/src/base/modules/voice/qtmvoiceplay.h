#ifndef QTMVOICEPLAY_H
#define QTMVOICEPLAY_H

#include <QObject>
#include <QAudioOutput>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include "qtmudprecvvoice.h"
#include "qtmvoiceiodevice.h"


class QTmVoicePlay : public QObject
{
    Q_OBJECT
public:
    explicit QTmVoicePlay(QObject *parent = 0);

    //初始化音频播放格式
    bool init(qint32 nPort);

    bool start_play_voice();
    bool stop_play_voice();

signals:

public slots:
    //接收网络传来的音频数据，存入IO设备
    void slot_AddVoiceBuffer(QByteArray buf,qint32 nBufLen);
    bool slot_start_play_voice();
    bool slot_stop_play_voice();

private:
    QAudioDeviceInfo m_OutputDeviceInfo;
    QAudioOutput* m_OutputAudio;
    QTmVoiceIODevice *m_OutputData;
    QTmUDPRecvVoice m_VoiceUDPRecv;
    bool pr_bOpen;

    int m_nPort;
};

#endif // QTMVOICEPLAY_H

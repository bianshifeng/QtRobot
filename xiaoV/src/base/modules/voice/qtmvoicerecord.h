#ifndef QTMVOICERECORD_H
#define QTMVOICERECORD_H

#include <QObject>
#include <QAudioInput>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include "qtmudpsendvoice.h"

class QTmVoiceRecord : public QObject
{
    Q_OBJECT
public:
    explicit QTmVoiceRecord(QObject *parent = 0);

    bool init();
    bool start_record_voice(QString szIP,qint32 nPort);
    bool stop_record_voice();

public:
    QIODevice * m_InputIO;
    QAudioDeviceInfo m_InputDeviceInfo;
    QAudioInput * m_InputAudio;
    QByteArray m_buffer;
    QTmUDPSendVoice m_VoiceUDPSend;

signals:

public slots:
    void slot_readVoiceData();
};

#endif // QTMVOICERECORD_H

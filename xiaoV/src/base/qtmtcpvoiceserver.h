#ifndef QTMTCPVOICESERVER_H
#define QTMTCPVOICESERVER_H

#include <QObject>
#include "qtmpackage.h"
#include <QTcpSocket>
#include <QTcpServer>
#include <QAudioOutput>
#include <QAudioInput>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <qtmvoiceiodevice.h>

class QTmTCPVoiceServer : public QObject
{
    Q_OBJECT
public:
    explicit QTmTCPVoiceServer(QObject *parent = 0);
    void initConfig();
signals:
    void sig_enableAudioSend(bool bStart);
    void sig_enableAudioReceive(bool bStart);
private slots:
    void slot_server_open(bool bOpen);
    void slot_new_connection();
    void slot_new_tcp_data();
    void slot_send_audio();

    void slot_enableAudioSend(bool bStart);
    void slot_enableAudioReceive(bool bStart);
private:
    QTcpSocket* pr_pSocket;
    QTcpServer* pr_pServer;
    int pr_nPort;
    QAudioInput* pr_pInputAudio;
    QIODevice* pr_pInputDevice;
    QAudioOutput* pr_pOutputAudio;
    QTmVoiceIODevice* pr_pOutputDevice;
    QTmPackage pr_package;
};

#endif // QTMTCPVOICESERVER_H

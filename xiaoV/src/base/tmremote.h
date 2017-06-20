#ifndef TMREMOTE_H
#define TMREMOTE_H

/*
 * Remote control interface
 */

#include <QObject>
#include <qtmpackage.h>
#include <qtmtcpcontrol.h>
#include <qtmtcpvoiceserver.h>
#include <QTimer>
#include <QTcpSocket>

class TmRemote : public QObject
{
    Q_OBJECT
public:
    explicit TmRemote(QObject *parent = 0);
    void initConfig();
    bool enableSendCamera(bool bEnable);
    void enableVoiceSend(bool bEnable);
    void enableVoicePlayer(bool bEnable);

private:
    QTmPackage m_package;
    QTmTcpControl* pr_pTcpControl;
    QTmTCPVoiceServer* pr_pAudioServer;
    QTimer* pr_pTmCamera;
    QTcpSocket* pr_pSockCamera;
    QJsonObject pr_objRoot;

signals:

public slots:
    void msg_control_cmd(QString strCmd, QJsonValue val);
    void msg_control_discon();
    void msg_camera_send();

};

#endif // TMREMOTE_H

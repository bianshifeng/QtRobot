#ifndef QTMUDPSENDVOICE_H
#define QTMUDPSENDVOICE_H

#include <QObject>
#include <QUdpSocket>
#include "qtmpackage.h"

class QTmUDPSendVoice : public QObject
{
    Q_OBJECT
public:
    explicit QTmUDPSendVoice(QObject *parent = 0);

    bool init(QString szAddress,qint32 nPort);
    bool SendVoicePackage(QByteArray baData,int nLen);
signals:

public slots:

private:
    QString m_szIP;
    qint32 m_nPort;
    QUdpSocket m_UDPsocket;
    QTmPackage m_Package;
};

#endif // QTMUDPSENDVOICE_H

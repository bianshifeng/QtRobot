#ifndef QTMUDPRECVVOICE_H
#define QTMUDPRECVVOICE_H

#include <QObject>
#include <QUdpSocket>
#include "qtmpackage.h"
#include <QTimer>

class QTmUDPRecvVoice : public QObject
{
    Q_OBJECT
public:
    explicit QTmUDPRecvVoice(QObject *parent = 0);

    bool init(qint32 nPort);
    void uninit();

signals:
    void signal_RecvData(QByteArray buf,qint32 nBufLen);

public slots:   
    void slot_RecvPackage();

    void slot_error(QAbstractSocket::SocketError socketError);
    void slot_stateChanged(QAbstractSocket::SocketState socketState);
    void slot_timer();
private:
    QUdpSocket m_UDPsocket;
    QTmPackage m_Package;   

    bool m_bStartRecvData;

    QTimer m_connectTimer;
    qint32 m_nTimeOut;

    qint32 m_nPort;
};

#endif // QTMUDPRECVVOICE_H

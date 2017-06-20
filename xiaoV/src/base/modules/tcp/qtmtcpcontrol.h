#ifndef QTMTCPCONTROL_H
#define QTMTCPCONTROL_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QTcpServer>
#include <QHostAddress>
#include <QReadWriteLock>
#include "qtmpackage.h"
#include <QTimer>
#include <QCoreApplication>
#include <QJsonValue>

class QTmTcpControl : public QObject
{
    Q_OBJECT
public:
    explicit QTmTcpControl(QObject *parent = 0);

public:
    void startServer(int nPort);

    QString getRemoteAddr(); // 获取远程连接的地址

    void write_ack(QString strJson);
signals:
    void signal_cur_disconnect();
    void signal_msg_cmd(QString strCmd, QJsonValue val);

public slots:
    void new_tcp_connect();
    void new_tcp_data();
    void sock_disconnect();

private:
    QTcpServer m_server;
    QTcpSocket* m_curSocket;

    QByteArray m_cmdData;
    int m_cmdDataSize;
    QTmPackage m_package;

    // remote addr
    QHostAddress m_addrRemoteIP;


};

#endif // QTMTCPCONTROL_H

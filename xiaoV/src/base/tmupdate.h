#ifndef TMUPDATE_H
#define TMUPDATE_H

#include <QObject>
#include <QTcpSocket>
#include <qtmpackage.h>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

class TmUpdateWork;

class TmUpdate : public QObject
{
    Q_OBJECT
public:
    explicit TmUpdate(QObject *parent = 0);
    QString getInfo_SN();
    QString getInfo_Name();
    void initConfig();

signals:
    void sig_update_info();
    void sig_initConfig();

private:
    TmUpdateWork* pr_work;
    QThread* pr_thread;
    QTimer* pr_tmLogin;
};

class TmUpdateWork : public QObject
{
    Q_OBJECT
public:
    explicit TmUpdateWork(TmUpdate* pWork,QObject *parent = 0);

signals:
    void sig_tmstop();
public slots:
    void initConfig();

    void timeout_login();
    bool update_ip_mac();
    bool send_update_info();

private:
    QTmPackage m_package;
    QNetworkAccessManager* pr_pNet;
    QString pr_strServerIP;
    int pr_nServerPort;
    TmUpdate* pr_parent;
    bool pr_bCheckPass;
public:
    QString pr_strSN;
    QString pr_strName;
    QString pr_strVersion;
    QString pr_strDescription;
    QString pr_strCPUID;
    QString pr_strLocalIP;

};


#endif // TMUPDATE_H

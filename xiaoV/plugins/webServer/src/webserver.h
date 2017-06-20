#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <QObject>
#include "tmglobaltool.h"

class WebServer : public QObject
{
    Q_OBJECT
public:
    explicit WebServer(QObject *parent = 0);
    static QString searchConfigFile(const QString configName = "webserver");

signals:


public slots:
    void slot_webrequest_talk(QString);
};

#endif // WEBSERVER_H

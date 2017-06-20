#ifndef QMLROBOT_H
#define QMLROBOT_H

#include <QObject>
#include <QString>
#include "tmmedia.h"
#include "tmjsengine.h"
#include "tmjsenginework.h"
#include "robot.h"
#include "webserver.h"
#include "robot/servers/voiceserver/voiceconnectionhandler.h"
#include "robot/servers/voiceserver/voicerequesthandler.h"

extern WebServer* g_webServer;

class QmlRobot : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString pageJsName READ pageJsName WRITE setPageJsName NOTIFY pageJsNameChanged)

    Q_ENUMS(BehaviorStatus)
public:
    enum BehaviorStatus
    {
        MoveFore = 1,
        MoveBack,
        MoveLeft,
        MoveRight
    };
    explicit QmlRobot(QObject *parent = 0);
    ~QmlRobot();
    Q_INVOKABLE void startListen();
    Q_INVOKABLE void startSay(const QString);
    Q_INVOKABLE void asking(bool isAsking);
    Q_INVOKABLE void talking(bool isTalking);
    Q_INVOKABLE void moveBehavior(bool isUp,int distance,int speed);
    Q_INVOKABLE void rotateBehavior(bool isRight,int angle,int speed);

    Q_INVOKABLE void moveDiffBehavior(bool bForward,int leftSpeed,int rightSpeed);
    Q_INVOKABLE void moveDiffBehaviorAdvance( int leftSpeed, int rightSpeed);


    Q_INVOKABLE void moveStart();
    Q_INVOKABLE void moveStop();
    Q_INVOKABLE void moveEnd();
    Q_INVOKABLE void resetWheelCode();
    Q_INVOKABLE void queryWheelCodeStart();
    Q_INVOKABLE void queryWheelCodeStop();
    Q_INVOKABLE void queryWheelCode(bool isRight);



    Q_INVOKABLE void changePageName(QString pageName);
    Q_INVOKABLE void runJsPageModel(QString jsModelName);
    Q_INVOKABLE void pushBehaviorCommand(QByteArray data);

    Q_INVOKABLE void setCompassConfig();

    QString pageJsName() const;

signals:

    void sig_qmlrobot_ui_video_show(QString strFilePath);
    void sig_qmlrobot_ui_background_set(QString strModeName, bool bShow, QString strFileName);
    void sig_change_page_by_name(QString pageName);
    void pageJsNameChanged(QString pageJsName);
    void sig_compass_info(double roll,double pitch,double yaw);
    void sig_qmlrobot_wheel_stop_normal_status(bool isNormalStop);
    void queryWheelCodeFeedBack(int wheelValue,int leftValue,int rightValue);
    void sig_qmlrobot_cruise_info(int leftWheelCode, int rightWheelCode, double roll, double pitch, double yaw);
private:
    WebServer* m_ptrWebServer;
    Robot* m_ptrRobot;
    QString m_pageJsName;

    VoiceConnectionHandler* m_voiceHandler;



public slots:

    void slot_qmlrobot_wheelcode_info(int wheelCode,int leftWheelCode,int rightWheelCode);
    void setPageJsName(QString pageJsName);
    void slot_pageName_changed(bool);

};

#endif // QMLROBOT_H

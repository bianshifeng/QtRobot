#ifndef TMJSENGINEWORK_H
#define TMJSENGINEWORK_H

#include <QObject>
#include <QThread>
#include <QScriptEngine>

class TmJsEngineWorkThread;

class TmJsEngineWork : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int nFrameID READ getFrameID WRITE setFrameID /*NOTIFY nFrameIDChanged*/)
    Q_PROPERTY(int nFrameMSleep READ getFrameDelay WRITE setFrameDelay /*NOTIFY bStoppedChanged*/)
    Q_PROPERTY(bool bStopped READ getStopped WRITE setStopped /*NOTIFY bStoppedChanged*/)
    Q_PROPERTY(bool bIsModule READ getIsModule) // only by read
public:
    explicit TmJsEngineWork(bool bIsModule=true, QObject *parent = 0);

    bool run_js_script_content(QString strJsScript);
    bool run_js_script_by_name(QString strName);

    void incFrameID();
    void reset();
    void exit();


private:
    int pr_nFrameID;
    int pr_nFrameDelay;
    bool pr_bStopped;
    //-----
    bool pr_bIsModule;
    TmJsEngineWorkThread* pr_pWorkThread;
signals:

public slots: // Property
    int getFrameID() const;
    void setFrameID(int nFrameID);
    int getFrameDelay() const;
    void setFrameDelay(int nFrameDelay);
    bool getStopped() const;
    void setStopped(bool bStopped);
    bool getIsModule() const;

    // event slot
    void js_event_call(QString event, QString val);
};


class TmJsEngineWorkThread : public QThread
{
    Q_OBJECT
public:
    explicit TmJsEngineWorkThread(TmJsEngineWork* pWork, QObject *parent = 0);
    bool m_bExit;
    void start_load_js(QString strTxt);
    void msg_js_event(QString event, QString val);
private:
    TmJsEngineWork* pr_pWork;
    QScriptEngine* pr_pEngine;
    QScriptValue pr_func_event;
    bool pr_bStartLoad;
    QString pr_strJsCmd;
    virtual void run();
};

#endif // TMJSENGINEWORK_H

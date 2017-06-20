#include "qmlrobothead.h"

QmlRobotHead::QmlRobotHead(QObject *parent)
    : QObject(parent),
      m_isSaying(false),
      m_isListening(false),
      m_ptrTmMedia(0)
{

    m_ptrTmMedia = new TmMedia();
    m_ptrTmMedia->initConfig();


    connect(m_ptrTmMedia,SIGNAL(sig_answer_result(QString)),
            this,SLOT(slot_tuling_result(QString)));
    connect(m_ptrTmMedia,SIGNAL(sig_isr_result(QString)),
            this,SLOT(slot_audio_result(QString)));
    connect(this,SIGNAL(listenStatusChanged(bool)),
            this,SLOT(slot_listenStatusChanged(bool)));


}

QmlRobotHead::~QmlRobotHead()
{
//    if(m_ptrTmMedia!=Q_NULLPTR)
//    {
//        disconnect(m_ptrTmMedia,SIGNAL(sig_answer_result(QString)),
//                this,SLOT(slot_tuling_result(QString)));
//        disconnect(m_ptrTmMedia,SIGNAL(sig_isr_result(QString)),
//                this,SLOT(slot_audio_result(QString)));
//        m_ptrTmMedia = 0;
    //    }
}

void QmlRobotHead::startListen()
{

}

void QmlRobotHead::startSay(const QString name)
{

    m_ptrTmMedia->playMediaAudio(name);
}

void QmlRobotHead::speak(const QString Jsname)
{
    g_log("sdssssssssssssssssssssss");
    g_pJsEngineMain->run_js_script_by_name("ziding/"+Jsname);
}

bool QmlRobotHead::listenStatus() const
{
    return m_listenStatus;
}

void QmlRobotHead::slot_listenStatusChanged(bool listenStatus)
{


    if(listenStatus){
        g_log("QmlRobotHead---slot_listenStatusChanged---sig_cmd_start");
        m_ptrTmMedia->sig_cmd_start();
    }else{
        g_log("QmlRobotHead---slot_listenStatusChanged---sig_cmd_stop");
        m_ptrTmMedia->sig_cmd_stop();
    }
}

void QmlRobotHead::slot_tuling_result(QString result)
{
    g_log("QmlRobotHead---slot_tuling_result---result");
    g_log(result);
}

void QmlRobotHead::slot_audio_result(QString result)
{
    g_log("QmlRobotHead---slot_audio_result---result");
    g_log(result);
}

void QmlRobotHead::setListenStatus(bool listenStatus)
{
    if (m_listenStatus == listenStatus)
        return;

    m_listenStatus = listenStatus;

    emit listenStatusChanged(listenStatus);
}

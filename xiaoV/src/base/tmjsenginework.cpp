#include "tmjsenginework.h"
#include <tmglobaltool.h>

TmJsEngineWork::TmJsEngineWork(bool bIsModule, QObject *parent) : QObject(parent)
{
    pr_bStopped = true;
    pr_nFrameDelay = 100;
    pr_nFrameID = 0;
    pr_bIsModule = bIsModule;
    pr_pWorkThread = new TmJsEngineWorkThread(this);
    pr_pWorkThread->start();
//    /this->moveToThread(pr_pWorkThread);
}

bool TmJsEngineWork::run_js_script_content(QString strJsScript)
{
    if( strJsScript.length() > 1 ){
        pr_pWorkThread->start_load_js(strJsScript);
        return true;
    }
    return false;
}

bool TmJsEngineWork::run_js_script_by_name(QString strName)
{
    QString strFilePath = g_strExeRoot + QString("/user/scripts/%1.js").arg(strName);
    if( pr_bIsModule ){
        strFilePath = g_strExeRoot + QString("/data/base/%1.js").arg(strName);
    }
    setStopped(true);
    qDebug()<<"Load js file:"<<strFilePath;
    QFileInfo fi(strFilePath);
    if( !fi.isFile() ){
        qDebug()<<"failture js file:"<<strFilePath;
        return false;
    }
    QFile file(strFilePath);
    if( file.open(QIODevice::ReadOnly) ){
        QByteArray jsContent = file.readAll();
        run_js_script_content(QString::fromUtf8(jsContent));
        file.close();
        return true;
    }
    return false;
}

void TmJsEngineWork::incFrameID()
{
    pr_nFrameID += 1;
}

void TmJsEngineWork::reset()
{
    pr_nFrameID = 0;
    pr_bStopped = true;
}

void TmJsEngineWork::exit()
{
    pr_pWorkThread->m_bExit = true;
    pr_pWorkThread->exit();
    pr_pWorkThread->deleteLater();

}

int TmJsEngineWork::getFrameID() const
{
    return pr_nFrameID;
}

void TmJsEngineWork::setFrameID(int nFrameID)
{
    pr_nFrameID = nFrameID;
}

int TmJsEngineWork::getFrameDelay() const
{
    return pr_nFrameDelay;
}

void TmJsEngineWork::setFrameDelay(int nFrameDelay)
{
    pr_nFrameDelay = nFrameDelay;
}

bool TmJsEngineWork::getStopped() const
{
    return pr_bStopped;
}

void TmJsEngineWork::setStopped(bool bStopped)
{
    qDebug()<<"set stopped"<<bStopped;
    pr_bStopped = bStopped;
}

bool TmJsEngineWork::getIsModule() const
{
    return pr_bIsModule;
}

void TmJsEngineWork::js_event_call(QString event, QString val)
{
    //qDebug()<<"JsEngine work"<<event<<val;
    if( pr_pWorkThread ){
        qDebug()<<"JsEngine work"<<event<<val;
        pr_pWorkThread->msg_js_event(event, val);
    }
}

// ------------------------

TmJsEngineWorkThread::TmJsEngineWorkThread(TmJsEngineWork *pWork, QObject *parent)
{
    Q_UNUSED(parent);
    pr_pWork = pWork;
    pr_bStartLoad = false;
    pr_strJsCmd = "";
    m_bExit = false;
}

void TmJsEngineWorkThread::start_load_js(QString strTxt)
{
    pr_pWork->setStopped(true);
    pr_bStartLoad = true;
    pr_strJsCmd = strTxt;
    //qDebug()<<"Js"<<strTxt;
}

void TmJsEngineWorkThread::msg_js_event(QString event, QString val)
{
    qDebug()<<"msg js event:"<<pr_func_event.toString()<<event<<val;
    if( pr_func_event.isValid() && pr_func_event.isFunction() ){
        QScriptValueList valList;
        valList << QScriptValue(event)<<QScriptValue(val);
        pr_func_event.call(QScriptValue(), valList);
    }
}

void TmJsEngineWorkThread::run()
{
    qDebug()<<"Module?:"<<pr_pWork->getIsModule()<<"TmJsEngineWorkThread ThreadID:"<<currentThreadId();
    // init
    if( !g_pRobot ){
        g_log("TmJsEngineWorkThread g_pRobot is Null! exit!");
        return;
    }
    pr_pEngine = new QScriptEngine();
    QScriptValue val1 = pr_pEngine->newQObject(g_pRobot);
    pr_pEngine->globalObject().setProperty("Robot", val1);
    QScriptValue val2 = pr_pEngine->newQObject(pr_pWork);
    pr_pEngine->globalObject().setProperty("jsState", val2);

    while(true){
        if( m_bExit ){
            break;
        }

        if( !pr_bStartLoad ){
            QCoreApplication::processEvents(QEventLoop::AllEvents, 10);
            msleep(100);
            continue;
        }

        // 延时大约1500加载
//        for( int i=0;i<15;i++ ){
//            QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
//            msleep(100);
//        }
        g_pRobot->log("Js Engine is loading...");
        pr_bStartLoad = false;
        pr_pWork->reset();
        // run
//        if(pr_pWork->getStopped()){
//            return;
//        }
        // 1.
        QScriptValue valret = pr_pEngine->evaluate(pr_strJsCmd);
        if( valret.isError() ){
            QScriptSyntaxCheckResult retCheck = pr_pEngine->checkSyntax(pr_strJsCmd);
            QString strRet = QString("[%1:%2] %3").arg(retCheck.errorLineNumber()).arg(retCheck.errorColumnNumber()).arg(retCheck.errorMessage());
            g_pRobot->log("Js Engine load error!: "+strRet);
        }else{
            pr_pWork->setStopped(false);
            g_pRobot->log("Js Engine load success! ");
        }
        // 2.
        QScriptValue func_loop = pr_pEngine->globalObject().property("jsLoop");
        if( !func_loop.isValid() || !func_loop.isFunction() ){
            pr_pWork->reset();
            continue;
        }
        pr_func_event = pr_pEngine->globalObject().property("jsEvent"); // set event func
        //qDebug()<<pr_func_event.toString();
        int nLastFrameID = 0;
        while( !pr_pWork->getStopped() ){
            nLastFrameID = pr_pWork->getFrameID();
            func_loop.call();
            if( nLastFrameID == pr_pWork->getFrameID() ){
                pr_pWork->incFrameID();
            }
            //qDebug()<<(pr_pWork->getIsModule()?"[Module]":"[Main  ]")<<"Js Frame:"<<pr_pWork->getFrameID();
            msleep(pr_pWork->getFrameDelay());
        }


        g_log("run here--------------------------------------------");

        g_pTmMedia->isr_stop();
        if( g_pKinect && g_pKinect->getIsOpened() ){
            g_pKinect->stopRun();
        }
        g_pTmMedia->stop();
        g_pTmMedia->stop(1);
        g_pTmSerial->json_load_stop();
        if( !pr_pWork->getIsModule() ){ // 主模块里停掉子模块
            g_pJsEngineMgr->load_js_module_stop();
        }
        pr_func_event = QScriptValue(); // reset event
        g_pRobot->log("Js Engine stopped!");
    }

    delete pr_pEngine;
}

#include "tmjsengine.h"
#include "tmglobaltool.h"


TmJsEngine::TmJsEngine(QObject *parent) : QObject(parent)
{

}

void TmJsEngine::exit()
{
    g_pJsEngineMain->exit();
    g_pJsEngineMain->deleteLater();
    g_pJsEngineModule->exit();
    g_pJsEngineModule->deleteLater();
}

void TmJsEngine::initConfig()
{
    // 1.
    if( !g_pRobot ){
        g_pRobot = new TmRobotInterface();
    }
    // 2.

    if( !g_pJsEngineModule ){
        g_log("g_pJsEngineModule-----------------------------");
        g_pJsEngineModule = new TmJsEngineWork(true);
    }

    if( !g_pJsEngineMain ){
        g_log("g_pJsEngineModule-----------------------------");
        g_pJsEngineMain = new TmJsEngineWork(false);
    }
}

bool TmJsEngine::load_js_main_by_name(QString strName)
{
    if( g_pJsEngineMain ){
        return g_pJsEngineMain->run_js_script_by_name(strName);
    }
    return false;
}

bool TmJsEngine::load_js_module_by_name(QString strName)
{
    if( g_pJsEngineModule ){
        return g_pJsEngineModule->run_js_script_by_name(strName);
    }
    return false;
}

bool TmJsEngine::load_js_main_content(QString strJsContent)
{
    if( g_pJsEngineMain ){
        return g_pJsEngineMain->run_js_script_content(strJsContent);
    }
    return false;
}

bool TmJsEngine::load_js_module_content(QString strJsContent)
{
    if( g_pJsEngineModule ){
        return g_pJsEngineModule->run_js_script_content(strJsContent);
    }
    return false;
}

bool TmJsEngine::load_js_main_isRunning()
{
    if( g_pJsEngineMain ){
        return !g_pJsEngineMain->getStopped();
    }
    return false;
}

bool TmJsEngine::load_js_module_isRunning()
{
    if( g_pJsEngineModule ){
        return !g_pJsEngineModule->getStopped();
    }
    return false;
}

void TmJsEngine::load_js_main_stop()
{
    if( g_pJsEngineMain ){
        g_pJsEngineMain->setStopped(true);
    }
}

void TmJsEngine::load_js_module_stop()
{
    if( g_pJsEngineModule ){
        g_pJsEngineModule->setStopped(true);
    }
}

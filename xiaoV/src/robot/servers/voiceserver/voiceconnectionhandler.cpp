#include "voiceconnectionhandler.h"
#include <QDebug>

VoiceConnectionHandler::VoiceConnectionHandler(QObject *parent) : QThread(parent)
{
    moveToThread(this);
    m_readTimer.moveToThread(this);
    m_readTimer.setSingleShot(true);
    connect(&m_readTimer,SIGNAL(timeout()),this,SLOT(slot_timeout()));
    this->start();
}

VoiceConnectionHandler::~VoiceConnectionHandler()
{
    this->quit();
    this->wait();

}

void VoiceConnectionHandler::slot_timeout()
{
    qDebug("---------------------------------------VoiceConnectionHandler::slot_timeout()");
}

void VoiceConnectionHandler::slot_startTimer(QString name)
{
    qDebug() << "void VoiceConnectionHandler::slot_startTimer()";
    m_readTimer.start(1000);
}

void VoiceConnectionHandler::run()
{
    try{
        qDebug("sdfsfsfsdsdfsdfsf");
        exec();
    }catch(...){
        qCritical("sdfsdfsf");
    }
}


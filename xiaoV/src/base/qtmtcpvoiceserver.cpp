#include "qtmtcpvoiceserver.h"
#include "tmglobaltool.h"

QTmTCPVoiceServer::QTmTCPVoiceServer(QObject *parent) : QObject(parent)
{
    pr_pServer = NULL;
    pr_pSocket = NULL;
    pr_pOutputAudio = NULL;
    pr_pOutputDevice = NULL;
    pr_pInputAudio = NULL;
    pr_pInputDevice = NULL;
    connect(this, SIGNAL(sig_enableAudioReceive(bool)), this, SLOT(slot_enableAudioReceive(bool)));
    connect(this, SIGNAL(sig_enableAudioSend(bool)), this, SLOT(slot_enableAudioSend(bool)));
}

void QTmTCPVoiceServer::initConfig()
{
    QJsonObject remoteObj = g_configJson.value("Remote").toObject();
    pr_nPort = remoteObj.value("audio_port").toInt(6604);
    slot_server_open(true);
}

void QTmTCPVoiceServer::slot_enableAudioSend(bool bStart)
{
    if( bStart ){
        if( pr_pInputAudio ){
            return;
        }
        QAudioFormat format;
        format.setSampleRate(8000);
        format.setChannelCount(1);
        format.setSampleSize(16);
        format.setCodec("audio/pcm");
        format.setByteOrder(QAudioFormat::LittleEndian);
        format.setSampleType(QAudioFormat::UnSignedInt);

        QAudioDeviceInfo inputDeviceInfo = QAudioDeviceInfo::defaultInputDevice();
        QString str=inputDeviceInfo.deviceName();
        //qDebug()<<str.toLatin1().data()<<str.toStdString().data();
        g_log("use input device:"+str);
        if(!inputDeviceInfo.isFormatSupported(format))
        {
            format = inputDeviceInfo.nearestFormat(format);
            qWarning()<<"[Input]raw audio format not supported by backend, cannot record audio!";
            return;
        }

        //创建录音获取对象
        pr_pInputAudio = new QAudioInput(inputDeviceInfo,format,this);
        pr_pInputDevice = pr_pInputAudio->start();
        connect(pr_pInputDevice, SIGNAL(readyRead()), this, SLOT(slot_send_audio()));
    }else{
        if( !pr_pInputAudio ){
            return;
        }
        disconnect(pr_pInputDevice, SIGNAL(readyRead()), this, SLOT(slot_send_audio()));
        pr_pInputAudio->stop();
        //pr_pInputAudio->deleteLater();
        pr_pInputAudio = NULL;
        pr_pInputDevice = NULL;
    }
}

void QTmTCPVoiceServer::slot_enableAudioReceive(bool bStart)
{
    if( bStart ){
        if( pr_pOutputAudio ){
            return;
        }
        QAudioFormat format;
        format.setSampleRate(8000);
        format.setChannelCount(1);
        format.setSampleSize(16);
        format.setCodec("audio/pcm");
        format.setByteOrder(QAudioFormat::LittleEndian);
        format.setSampleType(QAudioFormat::UnSignedInt);

        QAudioDeviceInfo outputDeviceInfo = QAudioDeviceInfo::defaultOutputDevice();
        QString str=outputDeviceInfo.deviceName();
        //qDebug()<<str.toLatin1().data()<<str.toStdString().data();
        g_log("use output device:"+str);
        if(!outputDeviceInfo.isFormatSupported(format))
        {
            format = outputDeviceInfo.nearestFormat(format);
            qWarning()<<"[Output]raw audio format not supported by backend, cannot play audio!";
            return;
        }

        //创建录音获取对象
        pr_pOutputAudio = new QAudioOutput(outputDeviceInfo,format,this);
        pr_pOutputDevice = new QTmVoiceIODevice();
        pr_pOutputAudio->start(pr_pOutputDevice);
    }else{
        if( !pr_pOutputAudio ){
            return;
        }
        pr_pOutputAudio->stop();
        //pr_pOutputAudio->deleteLater(); // 千万不能加这一句, 让内存管理自动释放
        pr_pOutputAudio = NULL;
        //pr_pOutputDevice->deleteLater();
        pr_pOutputDevice = NULL;
    }
}

void QTmTCPVoiceServer::slot_server_open(bool bOpen)
{
    if( pr_pServer == NULL ){
        pr_pServer = new QTcpServer();
        connect(pr_pServer, SIGNAL(newConnection()), this, SLOT(slot_new_connection()));
    }

    if( bOpen ){
        if( !pr_pServer->isListening() ){
            pr_pServer->listen(QHostAddress::Any, pr_nPort);
        }
    }else{
        if( pr_pServer->isListening() ){
            pr_pServer->close();
        }
    }
}

void QTmTCPVoiceServer::slot_new_connection()
{
    if( pr_pSocket ){
        disconnect(pr_pSocket, SIGNAL(readyRead()), this, SLOT(slot_new_data()));
        pr_pSocket->close();
    }
    pr_pSocket = pr_pServer->nextPendingConnection();
    connect(pr_pSocket, SIGNAL(readyRead()), this, SLOT(slot_new_tcp_data()));
}

void QTmTCPVoiceServer::slot_new_tcp_data()
{
    // send data
    if( !pr_pOutputAudio ){
        return;
    }
    QByteArray bArray = pr_pSocket->readAll();
    QByteArray bData = pr_package.parse_full_package_audio(bArray);
    pr_pOutputDevice->writeData(bData.data(), bData.size());
    //qDebug()<<"[Audio Write]"<<bArray.size();
}

void QTmTCPVoiceServer::slot_send_audio()
{
    if( !pr_pInputDevice ){
        return;
    }
    QByteArray bArray = pr_pInputDevice->readAll();
    if( pr_pSocket ){
        if( pr_pSocket->state() == QAbstractSocket::ConnectedState){
            QByteArray bPackage = pr_package.make_full_package_audio(bArray);
            pr_pSocket->write(bPackage);
            //pr_pSocket->flush();
            //qDebug()<<"[Audio Send]"<<bArray.size();
        }else{
            qDebug()<<"Audio Send Error! disconnect socket!";
            disconnect(pr_pSocket, SIGNAL(readyRead()), this, SLOT(slot_new_data()));
            pr_pSocket->close();
            pr_pSocket = NULL;
        }
    }
}

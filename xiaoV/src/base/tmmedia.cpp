#include "tmmedia.h"
#include <tmglobaltool.h>

TmMedia::TmMedia(QObject *parent) : QObject(parent)
{
    pr_pMediaWork = NULL;
    pr_inputAudio = NULL;
    pr_inputDevice = NULL;
    pr_nInputState = 0;
    pr_nInputFreeCount = 0;
    pr_nInputIsStart = 0;
}

void TmMedia::initConfig()
{
    // tts init
    QJsonObject root = g_configJson.value("TTS").toObject();
    QString tts_speaker = root.value("speaker").toString("nannan");
    int tts_speed = root.value("speed").toInt();
    int tts_pitch = root.value("pitch").toInt();
    int tts_special = root.value("special").toInt();

    // cmd isr
    QJsonObject isrObj = g_configJson.value("ISR").toObject();
    pr_strIsrTag = isrObj.value("tag_name").toString();
    pr_nIsrConfidence = isrObj.value("confidence").toInt();

    pr_pkdxf = new Sharedlib_kdxf();
    pr_pkdxf->login("556d116e");
    if( tts_special >= Sharedlib_kdxf::Special_Counts ){
        tts_special = 0;
    }
    QString strGramName = isrObj.value("gram_name").toString();
    pr_pkdxf->isr_setGramID(strGramName);
    pr_pkdxf->setDefaultParams(tts_speaker, tts_speed, tts_pitch, (Sharedlib_kdxf::Special_Type)tts_special);
    pr_strTTSParams = QString("%1_%2_%3_%4_").arg(tts_speaker).arg(tts_speed).arg(tts_pitch).arg((int)tts_special);
    // media work
    pr_pMediaWork = new TmMediaWork();
//    pr_pMediaWorkThread = new QThread();
//    pr_pMediaWork->moveToThread(pr_pMediaWorkThread);
//    pr_pMediaWorkThread->start();

    connect(this, SIGNAL(initMedia()), pr_pMediaWork, SLOT(initMedia()));
    connect(this, SIGNAL(setMedia(QUrl,int)), pr_pMediaWork, SLOT(setMedia(QUrl,int)));
    connect(this, SIGNAL(sig_play(int)), pr_pMediaWork, SLOT(play(int)));
    connect(this, SIGNAL(sig_stop(int)), pr_pMediaWork, SLOT(stop(int)));
    connect(this, SIGNAL(setVolume(int,int)), pr_pMediaWork, SLOT(setVolume(int,int)));
    connect(this, SIGNAL(sig_isr_start()), this, SLOT(slot_isr_start()));
    connect(this, SIGNAL(sig_cmd_start()), this, SLOT(slot_cmd_start()));
    connect(this, SIGNAL(sig_cmd_stop()), this, SLOT(slot_cmd_stop()));

    // init media in thread
    emit initMedia();
    pr_httpGet = new QNetworkAccessManager(this);
    connect(pr_httpGet,SIGNAL(finished(QNetworkReply*)),this,SLOT(tuling_replyFinish(QNetworkReply*)));

    // init dir
    QDir dirTTS(g_strExeRoot+"/data/tts");
    if( !dirTTS.exists() ){
        dirTTS.mkpath(g_strExeRoot+"/data/tts");
    }

    g_log("bsf-------------------------------tmmedia");
}

void TmMedia::exit()
{
//    pr_pMediaWorkThread->exit();
//    pr_pMediaWorkThread->deleteLater();
    pr_pMediaWork->deleteLater();
}

void TmMedia::playMediaAudio(QString strName, int nIndex)
{


    if( strName.length() < 1 ){
        return;
    }
    QString strFilePath = g_strExeRoot + QString("/data/audio/%1.wav").arg(strName);



    emit setMedia(QUrl::fromLocalFile(strFilePath), nIndex);
    emit sig_play(nIndex);
}

void TmMedia::playMediaTTS(QString strFilePath)
{
    if( strFilePath.length() < 1 ){
        return;
    }
    emit setMedia(QUrl::fromLocalFile(strFilePath), 0);
    emit sig_play(0);
}

bool TmMedia::playMediaTTS_Params(QString strTxt, QString strSpeaker, int nSpeed, int nPitch, int nSpecial)
{
    QString strParams = QString("%1_%2_%3_%4_").arg(strSpeaker).arg(nSpeed).arg(nPitch).arg(nSpecial);
    QString strMd5Path = g_strExeRoot + QString("/data/tts/%1.wav").arg(g_strGetMD5(strParams+strTxt));
    QFileInfo fi(strMd5Path);
    if( fi.isFile() ){
        return true;
    }
    // no file, create it
    if( pr_pkdxf ){
        int nLen = pr_pkdxf->getTTSfile(strTxt, strMd5Path, strSpeaker, nSpeed, nPitch, nSpecial);
        g_log("TmMedia::playMediaTTS_Params:"+strParams+strTxt+" "+strMd5Path);
        if( nLen < 46 ){
            qDebug()<<"TmMedia::playMediaTTS_Params, tts get failed! size:"<<nLen;
            QFile f(strMd5Path);
            f.remove();
            return false;
        }
        return true;
    }
    return false;
}

bool TmMedia::isPlaying(int nIndex)
{
    return pr_pMediaWork->isPlaying(nIndex);
}

void TmMedia::play(int nIndex)
{
    emit sig_play(nIndex);
}

void TmMedia::stop(int nIndex)
{
    emit sig_stop(nIndex);
}

void TmMedia::isr_start()
{
    emit sig_isr_start();
}

void TmMedia::slot_isr_start()
{
    if( pr_inputAudio ){
        pr_nInputIsStart = 1;
        return;
    }
    QAudioFormat format;
    format.setSampleRate(16000);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    QAudioDeviceInfo inputDeviceInfo = QAudioDeviceInfo::defaultInputDevice();
    if(!inputDeviceInfo.isFormatSupported(format))
    {
        format = inputDeviceInfo.nearestFormat(format);
        qWarning()<<"raw audio format not supported by backend, cannot play audio 1.";
        return ;
    }

    //创建录音获取对象
    if( !pr_inputAudio ){
        pr_inputAudio = new QAudioInput(inputDeviceInfo,format,this);
    }
    pr_inputDevice = pr_inputAudio->start();
    connect(pr_inputDevice, SIGNAL(readyRead()), this, SLOT(slot_audio_data()));
    pr_nInputState = 1;
    pr_nInputFreeCount = 0;
    pr_nInputIsStart = 0;
}

void TmMedia::isr_pause(bool bPause)
{
    if( bPause ){
        if( pr_nInputState == 1 ){
            pr_nInputState = 2;
        }
    }else{
        if( pr_nInputState == 2 ){
            pr_nInputState = 1;
        }
    }
    qDebug()<<"Intel state:"<<pr_nInputState;
}

void TmMedia::isr_stop()
{
    pr_nInputState = 0;
}

void TmMedia::tuling_answer(QString strAsk)
{
    if( strAsk.length() < 1 ){
        return;
    }
    QString strUrl = QString("http://www.tuling123.com/openapi/api?key=67c4bf883e1bdccf7f19204ec8aa927c&userid=%1&info=%2")
            .arg(g_pUpdate->getInfo_Name()).arg(strAsk);
    pr_httpGet->get(QNetworkRequest(QUrl(strUrl)));
}

int TmMedia::getValuedSamples(QByteArray &bArry)
{
    // 1 channel 16 bit
    char* pData = bArry.data();
    int nLen = bArry.length();
    int nSamples = nLen / ( 16 / 8 );
    int nCounts = 0;
    for( int i=0; i<nSamples; i++ ){
        short* pValue = (short*)(pData + i*(16/8));
        if( abs(*pValue) > 2000 ){
            nCounts += 1;
        }
    }
    //qDebug()<<"Sample value counts:"<<nCounts;
    return nCounts;
}

QString TmMedia::parser_tuling_json(QByteArray &bArry)
{
    QJsonDocument jsDoc;
    QJsonParseError error;
    jsDoc = QJsonDocument::fromJson(bArry, &error);
    if( error.error != QJsonParseError::NoError ){
        return "你欺负我，小薇不喜欢你了！";
    }
    QJsonObject jsObj = jsDoc.object();
    QString strRet = "";
    int nCode = jsObj.value("code").toInt();
    switch (nCode) {
    case 100000:
        {
            strRet = jsObj.value("text").toString();
        }
        break;
    case 200000:
        {
            strRet = "它的网址是:"+jsObj.value("url").toString();
        }
        break;
    case 302000:
        {
            QJsonArray jsArry = jsObj.value("list").toArray();
            int nCur = g_nGetRandom(0, jsArry.size()-1);
            strRet = "小薇已经精心为你挑选了一条新闻:"+jsArry.at(nCur).toObject().value("article").toString();
        }
        break;
    case 305000:
        {
            QJsonObject oneitem = jsObj.value("list").toArray().at(0).toObject();
            QString strInfo = QString("%1,%2出发，%3到站").arg(oneitem.value("trainnum").toString())
                    .arg(oneitem.value("starttime").toString()).arg(oneitem.value("endtime").toString());
            strRet = "小薇已经为你挑选了最合适的列车:"+strInfo;
        }
        break;
    case 306000:
        {
            QJsonObject oneitem = jsObj.value("list").toArray().at(0).toObject();
            QString strInfo = QString("%1,%2出发，%3到站").arg(oneitem.value("flight").toString())
                    .arg(oneitem.value("starttime").toString()).arg(oneitem.value("endtime").toString());
            strRet = "小薇已经为你挑选了最合适的航班:"+strInfo;
        }
        break;
    case 308000:
        {
            QJsonObject oneitem = jsObj.value("list").toArray().at(0).toObject();
            QString strInfo = QString("%1的菜谱为%2").arg(oneitem.value("name").toString())
                    .arg(oneitem.value("info").toString());
            strRet = "让小薇告诉你菜谱吧，";
        }
        break;
    default:
        strRet = "亲爱的,小薇知错啦，我会更加努力学习,跟上您的步伐的.";
        break;
    }
    if( nCode>40000 && nCode<50000 ){
        QString strOut = QString("tuling error! code:%1").arg(nCode);
        g_log(strOut);
    }
    return strRet;
}

void TmMedia::slot_audio_data()
{
    QByteArray bArry = pr_inputDevice->readAll();
    // 0 stop, 1 run, 2 pause
    if( pr_nInputState == 2 ){
        //qDebug()<<"talk state:"<<pr_nInputState<<pr_nInputIsStart<<pr_nInputFreeCount;
        return;
    }else if( pr_nInputState == 0 ){
        qDebug()<<"talk state:"<<pr_nInputState<<pr_nInputIsStart<<pr_nInputFreeCount;
        if( pr_inputAudio ){
            pr_inputAudio->stop();
            pr_inputAudio = NULL;
            disconnect(pr_inputDevice, SIGNAL(readyRead()), this, SLOT(slot_audio_data()));
            pr_inputDevice->close();
            pr_pkdxf->isr_stop();
            //pr_inputAudio->deleteLater();
            pr_inputAudio = NULL;
        }
        return;
    }
    // run
    int nVal = getValuedSamples(bArry);
    if( nVal > 0 ){
        qDebug()<<pr_nInputIsStart<<"->"<<nVal<<pr_nInputState<<pr_nInputFreeCount;;
    }

    if( pr_nInputIsStart == 0 && nVal > 1 ){
        pr_inputbuff = bArry;
        pr_nInputIsStart = 1;
    }else if( pr_nInputIsStart == 1 ){
        if( nVal > 10 ){
            pr_inputbuff += bArry;
            pr_nInputIsStart = 2;
        }else{
            pr_nInputIsStart = 0;
        }
    }else if( pr_nInputIsStart == 2 ){
        if( nVal > 10 ){
            pr_inputbuff += bArry;
            pr_nInputIsStart = 5;
        }else{
            pr_nInputIsStart = 0;
        }
        // 前三次作为语音开头的判断
    }else if( pr_nInputIsStart == 5 ){
        if( nVal > 10 ){
            pr_pkdxf->isr_start();
            pr_pkdxf->isr_updateAudio(pr_inputbuff.data(), pr_inputbuff.size());
            pr_pkdxf->isr_updateAudio(bArry.data(), bArry.size());
            pr_nInputIsStart = 6;
            pr_nInputFreeCount = 0;
        }else{
            pr_nInputIsStart = 0;
        }
    }else if( pr_nInputIsStart == 6 ){
        if( nVal > 5 ){
            pr_nInputFreeCount = 0;
        }else{
            pr_nInputFreeCount += 1;
        }
        pr_pkdxf->isr_updateAudio(bArry.data(), bArry.size());
        if( pr_nInputFreeCount > 3 ){
            QString strVoice = pr_pkdxf->isr_getResult();
            pr_pkdxf->isr_stop();
            //qDebug()<<strVoice.toStdString().data();
            pr_nInputIsStart = 0;
            pr_nInputFreeCount = 0;
            //tuling_answer(strVoice);
            emit sig_isr_result(strVoice);
        }
    }
}

void TmMedia::tuling_replyFinish(QNetworkReply *reply)
{
    QByteArray bReply =reply->readAll();
    QString strAnswer = parser_tuling_json(bReply);
    if( strAnswer.length() > 100 ){
        strAnswer = strAnswer.left(100);
        int nPos = strAnswer.lastIndexOf("。");
        if( nPos < 1 ){
            nPos = strAnswer.lastIndexOf(".");
            if( nPos < 1 ){
                nPos = strAnswer.lastIndexOf("，");
                if( nPos < 1 ){
                    nPos = strAnswer.lastIndexOf(",");
                    if( nPos < 1 ){
                        nPos = strAnswer.lastIndexOf(" ");
                    }
                }
            }
        }
        strAnswer = strAnswer.left(nPos+1);
    }
    emit sig_answer_result(strAnswer);
}

void TmMedia::slot_cmd_start()
{


    if(pr_inputDevice){

        return;
    }
    pr_pkdxf->isr_start(true, false);


    QAudioFormat format;
    format.setSampleRate(16000);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    QAudioDeviceInfo inputDeviceInfo = QAudioDeviceInfo::defaultInputDevice();
    if(!inputDeviceInfo.isFormatSupported(format))
    {
        format = inputDeviceInfo.nearestFormat(format);
        qWarning()<<"raw audio format not supported by backend, cannot play audio 1.";
        return ;
    }

    //创建录音获取对象
    if( !pr_inputAudio ){
        pr_inputAudio = new QAudioInput(inputDeviceInfo,format,this);
    }
    pr_inputDevice = pr_inputAudio->start();

    connect(pr_inputDevice, SIGNAL(readyRead()), this, SLOT(slot_audio_cmd_data()));
}

void TmMedia::slot_cmd_stop()
{
    if( pr_inputDevice == NULL ){
        return;
    }
    pr_inputAudio->stop();
    pr_inputAudio = NULL;
    pr_inputDevice->close();
    connect(pr_inputDevice, SIGNAL(readyRead()), this, SLOT(slot_audio_cmd_data()));
    pr_inputDevice = NULL;

    // get result
    QString strVoice = pr_pkdxf->isr_getResult();
    pr_pkdxf->isr_stop();

    // xml 命令
    int nCurConfidence = 0;
    QString strRet = slot_parserXmlResult(strVoice, pr_strIsrTag, &nCurConfidence);
    qDebug()<<"Isr result:"<<strRet.toStdString().data()<<nCurConfidence<<"/"<<pr_nIsrConfidence;
    if( (strRet.length()>0) && (nCurConfidence>pr_nIsrConfidence)){ // 可信度判别
        emit sig_isr_result(strRet);
    }else{
        qDebug()<<"Xml Data:"<<strVoice;
    }
}

void TmMedia::slot_audio_cmd_data()
{
    QByteArray bArray = pr_inputDevice->readAll();
    //qDebug()<<"isr update audio!"<<bArray.size();
    pr_pkdxf->isr_updateAudio(bArray.data(), bArray.size());
}

QString TmMedia::slot_parserXmlResult(QString strXml, QString strTagName, int *nConfidence)
{
    QDomDocument xmlDoc;
    QString strError;
    int nErrorLine, nErrorColum;
    //qDebug()<<strXml;
    if( !xmlDoc.setContent(strXml, &strError, &nErrorLine, &nErrorColum) ){
        QString strOut = QString("Xml Parse Error! [%1:%2] %3").arg(nErrorLine).arg(nErrorColum).arg(strError);
        g_log(strOut);
        return "";
    }
    // pase obj
    QDomElement root = xmlDoc.documentElement();
    if( root.isNull() ){
        qDebug()<<"xml Root is NULL!";
        return "";
    }

    QDomNodeList nodelist = root.elementsByTagName("confidence");
    if( nodelist.length()<1 ){
        qDebug()<<"Nodelist is null";
        return "";
    }
    QString strConfidence = nodelist.at(0).toElement().text();
    if( nConfidence ){
        *nConfidence = strConfidence.toInt(); // 返回可信度
    }
    // result
    nodelist = root.elementsByTagName("result");
    if( nodelist.length()>0 ){
        nodelist = nodelist.at(0).toElement().elementsByTagName("object");
        if( nodelist.length()>0 ){
            QDomNodeList objList = nodelist.at(0).toElement().childNodes();
            QDomElement oneEle;
            for(int i=0; i<objList.size();i++){
                oneEle = objList.at(i).toElement();
                if( oneEle.tagName() == strTagName ){
                    return oneEle.text();
                }
            }
        }
    }
    return "";
}

QString TmMedia::getTTSFilePath(QString strTxtSay)
{
    QString strMd5Path = g_strExeRoot + QString("/data/tts/%1.wav").arg(g_strGetMD5(pr_strTTSParams+strTxtSay));
    QFileInfo fi(strMd5Path);
    if( fi.isFile() ){
        return strMd5Path;
    }
    // no file, create it
    if( pr_pkdxf ){
        int nLen = pr_pkdxf->getTTSfile(strTxtSay, strMd5Path,false);
        g_log("TmMedia::getTTSFilePath:"+pr_strTTSParams+strTxtSay+" "+strMd5Path);
        if( nLen < 46 ){
            qDebug()<<"TmMedia::getTTSFilePath, tts get failed! size:"<<nLen;
            QFile f(strMd5Path);
            f.remove();
            return "";
        }
        return strMd5Path;
    }
    return "";
}



#ifndef TMMEDIA_H
#define TMMEDIA_H

/*
 * Used to control TmMedia work in thread.
 * also to make tts file and change voice with soundtouch.
 */

#include <QObject>
#include <tmmediawork.h>
#include <sharedlib_kdxf.h>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class TmMedia : public QObject
{
    Q_OBJECT
public:
    explicit TmMedia(QObject *parent = 0);

    void initConfig();
    void exit();

    // audio
    void playMediaAudio(QString strName, int nIndex=0); // path "./data/audio/xxx.wav"
    // tts
    QString getTTSFilePath(QString strTxtSay);
    void playMediaTTS(QString strFilePath); // path "./data/tts/xxx.wav"
    bool playMediaTTS_Params(QString strTxt, QString strSpeaker, int nSpeed, int nPitch, int nSpecial);
    // control
    bool isPlaying(int nIndex=0);
    void play(int nIndex=0);
    void stop(int nIndex=0);
    // isr
    void isr_start();
    void isr_pause(bool bPause);
    void isr_stop();
    // tuling
    void tuling_answer(QString strAsk);
private:
    TmMediaWork* pr_pMediaWork;
    QThread* pr_pMediaWorkThread;
    Sharedlib_kdxf* pr_pkdxf;
    QString pr_strTTSParams;
    QNetworkAccessManager* pr_httpGet;
    QAudioInput* pr_inputAudio;
    QIODevice* pr_inputDevice;
    int getValuedSamples(QByteArray& bArry);
    int pr_nInputIsStart, pr_nInputFreeCount;
    int pr_nInputState; // 0 stop, 1 run, 2 pause
    QByteArray pr_inputbuff;
    QString parser_tuling_json(QByteArray& bArry);

    QString pr_strIsrTag;
    int pr_nIsrConfidence;
signals:
    void initMedia();
    void setMedia(QUrl urlPath, int nIndex); // for fullname or net addr
    void setVolume(int nVol, int nIndex);
    void sig_play( int nIndex );
    void sig_stop( int nIndex );

    // isr
    void sig_isr_start();

    /*
     * sdfsdfsf
     */
    void sig_isr_result(QString result);
    void sig_answer_result(QString result);

    void sig_cmd_start();
    void sig_cmd_stop();

private slots:
    void slot_isr_start();
    void slot_audio_data();
    void tuling_replyFinish(QNetworkReply* reply);

    void slot_audio_cmd_data();

    QString slot_parserXmlResult(QString strXml, QString strTagName, int* nConfidence=NULL);

public slots:
    void slot_cmd_start();
    void slot_cmd_stop();
};

#endif // TMMEDIA_H

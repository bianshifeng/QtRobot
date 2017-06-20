#ifndef TMMEDIAWORK_H
#define TMMEDIAWORK_H
/*
 * Play Music, TTS, SoundTouch
 */
#include <QObject>
#include <QMediaPlayer>
#include <QAudioInput>
#include <QIODevice>

#define TmMediaWork_MAX 2 // max audio out channels

class TmMediaWork : public QObject
{
    Q_OBJECT
public:
    explicit TmMediaWork(QObject *parent = 0);
    bool isPlaying(int nIndex);
private:
    QMediaPlayer* pr_pMedia;
signals:

public slots:
    void initMedia();
    void setMedia(QUrl urlPath, int nIndex); // for fullname or net addr
    void setVolume(int nVol, int nIndex);
    void play( int nIndex );
    void stop( int nIndex );
};

#endif // TMMEDIAWORK_H

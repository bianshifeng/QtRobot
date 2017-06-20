#include "tmmediawork.h"

TmMediaWork::TmMediaWork(QObject *parent) : QObject(parent)
{
    pr_pMedia = NULL;
}

bool TmMediaWork::isPlaying(int nIndex)
{
    if( pr_pMedia && nIndex < TmMediaWork_MAX ){
        QMediaPlayer::State state = pr_pMedia[nIndex].state();
        if( state == QMediaPlayer::PlayingState ){
            return true;
        }
    }
    return false;
}

void TmMediaWork::initMedia()
{
    pr_pMedia = new QMediaPlayer[TmMediaWork_MAX];
    for( int i=0; i< TmMediaWork_MAX; i++){
        pr_pMedia[i].setVolume(99);
    }
}

void TmMediaWork::setMedia(QUrl urlPath, int nIndex)
{

    if( pr_pMedia && nIndex < TmMediaWork_MAX ){
        pr_pMedia[nIndex].setMedia(urlPath);
        //qDebug()<<urlPath.toString();
    }
}

void TmMediaWork::setVolume(int nVol, int nIndex)
{
    if( pr_pMedia && nIndex < TmMediaWork_MAX ){
        pr_pMedia[nIndex].setVolume(nVol);
    }
}

void TmMediaWork::play(int nIndex)
{
    if( pr_pMedia && nIndex < TmMediaWork_MAX ){
        pr_pMedia[nIndex].play();
    }
}

void TmMediaWork::stop(int nIndex)
{
    if( pr_pMedia && nIndex < TmMediaWork_MAX ){
        pr_pMedia[nIndex].stop();
    }
}



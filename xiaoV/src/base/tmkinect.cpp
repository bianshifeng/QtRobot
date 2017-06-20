#include "tmkinect.h"
#include <QDebug>
#include "tmglobaltool.h"

TmKinect::TmKinect(QObject *parent) : QObject(parent)
{
    pr_libKinect = NULL;
    pr_kinect_open = NULL;
    pr_kinect_getAudioAngle = NULL;
    pr_kinect_getHandAngle = NULL;
    pr_kinect_close = NULL;
    pr_bIsOpened = false;

    pr_infoList.append(0);
    pr_infoList.append(0);
    pr_infoList.append(0);
    pr_infoList.append(0);
    pr_infoList.append(0);
    pr_infoList.append(0);
    pr_libKinect = new QLibrary("KinectV2Driver");
    if( !pr_libKinect->load() ){
        qDebug()<<"Load library KinectV2Driver error!"<<pr_libKinect->errorString().toStdString().data();
        return;
    }
    // load functions
    pr_kinect_open = (OPENKINECT)pr_libKinect->resolve("OpenKinect");
    pr_kinect_close = (CLOSEKINECT)pr_libKinect->resolve("CloseKinect");
    pr_kinect_getAudioAngle = (GETAUDIOANGLE)pr_libKinect->resolve("GetAudioAngle");
    pr_kinect_getHandAngle = (GETHANDANGLE)pr_libKinect->resolve("GetHandAngle");
    qDebug()<<"Kinect Open address:"<<(int)pr_kinect_open;
}

void TmKinect::initConfig()
{
    g_log("TmKinect initConfig");
}

void TmKinect::stopRun()
{
    if( pr_libKinect && pr_bIsOpened){
        if( pr_kinect_close ){
            pr_bIsOpened = false;
            pr_kinect_close();
        }
    }
}

bool TmKinect::getIsOpened()
{
    if( !pr_libKinect ){
        return false;
    }
    return pr_bIsOpened;
}

void TmKinect::getAudioAngle(float *fAngle, float *fAngleConfidence)
{
    if( pr_libKinect && pr_bIsOpened ){
        if( pr_kinect_getAudioAngle ){
            pr_kinect_getAudioAngle(fAngle, fAngleConfidence);
        }
    }
}

bool TmKinect::getHandAngles(int nSwings[], int nExpands[], int nHeadAngles[])
{
    if( pr_libKinect && pr_bIsOpened ){
        if( pr_kinect_getHandAngle ){
            return pr_kinect_getHandAngle(nSwings, nExpands, nHeadAngles);
        }
    }
    return false;
}

QList<int> TmKinect::getBodyInfo()
{

    if( pr_libKinect  && pr_bIsOpened ){
        pr_infoList.clear();
        int nAngle[] = {0,0,0,0};
        if( pr_kinect_getHandAngle ){
            pr_kinect_getHandAngle(nAngle, nAngle+2, NULL);
        }
        float fAudioAngle[] = {0, 0};
        if( pr_kinect_getAudioAngle ){
            pr_kinect_getAudioAngle(fAudioAngle, fAudioAngle+1);
        }
        pr_infoList.append(nAngle[0]);
        pr_infoList.append(nAngle[2]);
        pr_infoList.append(nAngle[1]);
        pr_infoList.append(nAngle[3]);
        pr_infoList.append(fAudioAngle[0]*(-60));
        pr_infoList.append(fAudioAngle[1]*100);
    }
    return pr_infoList;
}

bool TmKinect::startRun()
{
    if( pr_libKinect ){
        if( pr_kinect_open ){
            qDebug()<<"try to open kinect!";
            pr_bIsOpened = pr_kinect_open();
            return pr_bIsOpened;
        }
    }
    return false;
}


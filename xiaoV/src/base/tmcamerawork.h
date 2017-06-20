#ifndef TMCAMERAWORK_H
#define TMCAMERAWORK_H

/*
 * Capture Camera and process with opencv.
 * Face check and video compress.
 * Work in thread.
 */

#include <QObject>
#include <QThread>
#include <QReadWriteLock>
#include "opencv2/opencv.hpp"
static QImage cvMat2QImage(const cv::Mat& mat);
static cv::Mat QImage2cvMat(QImage image);
class TmCameraWorkThread;

class TmCameraWork : public QObject
{
    Q_OBJECT
public:
    explicit TmCameraWork(QObject *parent = 0);
    explicit TmCameraWork(const int cameraIndex,QObject *parent = 0);

    bool getCameraSrc(cv::Mat& img);
    bool getCameraJpeg(QByteArray& bArry);
    int getCameraPersonCounts();
    void getCameraPersonCenter(int* pArray);


    void exit();
private:
    TmCameraWorkThread* pr_pThreadWork;

signals:


public slots:
    void initConfig();

};

// the work thread
class TmCameraWorkThread : public QThread
{
    Q_OBJECT
public:
    explicit TmCameraWorkThread(QObject *parent = 0);
    TmCameraWorkThread(const int cameraIndex,QObject *parent = 0 );
    bool m_bExit;
    int m_nPersonCount;
    int m_nCameraIndex;
    int m_nHeadPos[2];
    // opencv
    cv::Mat m_imgBuff;
    QReadWriteLock m_lockRW;
    cv::vector<uchar> m_jpegData;

private:
    virtual void run();

    // opencv
    cv::VideoCapture* pr_pCamera;
    cv::CascadeClassifier pr_cascade;
    bool pr_bHaveCamera;

    void recPersonCount(cv::Mat &img, bool bDebug=false);
};

#endif // TMCAMERAWORK_H

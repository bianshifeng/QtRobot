#include "tmcamerawork.h"
#include <tmglobaltool.h>
#include <QImage>


QImage cvMat2QImage(const cv::Mat& mat)
{
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if(mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        image.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row ++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if(mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if(mat.type() == CV_8UC4)
    {
        qDebug() << "CV_8UC4";
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}
cv::Mat QImage2cvMat(QImage image)
{
    cv::Mat mat;
    qDebug() << image.format();
    switch(image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, CV_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    }
    return mat;
}

TmCameraWorkThread::TmCameraWorkThread(QObject *parent)
{
    Q_UNUSED(parent);
    m_bExit = true;
    m_nPersonCount = 0;
    m_nCameraIndex =0;
    pr_bHaveCamera = true;
}

TmCameraWorkThread::TmCameraWorkThread( const int cameraIndex,QObject *parent)
{
    Q_UNUSED(parent);
    m_bExit = true;
    m_nPersonCount = 0;
    m_nCameraIndex =cameraIndex;
    pr_bHaveCamera = true;

}

void TmCameraWorkThread::run()
{
    qDebug()<<"TmCameraWorkThread :"<<currentThreadId();
    int bEnableDebug = 0;
    QSettings robotSetting(g_strExeRoot+"/settings/robot.ini", QSettings::IniFormat);
    bEnableDebug = robotSetting.value("Settings/debug",QVariant(0)).toInt();
    if( bEnableDebug ){
        cv::namedWindow("cam_debug");
    }
    m_bExit = false;
    
    // load camera config
    QJsonObject root = g_configJson.value("Camera").toObject();
    int camera_width = root.value("width").toInt();
    int camera_height = root.value("height").toInt();
    camera_width = 1280;
    camera_height = 1024;


    qDebug() << "TmCameraWorkThread : camera_height "<<camera_height;
    int camera_fps = root.value("fps").toInt();
    int camera_quality = root.value("quality").toInt();

    // open camera and set config
    pr_pCamera = new cv::VideoCapture();
    if( pr_pCamera && pr_pCamera->open(m_nCameraIndex)) {
        pr_pCamera->set(CV_CAP_PROP_FPS , camera_fps);
        pr_pCamera->set(CV_CAP_PROP_FRAME_WIDTH , camera_width);
        pr_pCamera->set(CV_CAP_PROP_FRAME_HEIGHT , camera_height);
        //m_bExit = false;
    }else{
        //m_bExit = true;
        pr_bHaveCamera = false;
        m_imgBuff = cv::imread("./data/ui/no_camera.png");
        g_log("TmCameraWorkThread Error! No Camera find!");
        return;
    }

    // load face template
    pr_cascade.load("./data/face/haarcascade_frontalface_alt2.xml");
    // start
    int nCheckCount = 0;
    while(!m_bExit){
        QCoreApplication::processEvents(QEventLoop::AllEvents, 20);
        msleep(20);


        // lock and write
        m_lockRW.lockForWrite();
        // get image
        if( pr_bHaveCamera ){
            (*pr_pCamera)>>m_imgBuff;
        }

        // compress
        if( !m_imgBuff.empty() ){
            cv::vector<int> jpegParams = cv::vector<int>(2);
            jpegParams[0] = CV_IMWRITE_JPEG_QUALITY;
            jpegParams[1] = camera_quality;//default(95) 0-100
            cv::imencode(".jpg", m_imgBuff, m_jpegData, jpegParams );
        }
        // face check
        nCheckCount += 1;
        if( nCheckCount > 20 ){
            nCheckCount = 0;
            recPersonCount(m_imgBuff, (bEnableDebug>0?true:false));
        }
        m_lockRW.unlock();

    }
    pr_pCamera->release();
    g_log("Camera exit!");
}

void TmCameraWorkThread::recPersonCount(cv::Mat &img, bool bDebug)
{
    cv::vector<cv::Rect> faces;
    cv::Mat gray, imgDebug;

    cv::cvtColor(img, gray, CV_BGR2GRAY);
    pr_cascade.detectMultiScale(gray, faces, 1.1, 2, CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
    int nCount = 0;
    int nDistance = gray.cols;
    m_nHeadPos[0] = -1;
    m_nHeadPos[1] = -1;
    cv::Rect rectDebug;
    for( cv::vector<cv::Rect>::const_iterator r = faces.begin(); r != faces.end(); r++ )
    {
        qDebug()<<++nCount<<"["<<r->x<<r->y<<r->width<<r->height<<"]";
        //cv::rectangle(gray, *r, cv::Scalar(255,0,0));
        int nDisLen = abs(gray.cols - r->x - r->width/2);
        if( nDisLen < nDistance ){
            m_nHeadPos[0] = (r->x + r->width/2)*100/gray.cols/2 - 100; // percentage
            m_nHeadPos[1] = (r->y + r->height/2)*100/gray.cols/2 - 100; // percentage
            nDistance = nDisLen;
            rectDebug = *r;
        }
    }
    if( bDebug ){
        imgDebug = img.clone();
        if( nCount > 0 )
            cv::rectangle(imgDebug, rectDebug, cv::Scalar(0,0,255));
        cv::imshow("cam_debug", imgDebug);
    }
    m_nPersonCount = nCount;
}
//---------------------------------------______________________-----//

TmCameraWork::TmCameraWork(QObject *parent) : QObject(parent)
{
    pr_pThreadWork = new TmCameraWorkThread();
    pr_pThreadWork->start();
}

TmCameraWork::TmCameraWork(const int cameraIndex, QObject *parent)
{
    pr_pThreadWork = new TmCameraWorkThread(cameraIndex);
    pr_pThreadWork->start();
}

bool TmCameraWork::getCameraSrc(cv::Mat &img)
{
    if( pr_pThreadWork->m_bExit ){
        return false;
    }
    if( pr_pThreadWork->m_lockRW.tryLockForRead(20) ){
        img = pr_pThreadWork->m_imgBuff.clone();
        pr_pThreadWork->m_lockRW.unlock();
        return true;
    }
    return false;
}

bool TmCameraWork::getCameraJpeg(QByteArray &bArry)
{
    if( pr_pThreadWork->m_lockRW.tryLockForRead(20) ){
        bArry = QByteArray((const char*)pr_pThreadWork->m_jpegData.data(), pr_pThreadWork->m_jpegData.size());
        pr_pThreadWork->m_lockRW.unlock();
        return true;
    }
    return false;
}

int TmCameraWork::getCameraPersonCounts()
{
    return pr_pThreadWork->m_nPersonCount;
}

void TmCameraWork::getCameraPersonCenter(int *pArray)
{
    if( pArray ){
        pArray[0] = pr_pThreadWork->m_nHeadPos[0];
        pArray[1] = pr_pThreadWork->m_nHeadPos[1];
    }
}

void TmCameraWork::exit()
{
    pr_pThreadWork->m_bExit = true;
    pr_pThreadWork->msleep(500);
    pr_pThreadWork->exit();
    pr_pThreadWork->deleteLater();
}

void TmCameraWork::initConfig()
{
    g_log("TmCameraWork");

}







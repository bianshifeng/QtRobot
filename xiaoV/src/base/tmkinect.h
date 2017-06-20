#ifndef TMKINECT_H
#define TMKINECT_H

#include <QObject>
#include <QLibrary>

typedef bool (*OPENKINECT)();
typedef void (*CLOSEKINECT)();
typedef void (*GETAUDIOANGLE)(float* fAngle, float* fAngleConfidence);
typedef bool (*GETHANDANGLE)(int* nSwings, int* nExpands, int* nHeadAngle);

class TmKinect : public QObject
{
    Q_OBJECT
public:
    explicit TmKinect(QObject *parent = 0);

private:
    QLibrary* pr_libKinect;
    OPENKINECT pr_kinect_open;
    CLOSEKINECT pr_kinect_close;
    GETAUDIOANGLE pr_kinect_getAudioAngle;
    GETHANDANGLE pr_kinect_getHandAngle;
    QList<int> pr_infoList;
    bool pr_bIsOpened;

public:
    void initConfig();
    bool startRun();
    void stopRun();
    bool getIsOpened();
    void getAudioAngle(float* fAngle, float* fAngleConfidence);
    bool getHandAngles(int nSwings[2], int nExpands[2], int nHeadAngles[2]);
    QList<int> getBodyInfo(); // return left hand, right hand, voice angle/confidence info
};



#endif // TMKINECT_H

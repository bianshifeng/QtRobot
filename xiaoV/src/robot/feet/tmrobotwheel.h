#ifndef TMROBOTWHEEL_H
#define TMROBOTWHEEL_H

#include <QObject>
#include "tmglobaltool.h"
#include <QMatrix>



class TmRobotWheel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(BehaviorState status READ status WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(BehaviorType wheelType READ wheelType WRITE setWheelType NOTIFY wheelTypeChanged)
    Q_ENUMS(BehaviorState)
    Q_ENUMS(BehaviorType)

public:

    enum BehaviorType{MoveForward,MoveBackward,TurnLeft,TurnRight};
    enum BehaviorState{
        Ready,
        Abort,
        WaitForMove,
        Moving,
        WaitForTurn,
        Turning,
        WaitForComplete,
        Complete,
        WaitForPause,
        Paused
    };

    explicit TmRobotWheel(QObject *parent = 0);
    ~TmRobotWheel();

    void loopDoWork();

    BehaviorState status() const;
    BehaviorType wheelType() const;


private:
    BehaviorState m_status;
    BehaviorType m_wheelType;

    void doReadyWork();
    void doMoveWork(int);
    void doTurnWork(int);
    void doPauseWork();
    void doCompleteWork();


public slots:
    void slot_wheel_stop_sig(bool);
    void slot_wheel_info_sig(int leftWheelCode,int rightWheelCode, double roll,double pitch,double yaw);
    void setStatus(BehaviorState status);
    void setWheelType(BehaviorType wheelType);


signals:
    void statusChanged(BehaviorState status);
    void wheelTypeChanged(BehaviorType wheelType);
};

#endif // TMROBOTWHEEL_H

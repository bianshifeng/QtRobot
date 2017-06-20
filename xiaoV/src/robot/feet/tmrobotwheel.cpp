#include "tmrobotwheel.h"
#include "tmglobaltool.h"


TmRobotWheel::TmRobotWheel(QObject *parent) : QObject(parent)
{



    connect(g_pTmSerial,SIGNAL(sig_tmserial_wheel_stop_normal_status(bool)),
            this,SLOT(slot_wheel_stop_sig(bool)));
    connect(g_pTmSerial,SIGNAL(sig_tmserial_cruise_info(int,int,double,double,double)),
            this,SLOT(slot_wheel_info_sig(int,int,double,double,double)));


    g_pTmSerial->motor_wheel_code_query_start();

}

TmRobotWheel::~TmRobotWheel()
{
    g_pTmSerial->motor_wheel_code_query_stop();

}

void TmRobotWheel::loopDoWork()
{
    if(m_status == Ready){
        doReadyWork();
    }else if(m_status == WaitForMove ){
        doMoveWork(100);
    }else if( m_status == WaitForTurn){
        doTurnWork(20);
    }else if( m_status == WaitForPause){
        doPauseWork();
    }else if( m_status == WaitForComplete){
        doCompleteWork();
    }

}

TmRobotWheel::BehaviorState TmRobotWheel::status() const
{
    return m_status;
}

TmRobotWheel::BehaviorType TmRobotWheel::wheelType() const
{
    return m_wheelType;
}



void TmRobotWheel::setStatus(TmRobotWheel::BehaviorState status)
{
    if (m_status == status)
        return;

    m_status = status;
    emit statusChanged(status);
}

void TmRobotWheel::setWheelType(TmRobotWheel::BehaviorType wheelType)
{
    if (m_wheelType == wheelType)
        return;

    m_wheelType = wheelType;
    emit wheelTypeChanged(wheelType);
}

void TmRobotWheel::doReadyWork()
{
    g_pTmSerial->motor_wheel_code_reset();
    setStatus(WaitForMove);
}

void TmRobotWheel::doMoveWork(int moveLength)
{
    g_pRobot->motor_wheel_move(true,moveLength,10);
    setStatus(Moving);
}

void TmRobotWheel::doTurnWork(int turnAngle)
{
    g_pRobot->motor_wheel_rotate(true,turnAngle,10);
    setStatus(Turning);
}
void TmRobotWheel::doPauseWork()
{
    g_pRobot->motor_wheel_stop(false);
}
void TmRobotWheel::doCompleteWork()
{
    g_pRobot->motor_wheel_stop(true);
}

void TmRobotWheel::slot_wheel_stop_sig(bool)
{
    //通过硬件中断信息来获取终端信号
    if(m_status == Turning){
        setStatus(Complete);
    }else if(m_status == Moving){
        setStatus(Complete);
    }else if(m_status == WaitForComplete){
        setStatus(Complete);
    }else if(m_status == WaitForPause){
        setStatus(Paused);
    }else{
        setStatus(Abort);
    }
}

void TmRobotWheel::slot_wheel_info_sig(int leftWheelCode, int rightWheelCode, double roll, double pitch, double yaw)
{
    loopDoWork();
}





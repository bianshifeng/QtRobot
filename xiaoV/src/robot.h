#ifndef ROBOT_H
#define ROBOT_H

#include <QObject>
#include "tmnaviangle.h"



class Robot : public QObject
{
    Q_OBJECT
//    Q_PROPERTY(QString strCurMode READ strCurMode WRITE setStrCurMode NOTIFY strCurModeChanged)

public:
    explicit Robot(QObject *parent = 0);
    ~Robot();

    //dlgMain
    QString m_strCurMode;  //current mode

    //from dlgvideoshow
    bool m_bIsPlaying;

    //dlgpwd
    QString m_strModuleName;

    void listening();
    void thinking();
    void answer();

    void compass_test();

public slots:
    void slot_answer_result(QString);
    void slot_load_pageEngine(QString);
    void slot_angle_info(double fYoll, double fPitch, double fYaw);



private:
    void initLogic();
    void exitapp();
    void initLogicOld();
    void loadPageJsEngine(const QString pageName);
    QThread* m_ptrPluginThread;


signals:

    //init
    void sig_init_success();
    void sig_ui_jump_name(bool);

    //compass
    void sig_compass_info(double fYoll, double fPitch, double fYaw);
    void sig_robot_read_wheel_code_info(int wheelCode,int leftWheelCode,int rightWheelCode);
    void sig_robot_wheel_stop_normal_status(bool);


    // from dlgvideoshow
    void sig_set_video_show(QString strFilePath); // Path for user dir
    void sig_set_video_stopped();
    void sig_set_video_volume(int nVolume);

    // from dlgstand
    void pic_btn_show(bool bShow);
    void pic_btn_name(QString strName);

    // from dlgyyy
    void pic_btn_show_yyy(bool bShow);
    void pic_btn_name_yyy(QString strName);

    //dlgpicshow
    void sig_pics_show(QString strFileDir);
    void sig_pics_hide();

private slots:
    void pwd_check_return(bool bIsOk);
    void loading_over();
    // ---------
    void slot_audio_result(QString result);
    void slot_tuling_result(QString result);
    void slot_touch_result(QString result);

};

#endif // ROBOT_H

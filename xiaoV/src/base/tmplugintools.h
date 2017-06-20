#ifndef TMPLUGINTOOLS_H
#define TMPLUGINTOOLS_H

#include <QObject>
#include <QUdpSocket>

class TmPluginTools : public QObject
{
    Q_OBJECT
public:
    explicit TmPluginTools(QObject *parent = 0);
    void initConfig();

signals:
    void sig_init();
    void sig_exscreen_pic_dir(QString strDirName); // Ex: ./user/data/xxx
    void sig_exscreen_pic_file(QString strFileFullName); // Ex: ./user/exscreen/xxx.png
    void sig_exscreen_video_dir(QString strDirName); // Ex: ./user/data/xxx
    void sig_exscreen_video_file(QString strFileFullName); // Ex: ./user/video/xxx.mp4

private slots:
    void slot_init();
    void slot_exscreen_pic_dir(QString strDirName);
    void slot_exscreen_pic_file(QString strFileFullName);
    void slot_exscreen_video_dir(QString strDirName);
    void slot_exscreen_video_file(QString strFileFullName);
private:
    QUdpSocket* pr_pSocketExScreen;
    int pr_nUdpPort;
};

#endif // TMPLUGINTOOLS_H

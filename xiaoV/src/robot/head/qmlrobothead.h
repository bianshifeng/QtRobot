#ifndef QMLROBOTHEAD_H
#define QMLROBOTHEAD_H
#include "tmglobaltool.h"
#include <QObject>

class QmlRobotHead : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool listenStatus READ listenStatus WRITE setListenStatus NOTIFY listenStatusChanged)
public:
    explicit QmlRobotHead(QObject *parent = 0);
    ~QmlRobotHead();
    Q_INVOKABLE void startListen();
    Q_INVOKABLE void startSay(const QString);
    Q_INVOKABLE void speak(const QString);

    bool listenStatus() const;


signals:


    void listenStatusChanged(bool listenStatus);

public slots:
    void slot_listenStatusChanged(bool listenStatus);
    void slot_tuling_result(QString);
    void slot_audio_result(QString);

void setListenStatus(bool listenStatus);


private:
    bool m_isListening;
    bool m_isSaying;
    TmMedia* m_ptrTmMedia;
    bool m_listenStatus;
};

#endif // QMLROBOTHEAD_H

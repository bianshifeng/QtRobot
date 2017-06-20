#ifndef VOICEREQUESTHANDLER2_H
#define VOICEREQUESTHANDLER2_H

#include <QObject>
#include <QThread>
#include <QTimer>

class VoiceConnectionHandler : public QThread
{
    Q_OBJECT
    Q_DISABLE_COPY(VoiceConnectionHandler)
public:
    explicit VoiceConnectionHandler(QObject *parent = 0);
    ~VoiceConnectionHandler();

signals:

public slots:
    void slot_timeout();
    void slot_startTimer(QString);

private:
    QTimer m_readTimer;

    // QThread interface
protected:
    void run();
};

#endif // VOICEREQUESTHANDLER2_H

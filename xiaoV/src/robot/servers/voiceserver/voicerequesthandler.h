#ifndef VOICEREQUESTHANDLER_H
#define VOICEREQUESTHANDLER_H

#include <QObject>

class VoiceRequestHandler : public QObject
{
    Q_OBJECT
public:
    explicit VoiceRequestHandler(QObject *parent = 0);
    ~VoiceRequestHandler();

signals:

public slots:
};

#endif // VOICEREQUESTHANDLER_H

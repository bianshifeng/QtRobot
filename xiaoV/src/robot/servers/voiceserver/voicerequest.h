#ifndef VOICEREQUEST_H
#define VOICEREQUEST_H
#include <QTcpSocket>
#include <QIODevice>

class VoiceRequest
{

public:
    VoiceRequest();
    ~VoiceRequest();

    void readFromTcpSocket(QTcpSocket *socket);
    void readFromDeviceSocket(QIODevice *socket);
private:
    bool m_nate;
};

#endif // VOICEREQUEST_H

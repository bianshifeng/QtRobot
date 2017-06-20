#include "voicerequest.h"

VoiceRequest::VoiceRequest()
{

}

VoiceRequest::~VoiceRequest()
{

}

void VoiceRequest::readFromTcpSocket(QTcpSocket *socket)
{

}

void VoiceRequest::readFromDeviceSocket(QIODevice *socket)
{
    QByteArray bArray = socket->readAll();
}


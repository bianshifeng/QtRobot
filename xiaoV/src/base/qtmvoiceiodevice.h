#ifndef QTMVOICEIODEVICE_H
#define QTMVOICEIODEVICE_H

#include <QIODevice>
#include <QByteArrayList>

class QTmVoiceIODevice : public QIODevice
{
public:
    QTmVoiceIODevice();

    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);

private:
    QByteArray m_buffer;

    QByteArrayList m_bufList;
};

#endif // QTMVOICEIODEVICE_H

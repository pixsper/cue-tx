#include "qmscudpreceiver.h"
#include <QDataStream>
#include <string.h>

QMscUdpReceiver::QMscUdpReceiver(QObject *parent) : QObject(parent)
{
    _udpSocket = new QUdpSocket(this);
}

void QMscUdpReceiver::readPendingDatagrams()
{
    while (_udpSocket->hasPendingDatagrams())
    {
        QNetworkDatagram datagram = _udpSocket->receiveDatagram();
        processDatagram(datagram);
    }
}

void QMscUdpReceiver::processDatagram(const QNetworkDatagram& datagram)
{
    if (datagram.data().length() < MSC_GMA_HEADER_LENGTH)
        return;

    QDataStream data(datagram.data());
    data.setByteOrder(QDataStream::LittleEndian);

    const int ID_LENGTH = 4;

    char headerId1[ID_LENGTH];
    data.readRawData(headerId1, ID_LENGTH);

    if (strncmp(headerId1, MSC_GMAHEADER_ID1, ID_LENGTH))
        return;

    char headerId2[ID_LENGTH];
    data.readRawData(headerId2, ID_LENGTH);

    if (strncmp(headerId2, MSC_GMAHEADER_ID2, ID_LENGTH))
        return;

    qint32 packetLength;
    data >> packetLength;

    if (packetLength != datagram.data().length())
        return;

    QByteArray mscData = datagram.data();
    mscData.remove(0, MSC_GMA_HEADER_LENGTH);

    emit packetReceived(mscData);
}

bool QMscUdpReceiver::startListening(quint16 port)
{
    connect(_udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
    return _udpSocket->bind(port, QUdpSocket::ShareAddress);
}

void QMscUdpReceiver::stopListening()
{
    _udpSocket->disconnectFromHost();
   disconnect(_udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
}

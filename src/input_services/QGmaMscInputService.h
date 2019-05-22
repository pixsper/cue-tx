#pragma once

#include "QCueTxInputService.h"
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QDataStream>

class QGmaMscInputService : public QCueTxInputService
{
    Q_OBJECT

    const int MSC_GMA_HEADER_LENGTH = 12;
    const char* MSC_GMAHEADER_ID1 = "GMA";
    const char* MSC_GMAHEADER_ID2 = "MSC";

    const char* SETTINGS_PORT_KEY = "port";
    const quint16 SETTINGS_PORT_DEFAULT = 6004;

    QUdpSocket* _udpSocket;


public:
    explicit QGmaMscInputService(QObject *parent = nullptr)
        : QCueTxInputService(parent),
          _udpSocket(new QUdpSocket(this))
    {

    }

    bool start(const QVariantMap& settings) override
    {
        quint16 port = SETTINGS_PORT_DEFAULT;

        auto it = settings.find(SETTINGS_PORT_KEY);

        if (it != settings.end())
            port = static_cast<quint16>(it.value().toInt());

        connect(_udpSocket, &QUdpSocket::readyRead, this, &QGmaMscInputService::readPendingDatagrams);
        return _udpSocket->bind(port, QUdpSocket::ShareAddress);
    }

    void stop() override
    {
        _udpSocket->disconnectFromHost();
       disconnect(_udpSocket, &QUdpSocket::readyRead, this, &QGmaMscInputService::readPendingDatagrams);
    }

    void processDatagram(const QNetworkDatagram& datagram)
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

        MscMessage message;
        if (MscMessage::FromByteArray(mscData, message))
            emit messageReceived(message);
    }

private slots:
    void readPendingDatagrams()
    {
        while (_udpSocket->hasPendingDatagrams())
        {
            QNetworkDatagram datagram = _udpSocket->receiveDatagram();
            processDatagram(datagram);
        }
    }

};

#pragma once

#include <QObject>
#include <QUdpSocket>
#include <QNetworkDatagram>

class QMscUdpReceiver : public QObject
{
    const int MSC_GMA_HEADER_LENGTH = 12;
    const char* MSC_GMAHEADER_ID1 = "GMA";
    const char* MSC_GMAHEADER_ID2 = "MSC";

    QUdpSocket* _udpSocket;


    void processDatagram(const QNetworkDatagram& datagram);

    Q_OBJECT
public:
    explicit QMscUdpReceiver(QObject *parent = nullptr);

    bool startListening(quint16 port);
    void stopListening();

signals:
    void packetReceived(const QByteArray& data);

private slots:
    void readPendingDatagrams();
};

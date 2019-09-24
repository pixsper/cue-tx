#pragma once

#include <QObject>
#include <QDataStream>
#include <QtEndian>
#include "oscpp/server.hpp"

class QOscTcpDecoder : public QObject
{
    Q_OBJECT

    const int BUFFER_RESERVE_SIZE = 4098;

    QByteArray _buffer;

public:
    explicit QOscTcpDecoder(bool isSlip, QObject *parent = nullptr)
        : QObject(parent)
    {
        _buffer.reserve(BUFFER_RESERVE_SIZE);
    }

    void addData(const QByteArray& data)
    {
        _buffer.append(data);
        processBuffer();
    }

    void processBuffer()
    {
        if (_buffer.size() < 4)
            return;

        qint32 length = qFromBigEndian<qint32>(_buffer.data());

        if (_buffer.size() < length + 4)
            return;

        OSCPP::Server::Packet packet(_buffer.data() + 4, length);
        emit packetRead(packet);

        _buffer.remove(0, length + 4);

        processBuffer();
    }

signals:
    void packetRead(const OSCPP::Server::Packet& packet);
};


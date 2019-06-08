// This file is part of CueTX.
//
// CueTX is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation, either version 3 of
// the License, or (at your option) any later version.
//
// CueTX is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with CueTX.  If not, see <https://www.gnu.org/licenses/>.

#pragma once

#include "QCueTxInputService.hpp"
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QDataStream>
#include <QRegularExpression>
#include "oscpp/server.hpp"

class QEosOscInputService : public QCueTxInputService
{
    Q_OBJECT

    const char* SETTINGS_PORT_KEY = "port";
    const quint16 SETTINGS_PORT_DEFAULT = 8001;


    QUdpSocket* _udpSocket;
    QRegularExpression _addressRegex;

public:
    explicit QEosOscInputService(QObject *parent = nullptr)
        : QCueTxInputService(parent),
          _udpSocket(new QUdpSocket(this)),
          _addressRegex(R"(\/eos\/out\/event\/cue\/(\d*\.?\d+)\/(\d*\.?\d+)\/(fire|stop))")
    {
        _addressRegex.optimize();
    }

    bool start(const QVariantMap& settings) override
    {
        quint16 port = SETTINGS_PORT_DEFAULT;

        const auto it = settings.find(SETTINGS_PORT_KEY);

        if (it != settings.end())
            port = static_cast<quint16>(it.value().toInt());

        connect(_udpSocket, &QUdpSocket::readyRead, this, &QEosOscInputService::readPendingDatagrams);
        return _udpSocket->bind(port, QUdpSocket::ShareAddress);
    }

    void stop() override
    {
        _udpSocket->disconnectFromHost();
       disconnect(_udpSocket, &QUdpSocket::readyRead, this, &QEosOscInputService::readPendingDatagrams);
    }

    void processPacket(const OSCPP::Server::Packet& packet)
    {
        const OSCPP::Server::Message message(packet);

        const auto match = _addressRegex.match(message.address());

        if (!match.isValid())
            return;

        if (match.capturedTexts()[3] == "fire")
        {

        }
        else if (match.capturedTexts()[3] == "stop")
        {

        }
    }

    void processDatagram(const QNetworkDatagram& datagram)
    {
	    const OSCPP::Server::Packet packet(datagram.data().data(), static_cast<size_t>(datagram.data().size()));

        if (packet.isBundle())
        {
	        const OSCPP::Server::Bundle bundle(packet);
            OSCPP::Server::PacketStream packets(bundle.packets());

            while (!packets.atEnd())
                processPacket(packets.next());
        }
        else
        {
            processPacket(packet);
        }
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

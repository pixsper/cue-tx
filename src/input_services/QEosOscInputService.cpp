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

#include "QEosOscInputService.hpp"

const QString QEosOscInputService::SETTINGS_MODE_KEY = "in_eos_mode";
const QEosOscInputService::ReceiveMode QEosOscInputService::SETTINGS_MODE_DEFAULT;

const QString QEosOscInputService::SETTINGS_HOSTIP_KEY = "in_eos_host_port";
const QString QEosOscInputService::SETTINGS_HOSTIP_DEFAULT = "127.0.0.1";

const QString QEosOscInputService::SETTINGS_HOSTPORT_KEY = "in_eos_host_ip";
const quint16 QEosOscInputService::SETTINGS_HOSTPORT_MIN;
const quint16 QEosOscInputService::SETTINGS_HOSTPORT_MAX;
const quint16 QEosOscInputService::SETTINGS_HOSTPORT_DEFAULT;


QVariantMap QEosOscInputService::staticDefaultSettings()
{
    return QVariantMap
    {
        { SETTINGS_MODE_KEY, static_cast<int>(SETTINGS_MODE_DEFAULT) },
        { SETTINGS_HOSTIP_KEY, SETTINGS_HOSTIP_DEFAULT },
        { SETTINGS_HOSTPORT_KEY, SETTINGS_HOSTPORT_DEFAULT }
    };
}

QEosOscInputService::QEosOscInputService(QObject* parent)
    : QCueTxInputService(parent),
      _tcpSocket(new QTcpSocket(this)),
      _udpSocket(new QUdpSocket(this)),
      _addressRegex(R"(\/eos\/out\/event\/cue\/(\d*\.?\d+)\/(\d*\.?\d+)\/(fire|stop))")
{
    _addressRegex.optimize();
}

bool QEosOscInputService::start(const QVariantMap& settings)
{
    _mode = SETTINGS_MODE_DEFAULT;

    {
        const auto it = settings.find(SETTINGS_MODE_KEY);
        if (it != settings.end())
            _mode = (ReceiveMode)it.value().toInt();
    }

    _hostIp.setAddress(SETTINGS_HOSTIP_DEFAULT);

    {
        const auto it = settings.find(SETTINGS_HOSTIP_KEY);
        if (it != settings.end())
        {
            if (!_hostIp.setAddress(it.value().toString()))
                _hostIp.setAddress(SETTINGS_HOSTIP_DEFAULT);
        }
    }


    _hostPort = SETTINGS_HOSTPORT_DEFAULT;

    {
        const auto it = settings.find(SETTINGS_HOSTPORT_KEY);
        if (it != settings.end())
            _hostPort = static_cast<quint16>(it.value().toInt());
    }



    switch(_mode)
    {
        case ReceiveMode::TcpPacketLengthHeaders:

            _tcpDecoder = new QOscTcpDecoder(false, this);
            connect(_tcpDecoder, &QOscTcpDecoder::packetRead, this, &QEosOscInputService::processPacket);

            connect(_tcpSocket, &QIODevice::readyRead, this, &QEosOscInputService::readNetworkData);
            connect(_tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &QEosOscInputService::handleTcpError);

            _tcpSocket->connectToHost(_hostIp, EOS_TCP_PORT);

            return true;

        case ReceiveMode::TcpSlip:

             _tcpDecoder = new QOscTcpDecoder(true, this);
             connect(_tcpDecoder, &QOscTcpDecoder::packetRead, this, &QEosOscInputService::processPacket);

            connect(_tcpSocket, &QIODevice::readyRead, this, &QEosOscInputService::readNetworkData);
            connect(_tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &QEosOscInputService::handleTcpError);

            _tcpSocket->connectToHost(_hostIp, EOS_TCP_PORT);
            return true;

        case ReceiveMode::Udp:

            connect(_udpSocket, &QIODevice::readyRead, this, &QEosOscInputService::readNetworkData);
            return _udpSocket->bind(_hostPort, QUdpSocket::ShareAddress);

        default:
            Q_ASSERT(0); // Unknown receive mode
            return false;
    }
}

void QEosOscInputService::stop()
{
    switch(_mode)
    {
        case ReceiveMode::TcpPacketLengthHeaders:
        case ReceiveMode::TcpSlip:

            _tcpSocket->disconnectFromHost();
            disconnect(_tcpSocket, &QIODevice::readyRead, this, &QEosOscInputService::readNetworkData);
            disconnect(_tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &QEosOscInputService::handleTcpError);

            break;

        case ReceiveMode::Udp:

            _udpSocket->disconnectFromHost();
            disconnect(_udpSocket, &QUdpSocket::readyRead, this, &QEosOscInputService::readNetworkData);

            break;
    }
}

void QEosOscInputService::handleTcpError(QAbstractSocket::SocketError socketError)
{
    switch (socketError)
    {
        case QAbstractSocket::RemoteHostClosedError:
        case QAbstractSocket::HostNotFoundError:
        case QAbstractSocket::ConnectionRefusedError:
            _tcpSocket->connectToHost(_hostIp, EOS_TCP_PORT);
            break;
        default:
            break;
    }
}

void QEosOscInputService::processPacket(const OSCPP::Server::Packet& packet)
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

void QEosOscInputService::processDatagram(const QNetworkDatagram& datagram)
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

void QEosOscInputService::readNetworkData()
{
    switch(_mode)
    {
        case QEosOscInputService::ReceiveMode::TcpPacketLengthHeaders:
        case QEosOscInputService::ReceiveMode::TcpSlip:

            _tcpDecoder->addData(_tcpSocket->readAll());

            break;

        case QEosOscInputService::ReceiveMode::Udp:

            while (_udpSocket->hasPendingDatagrams())
                processDatagram(_udpSocket->receiveDatagram());

            break;
    }
}

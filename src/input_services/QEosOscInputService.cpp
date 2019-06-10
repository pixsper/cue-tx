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
const QString QEosOscInputService::SETTINGS_HOSTUDPPORT_KEY = "in_eos_host_udp_port";
const quint16 QEosOscInputService::SETTINGS_HOSTUDPPORT_MIN;
const quint16 QEosOscInputService::SETTINGS_HOSTUDPPORT_MAX;
const quint16 QEosOscInputService::SETTINGS_HOSTUDPPORT_DEFAULT;


QVariantMap QEosOscInputService::staticDefaultSettings()
{
    return QVariantMap
    {
        { SETTINGS_MODE_KEY, static_cast<int>(SETTINGS_MODE_DEFAULT) },
        { SETTINGS_HOSTUDPPORT_KEY, SETTINGS_HOSTUDPPORT_DEFAULT }
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
    quint16 port = SETTINGS_HOSTUDPPORT_DEFAULT;

    const auto it = settings.find(SETTINGS_HOSTUDPPORT_KEY);

    if (it != settings.end())
        port = static_cast<quint16>(it.value().toInt());

    switch(_mode)
    {
        case ReceiveMode::TcpPacketLengthHeaders:
        case ReceiveMode::TcpSlip:

            connect(_tcpSocket, &QIODevice::readyRead, this, &QEosOscInputService::readPendingDatagrams);
            connect(_tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &QEosOscInputService::handleTcpError);
            return _tcpSocket->bind(port, QUdpSocket::ShareAddress);

        case ReceiveMode::Udp:

            connect(_udpSocket, &QIODevice::readyRead, this, &QEosOscInputService::readPendingDatagrams);
            return _udpSocket->bind(port, QUdpSocket::ShareAddress);

        default:
            Q_ASSERT(0); // Unknown receive mode
            return false;
    }
}

void QEosOscInputService::stop()
{
    _udpSocket->disconnectFromHost();
    disconnect(_udpSocket, &QUdpSocket::readyRead, this, &QEosOscInputService::readPendingDatagrams);
}

void QEosOscInputService::handleTcpError(QAbstractSocket::SocketError socketError)
{
    switch (socketError)
    {
        case QAbstractSocket::RemoteHostClosedError:
            break;
        case QAbstractSocket::HostNotFoundError:
            //QMessageBox::information(this, tr("Fortune Client"), tr("The host was not found. Please check the host name and port settings."));
            break;
        case QAbstractSocket::ConnectionRefusedError:
            //QMessageBox::information(this, tr("Fortune Client"), tr("The connection was refused by the peer. Make sure the fortune server is running, and check that the host name and port settings are correct."));
            break;
        default:
           // QMessageBox::information(this, tr("Fortune Client"), tr("The following error occurred: %1.").arg(tcpSocket->errorString()));
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



void QEosOscInputService::readPendingDatagrams()
{
    while (_udpSocket->hasPendingDatagrams())
    {
        QNetworkDatagram datagram = _udpSocket->receiveDatagram();
        processDatagram(datagram);
    }
}

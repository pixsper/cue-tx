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
#include <QTcpSocket>
#include <QNetworkDatagram>
#include <QDataStream>
#include <QRegularExpression>
#include "../QOscTcpDecoder.hpp"
#include "oscpp/server.hpp"

class QEosOscInputService : public QCueTxInputService
{
public:
    enum class ReceiveMode
    {
        TcpPacketLengthHeaders = 0,
        TcpSlip = 1,
        Udp = 2
    };

private:
    Q_OBJECT

    static const quint16 EOS_TCP_PORT = 3032;

    QTcpSocket* _tcpSocket;
    QUdpSocket* _udpSocket;

    QOscTcpDecoder* _tcpDecoder;

    ReceiveMode _mode;
    QHostAddress _hostIp;
    quint16 _hostPort;

    QRegularExpression _addressRegex;

public:
    static const QString SETTINGS_MODE_KEY;
    static const ReceiveMode SETTINGS_MODE_DEFAULT = ReceiveMode::TcpPacketLengthHeaders;
    static const QString SETTINGS_HOSTIP_KEY;
    static const QString SETTINGS_HOSTIP_DEFAULT;
    static const QString SETTINGS_HOSTPORT_KEY;
    static const quint16 SETTINGS_HOSTPORT_MIN = 1;
    static const quint16 SETTINGS_HOSTPORT_MAX = 65535;
    static const quint16 SETTINGS_HOSTPORT_DEFAULT = 8001;


    static QVariantMap staticDefaultSettings();


    explicit QEosOscInputService(QObject *parent = nullptr);

    bool start(const QVariantMap& settings) override;

    void stop() override;

    QVariantMap defaultSettings() const override { return staticDefaultSettings(); }

private:
    void handleTcpError(QAbstractSocket::SocketError socketError);

    void processDatagram(const QNetworkDatagram& datagram);

private slots:
    void processPacket(const OSCPP::Server::Packet& packet);

    void readNetworkData();

};

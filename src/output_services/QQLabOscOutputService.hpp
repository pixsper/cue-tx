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

#include <QHash>
#include <QUdpSocket>
#include "QCueTxOutputService.hpp"
#include "oscpp/client.hpp"

class QQLabOscOutputService : public QCueTxOutputService
{
    Q_OBJECT

    const quint16 QLAB_UDP_PORT = 53000;
    const int UDP_BUFFER_SIZE = 128;

    const char* SETTINGS_HOSTIP_NAME = "host_ip";
    const char* SETTINGS_HOSTIP_DEFAULT = "127.0.0.1";

    QHostAddress _hostIp;

    QUdpSocket* _udpSocket;

public:
    explicit QQLabOscOutputService(QObject *parent = nullptr)
        : QCueTxOutputService(parent),
          _udpSocket(new QUdpSocket(this))
    {
        _udpSocket->setSocketOption(QAbstractSocket::SocketOption::MulticastTtlOption, true);
    }

    virtual bool start(const QVariantMap& settings) override
    {
	    const auto it = settings.find(SETTINGS_HOSTIP_NAME);

        if (it != settings.end())
            _hostIp = QHostAddress(it.value().toString());
        else
            _hostIp = QHostAddress(SETTINGS_HOSTIP_DEFAULT);

        return _hostIp.isNull();
    }

    virtual void stop() override
    {
        _hostIp.clear();
    }

public slots:
    void sendMessage(const MscMessage& message) override
    {
        if (_hostIp.isNull())
            return;

        QByteArray buffer(UDP_BUFFER_SIZE, 0);
        OSCPP::Client::Packet packet(buffer.data(), static_cast<size_t>(buffer.size()));

        switch(message._commandType)
        {
            case MscCommandType::AllOff:
                packet.openMessage("/hardStop", 0).closeMessage();
                break;

            case MscCommandType::StandbyPlus:
                packet.openMessage("/playhead/next", 0).closeMessage();
                break;

            case MscCommandType::StandbyMinus:
                packet.openMessage("/playhead/previous", 0).closeMessage();
                break;

            case MscCommandType::SequencePlus:
                packet.openMessage("/playhead/nextSequence", 0).closeMessage();
                break;

            case MscCommandType::SequenceMinus:
                packet.openMessage("/playhead/previousSequence", 0).closeMessage();
                break;

            case MscCommandType::Reset:
                if (message._cueNumber == "")
                {
                    packet.openMessage("/reset", 0).closeMessage();
                }
                else
                {
                    packet.openMessage("/reset", 1)
                        .string(reinterpret_cast<const char*>(message._cueNumber.toString().toStdString().c_str())).closeMessage();
                }
                break;

            case MscCommandType::Go:

                if (message._cueNumber == "")
                {
                    packet.openMessage("/go", 0);
                }
                else
                {
                    packet.openMessage("/go", 1)
                        .string(reinterpret_cast<const char*>(message._cueNumber.toString().toStdString().c_str())).closeMessage();
                }
                break;

            case MscCommandType::Stop:
                if (message._cueNumber == "")
                {
                    packet.openMessage("/pause", 0);
                }
                else
                {
                    packet.openMessage("/pause", 1)
                        .string(reinterpret_cast<const char*>(message._cueNumber.toString().toStdString().c_str())).closeMessage();
                }
                break;

            case MscCommandType::Resume:
                if (message._cueNumber == "")
                {
                    packet.openMessage("/resume", 0);
                }
                else
                {
                    packet.openMessage("/resume", 1)
                        .string(reinterpret_cast<const char*>(message._cueNumber.toString().toStdString().c_str())).closeMessage();
                }
                break;

            case MscCommandType::Load:
                if (message._cueNumber != "")
                {
                    packet.openMessage("/load", 1)
                        .string(reinterpret_cast<const char*>(message._cueNumber.toString().toStdString().c_str())).closeMessage();
                }
                break;

            default:
                return;
        }

        buffer.resize(static_cast<int>(packet.size()));

        _udpSocket->writeDatagram(buffer, _hostIp, QLAB_UDP_PORT);
    }
};

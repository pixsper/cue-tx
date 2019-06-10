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

#include "QDisguiseOscOutputService.hpp"

const QString QDisguiseOscOutputService::SETTINGS_HOSTIP_KEY = "out_disguise_host_ip";
const QString QDisguiseOscOutputService::SETTINGS_HOSTIP_DEFAULT = "127.0.0.1";
const QString QDisguiseOscOutputService::SETTINGS_HOSTPORT_KEY = "out_disguise_host_port";

QDisguiseOscOutputService::QDisguiseOscOutputService(QObject* parent)
    : QCueTxOutputService(parent),
      _hostIp(SETTINGS_HOSTIP_DEFAULT),
      _hostPort(SETTINGS_HOSTPORT_DEFAULT),
      _udpSocket(new QUdpSocket(this))
{
}

bool QDisguiseOscOutputService::start(const QVariantMap& settings)
{
    const auto itIp = settings.find(SETTINGS_HOSTIP_KEY);

    if (itIp != settings.end())
        _hostIp = QHostAddress(itIp.value().toString());
    else
        _hostIp = QHostAddress(SETTINGS_HOSTIP_DEFAULT);

    const auto itPort = settings.find(SETTINGS_HOSTPORT_KEY);

    if (itPort != settings.end())
        _hostPort = static_cast<quint16>(itPort.value().toInt());
    else
        _hostPort = SETTINGS_HOSTPORT_DEFAULT;

    return _hostIp.isNull();
}

void QDisguiseOscOutputService::stop()
{
    _hostIp.clear();
}

void QDisguiseOscOutputService::sendMessage(const MscMessage& message)
{
    if (_hostIp.isNull())
        return;

    QByteArray buffer(UDP_BUFFER_SIZE, 0);
    OSCPP::Client::Packet packet(buffer.data(), static_cast<size_t>(buffer.size()));

    switch (message._commandType)
    {
        case MscCommandType::Go:

            if (message._cueNumber == "")
            {
                packet.openMessage("/d3/showcontrol/nextsection", 0).closeMessage();
            }
            else
            {
                packet.openMessage("/d3/showcontrol/cue", 1)
                        .string(message._cueNumber.toString().toStdString().c_str()).closeMessage();
            }
            break;

        case MscCommandType::Stop:
            if (message._cueNumber != "")
                packet.openMessage("/d3/showcontrol/stop", 0).closeMessage();
            break;

        case MscCommandType::Reset:
            packet.openMessage("/d3/showcontrol/returntostart", 0).closeMessage();
            break;

        case MscCommandType::OpenCueList:
            packet.openMessage("d3/showcontrol/trackid", 1)
                    .string(message._cueList.toString().toStdString().c_str()).closeMessage();
            break;

        default:
            return;
    }

    buffer.resize(static_cast<int>(packet.size()));

    _udpSocket->writeDatagram(buffer, _hostIp, _hostPort);
}

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

#include "QGmaMscOutputService.hpp"

const QString QGmaMscOutputService::SETTINGS_HOSTIP_KEY = "out_gma_host_ip";
const QString QGmaMscOutputService::SETTINGS_HOSTIP_DEFAULT = "127.0.0.1";

const QString QGmaMscOutputService::SETTINGS_HOSTPORT_KEY = "out_gma_host_port";

QVariantMap QGmaMscOutputService::staticDefaultSettings()
{
    return QVariantMap
    {
        { SETTINGS_HOSTIP_KEY, SETTINGS_HOSTIP_DEFAULT },
        { SETTINGS_HOSTPORT_KEY, SETTINGS_HOSTPORT_DEFAULT }
    };
}

QGmaMscOutputService::QGmaMscOutputService(QObject* parent)
    : QCueTxOutputService(parent),
      _hostPort(SETTINGS_HOSTPORT_DEFAULT),
      _udpSocket(new QUdpSocket(this))
{
}

bool QGmaMscOutputService::start(const QVariantMap& settings)
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

void QGmaMscOutputService::stop()
{
    _hostIp.clear();
}

void QGmaMscOutputService::sendMessage(const MscMessage& message)
{
    if (_hostIp.isNull())
        return;


}

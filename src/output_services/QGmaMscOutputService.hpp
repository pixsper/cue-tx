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

class QGmaMscOutputService : public QCueTxOutputService
{
    Q_OBJECT

    const char* SETTINGS_HOSTIP_NAME = "host_ip";
    const char* SETTINGS_HOSTIP_DEFAULT = "127.0.0.1";

    const char* SETTINGS_HOSTPORT_NAME = "host_port";
    const quint16 SETTINGS_HOSTPORT_DEFAULT = 6004;


    QHostAddress _hostIp;
    quint16 _hostPort;

    QUdpSocket* _udpSocket;


public:
    explicit QGmaMscOutputService(QObject *parent = nullptr)
	    : QCueTxOutputService(parent), 
			_hostPort(SETTINGS_HOSTPORT_DEFAULT),
			_udpSocket(new QUdpSocket(this))
    {
    }

    bool start(const QVariantMap& settings) override
    {
	    const auto itIp = settings.find(SETTINGS_HOSTIP_NAME);

        if (itIp != settings.end())
            _hostIp = QHostAddress(itIp.value().toString());
        else
            _hostIp = QHostAddress(SETTINGS_HOSTIP_DEFAULT);

	    const auto itPort = settings.find(SETTINGS_HOSTPORT_NAME);

        if (itPort != settings.end())
            _hostPort = static_cast<quint16>(itPort.value().toInt());
        else
            _hostPort = SETTINGS_HOSTPORT_DEFAULT;

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


    }
};

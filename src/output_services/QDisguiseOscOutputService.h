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
#include "QCueTxOutputService.h"
#include "oscpp/client.hpp"

class QDisguiseOscOutputService : public QCueTxOutputService
{
Q_OBJECT

	const int UDP_BUFFER_SIZE = 128;

	const char* SETTINGS_HOSTIP_NAME = "host_ip";
	const char* SETTINGS_HOSTIP_DEFAULT = "127.0.0.1";

	const char* SETTINGS_HOSTPORT_NAME = "host_port";
	const quint16 SETTINGS_HOSTPORT_DEFAULT = 7401;


	QHostAddress _hostIp;
	quint16 _hostPort;

	QUdpSocket* _udpSocket;


public:
	explicit QDisguiseOscOutputService(QObject* parent = nullptr)
		: QCueTxOutputService(parent),
		  _hostIp(SETTINGS_HOSTIP_DEFAULT),
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

	void stop() override
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
};

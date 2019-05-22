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


protected:
    explicit QDisguiseOscOutputService(QObject *parent = nullptr)
        : QCueTxOutputService(parent),
          _udpSocket(new QUdpSocket(this))
    {

    }

public:
    virtual bool start(const QVariantMap& settings) override
    {
        auto itIp = settings.find(SETTINGS_HOSTIP_NAME);

        if (itIp != settings.end())
            _hostIp = QHostAddress(itIp.value().toString());
        else
            _hostIp = QHostAddress(SETTINGS_HOSTIP_DEFAULT);

        auto itPort = settings.find(SETTINGS_HOSTPORT_NAME);

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

        QByteArray buffer(UDP_BUFFER_SIZE, 0);
        OSCPP::Client::Packet packet(buffer.data(), static_cast<size_t>(buffer.size()));

        switch(message._commandType)
        {
            case MscCommandType::Go:

                if (message._cueNumber == "")
                {
                    packet.openMessage("/d3/showcontrol/nextsection", 0).closeMessage();
                }
                else
                {
                    packet.openMessage("/d3/showcontrol/cue", 1)
                        .string(reinterpret_cast<const char*>(message._cueNumber.data())).closeMessage();
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
                        .string(reinterpret_cast<const char*>(message._cueList.data())).closeMessage();
                break;

            default:
                return;
        }

        buffer.resize(static_cast<int>(packet.size()));

        _udpSocket->writeDatagram(buffer, _hostIp, _hostPort);
    }
};

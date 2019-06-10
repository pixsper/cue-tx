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


    QHostAddress _hostIp;

    QUdpSocket* _udpSocket;

public:
    static const QString SETTINGS_HOSTIP_KEY;
    static const QString SETTINGS_HOSTIP_DEFAULT;

    static QVariantMap staticDefaultSettings()
    {
        return QVariantMap
        {
            { SETTINGS_HOSTIP_KEY, SETTINGS_HOSTIP_DEFAULT }
        };
    }


    explicit QQLabOscOutputService(QObject *parent = nullptr);

    virtual bool start(const QVariantMap& settings) override;

    virtual void stop() override;

    QVariantMap defaultSettings() const override { return staticDefaultSettings(); }

public slots:
    void sendMessage(const MscMessage& message) override;
};

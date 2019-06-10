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


    QHostAddress _hostIp;
    quint16 _hostPort;

    QUdpSocket* _udpSocket;


public:
    static const QString SETTINGS_HOSTIP_KEY;
    static const QString SETTINGS_HOSTIP_DEFAULT;

    static const QString SETTINGS_HOSTPORT_KEY;
    static const quint16 SETTINGS_HOSTPORT_DEFAULT = 6004;

    static QVariantMap staticDefaultSettings();


    explicit QGmaMscOutputService(QObject *parent = nullptr);

    bool start(const QVariantMap& settings) override;

    virtual void stop() override;

    QVariantMap defaultSettings() const override { return staticDefaultSettings(); }

public slots:
    void sendMessage(const MscMessage& message) override;
};

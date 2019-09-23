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
#include <QHostAddress>
#include "QCueTxOutputService.hpp"

class QGmaMscOutputService : public QCueTxOutputService
{
    Q_OBJECT

    const int MSC_GMA_HEADER_LENGTH = 12;
    const char* MSC_GMAHEADER_ID1 = "GMA";
    const char* MSC_GMAHEADER_ID2 = "MSC";

    QUdpSocket* _udpSocket;

    quint16 _udpPort;
    bool _isAddZeroPadding;

public:
    static const QString SETTINGS_HOSTPORT_KEY;
    static const quint16 SETTINGS_HOSTPORT_DEFAULT = 6004;

    static const quint16 SETTINGS_HOSTPORT_MIN = 6000;
    static const quint16 SETTINGS_HOSTPORT_MAX = 6100;

    static const QString SETTINGS_ADDZEROPADDING_KEY;
    static const bool SETTINGS_ADDZEROPADDING_DEFAULT = true;

    static QVariantMap staticDefaultSettings();

    explicit QGmaMscOutputService(QObject *parent = nullptr);

    bool start(const QVariantMap& settings) override;

    virtual void stop() override;

    QVariantMap defaultSettings() const override { return staticDefaultSettings(); }

public slots:
    void sendMessage(const MscMessage& message) override;
};

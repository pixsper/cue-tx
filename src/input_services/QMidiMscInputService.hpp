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
#include "../qrtmidi/QRtMidiIn.hpp"
#include <QVariant>

class QMidiMscInputService : public QCueTxInputService
{
    Q_OBJECT

    const QString RTMIDI_PORT_NAME = "Cue TX Input";


    QRtMidiIn* _midiIn;


public:
    static const QString SETTINGS_PORTNAME_KEY;
    static const QString SETTINGS_PORTNAME_DEFAULT;
    static const QString SETTINGS_ISVIRTUAL_KEY;
#ifdef Q_OS_MAC
    static const bool SETTINGS_ISVIRTUAL_DEFAULT = true;
#else
    static const bool SETTINGS_ISVIRTUAL_DEFAULT = false;
#endif

    static QVariantMap staticDefaultSettings();

    explicit QMidiMscInputService(QObject* parent = nullptr);

    bool start(const QVariantMap& settings) override;

    void stop() override;

    QVariantMap defaultSettings() const override { return staticDefaultSettings(); }

private slots:
    void processMessage(double timestamp, const QByteArray& message);

};

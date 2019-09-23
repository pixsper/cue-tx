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

#include <QVariant>
#include "QCueTxOutputService.hpp"
#include "../qrtmidi/QRtMidiOut.hpp"

class QMidiMscOutputService : public QCueTxOutputService
{
    Q_OBJECT

    QRtMidiOut* _midiOut;

public:
    static const QString VIRTUAL_MIDI_PORT_NAME;
    static const QString SETTINGS_PORTNAME_KEY;
    static const QString SETTINGS_PORTNAME_DEFAULT;
    static const QString SETTINGS_ISVIRTUAL_KEY;
#ifdef Q_OS_MAC
    static const bool SETTINGS_ISVIRTUAL_DEFAULT = true;
#else
    static const bool SETTINGS_ISVIRTUAL_DEFAULT = false;
#endif

    static QVariantMap staticDefaultSettings();


    explicit QMidiMscOutputService(QObject *parent = nullptr);

    bool start(const QVariantMap& settings) override;

    void stop() override;

    QVariantMap defaultSettings() const override { return staticDefaultSettings(); }

public slots:
    void sendMessage(const MscMessage& message) override;
};

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
#include "../qmidi/QRtMidiOut.hpp"

class QMidiMscOutputService : public QCueTxOutputService
{
    Q_OBJECT

    QRtMidiOut* _midiOut;

    const char* SETTINGS_PORT_NAME = "port";
    const char* SETTINGS_PORT_DEFAULT = "";

    const char* SETTINGS_ISVIRTUAL_NAME = "is_virtual";
#ifdef Q_OS_MAC
    const bool SETTINGS_ISVIRTUAL_DEFAULT = true;
#else
    const bool SETTINGS_ISVIRTUAL_DEFAULT = false;
#endif

    const char* RTMIDI_PORT_NAME = "Cue TX Output";


public:
    explicit QMidiMscOutputService(QObject *parent = nullptr)
        : QCueTxOutputService(parent),
          _midiOut(new QRtMidiOut(this, RtMidi::Api::UNSPECIFIED, "Cue TX MIDI Output Client"))
    {

    }

    bool start(const QVariantMap& settings) override
    {
#ifdef Q_OS_MAC
        const auto itVPort = settings.find(SETTINGS_ISVIRTUAL_NAME);

        if ((itVPort != settings.end() && itVPort.value().toBool()) || SETTINGS_ISVIRTUAL_DEFAULT)
        {
            _midiOut->openVirtualPort(RTMIDI_PORT_NAME);
            return _midiOut->isPortOpen();
        }

#endif
	    const auto itPort = settings.find(SETTINGS_PORT_NAME);
        if (itPort != settings.end())
        {
	        const auto ports = QRtMidiOut::getMidiOutPorts();

            for(const auto& pair : ports.toStdMap())
            {
                if (pair.second == itPort.value().toString())
                {
                    _midiOut->openPort(pair.first, RTMIDI_PORT_NAME);
                    return _midiOut->isPortOpen();
                }
            }
        }

        // Just open the first port we can find, if any
        _midiOut->openPort(0, RTMIDI_PORT_NAME);
        return _midiOut->isPortOpen();
    }

    void stop() override
    {
        _midiOut->closePort();
    }

public slots:
    void sendMessage(const MscMessage& message) override
    {

    }
};

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

#include "QMidiMscOutputService.hpp"

const QString QMidiMscOutputService::SETTINGS_PORTNAME_KEY = "out_midi_port";
const QString QMidiMscOutputService::SETTINGS_PORTNAME_DEFAULT = "";
const QString QMidiMscOutputService::SETTINGS_ISVIRTUAL_KEY = "out_midi_is_virtual";
const bool QMidiMscOutputService::SETTINGS_ISVIRTUAL_DEFAULT;

QVariantMap QMidiMscOutputService::staticDefaultSettings()
{
    return QVariantMap
    {
        { SETTINGS_PORTNAME_KEY, SETTINGS_PORTNAME_DEFAULT },
        { SETTINGS_ISVIRTUAL_KEY, SETTINGS_ISVIRTUAL_DEFAULT }
    };
}

QMidiMscOutputService::QMidiMscOutputService(QObject* parent)
    : QCueTxOutputService(parent),
      _midiOut(new QRtMidiOut(this, RtMidi::Api::UNSPECIFIED, "Cue TX MIDI Output Client"))
{

}

bool QMidiMscOutputService::start(const QVariantMap& settings)
{
#ifdef Q_OS_MAC
    const auto itVPort = settings.find(SETTINGS_ISVIRTUAL_KEY);

    if ((itVPort != settings.end() && itVPort.value().toBool()) || SETTINGS_ISVIRTUAL_DEFAULT)
    {
        _midiOut->openVirtualPort(RTMIDI_PORT_NAME);
        return _midiOut->isPortOpen();
    }

#endif
    const auto itPort = settings.find(SETTINGS_PORTNAME_KEY);
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

void QMidiMscOutputService::stop()
{
    _midiOut->closePort();
}

void QMidiMscOutputService::sendMessage(const MscMessage& message)
{

}

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

#include "QMidiMscInputService.hpp"

const QString QMidiMscInputService::SETTINGS_PORTNAME_KEY = "in_midi_port";
const QString QMidiMscInputService::SETTINGS_PORTNAME_DEFAULT = "";
const QString QMidiMscInputService::SETTINGS_ISVIRTUAL_KEY = "in_midi_is_virtual";
const bool QMidiMscInputService::SETTINGS_ISVIRTUAL_DEFAULT;

QVariantMap QMidiMscInputService::staticDefaultSettings()
{
    return QVariantMap
    {
        { SETTINGS_PORTNAME_KEY, SETTINGS_PORTNAME_DEFAULT },
        { SETTINGS_ISVIRTUAL_KEY, SETTINGS_ISVIRTUAL_DEFAULT }
    };
}

QMidiMscInputService::QMidiMscInputService(QObject* parent)
    : QCueTxInputService(parent),
      _midiIn(new QRtMidiIn(this, RtMidi::Api::UNSPECIFIED, "Cue TX MIDI Input Client"))
{
    _midiIn->ignoreTypes(false, true, true);
}

bool QMidiMscInputService::start(const QVariantMap& settings)
{
    connect(_midiIn, &QRtMidiIn::messageReceived, this, &QMidiMscInputService::processMessage);

#ifdef Q_OS_MAC
    const auto itVPort = settings.find(SETTINGS_ISVIRTUAL_KEY);

    if ((itVPort != settings.end() && itVPort.value().toBool()) || SETTINGS_ISVIRTUAL_DEFAULT)
    {
        _midiIn->openVirtualPort(RTMIDI_PORT_NAME);
        return _midiIn->isPortOpen();
    }

#endif
    const auto itPort = settings.find(SETTINGS_PORTNAME_KEY);
    if (itPort != settings.end())
    {
        const auto ports = QRtMidiIn::getMidiInPorts();

        for(const auto& pair : ports.toStdMap())
        {
            if (pair.second == itPort.value().toString())
            {
                _midiIn->openPort(pair.first, RTMIDI_PORT_NAME);
                return _midiIn->isPortOpen();
            }
        }
    }

    // Just open the first port we can find, if any
    _midiIn->openPort(0, RTMIDI_PORT_NAME);
    return _midiIn->isPortOpen();
}

void QMidiMscInputService::stop()
{
    if (_midiIn->isPortOpen())
        _midiIn->closePort();

    disconnect(_midiIn, &QRtMidiIn::messageReceived, this, &QMidiMscInputService::processMessage);
}

void QMidiMscInputService::processMessage(double timestamp, const QByteArray& message)
{
    MscMessage mscMessage;
    if (MscMessage::fromByteArray(message, mscMessage))
        emit messageReceived(mscMessage);
}

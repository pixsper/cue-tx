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

#include "QRtMidiOut.h"

QRtMidiOut::QRtMidiOut(QObject* parent, RtMidi::Api api, const QString& clientName)
    : QObject(parent),
      _midiOut(api, clientName.toStdString())
{
    _midiOut.setErrorCallback(&QRtMidiOut::rtMidiErrorCallback, this);
}

QRtMidiOut::QRtMidiOut(RtMidi::Api api, const QString& clientName)
    : QRtMidiOut(nullptr, api, clientName)
{

}

RtMidi::Api QRtMidiOut::getCurrentApi()
{
    return _midiOut.getCurrentApi();
}

void QRtMidiOut::openPort(int portNumber, const QString& portName)
{
    _midiOut.openPort(static_cast<unsigned int>(portNumber), portName.toStdString());
}

void QRtMidiOut::openVirtualPort(const QString& portName)
{
    _midiOut.openVirtualPort(portName.toStdString());
}

void QRtMidiOut::closePort()
{
    _midiOut.closePort();
}

bool QRtMidiOut::isPortOpen() const
{
    return _midiOut.isPortOpen();
}

int QRtMidiOut::getPortCount()
{
    return static_cast<int>(_midiOut.getPortCount());
}

QString QRtMidiOut::getPortName(int portNumber)
{
    return QString::fromStdString(_midiOut.getPortName(static_cast<unsigned int>(portNumber)));
}

QMap<int, QString> QRtMidiOut::getMidiOutPorts()
{
    QMap<int, QString> midiPorts;
    RtMidiOut midiOut;
    const quint32 midiPortCount = midiOut.getPortCount();

    for(quint32 i = 0; i < midiPortCount; ++i)
        midiPorts.insert(static_cast<const int>(i), midiOut.getPortName(i).c_str());

    return midiPorts;
}

void QRtMidiOut::rtMidiErrorCallback(RtMidiError::Type type, const std::string &errorText, void *userData)
{
	auto qMidiOut = static_cast<QRtMidiOut*>(userData);

    emit qMidiOut->errorReceived(type, QString::fromStdString(errorText));
}

void QRtMidiOut::sendMessage(const QByteArray& message)
{
    _midiOut.sendMessage(reinterpret_cast<const unsigned char*>(message.data()), static_cast<size_t>(message.size()));
}

void QRtMidiOut::sendMessage(const char* message, int size)
{
    _midiOut.sendMessage(reinterpret_cast<const unsigned char*>(message), static_cast<size_t>(size));
}

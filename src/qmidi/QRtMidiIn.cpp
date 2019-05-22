#include "QRtMidiIn.h"

QRtMidiIn::QRtMidiIn(QObject* parent, RtMidi::Api api, QString clientName, int queueSizeLimit)
    : QObject(parent),
      _midiIn(api, clientName.toStdString(), static_cast<unsigned int>(queueSizeLimit))
{
    _midiIn.setCallback(&QRtMidiIn::rtMidiCallback, this);
    _midiIn.setErrorCallback(&QRtMidiIn::rtMidiErrorCallback, this);
}

QRtMidiIn::QRtMidiIn(RtMidi::Api api, QString clientName, int queueSizeLimit)
    : QRtMidiIn(nullptr, api, clientName, queueSizeLimit)
{

}

QRtMidiIn::~QRtMidiIn()
{
    _midiIn.cancelCallback();
}

RtMidi::Api QRtMidiIn::getCurrentApi()
{
    return _midiIn.getCurrentApi();
}

void QRtMidiIn::openPort(int portNumber, const QString portName)
{
    _midiIn.openPort(static_cast<unsigned int>(portNumber), portName.toStdString());
}

void QRtMidiIn::openVirtualPort(const QString portName)
{
    _midiIn.openVirtualPort(portName.toStdString());
}

void QRtMidiIn::closePort()
{
    _midiIn.closePort();
}

bool QRtMidiIn::isPortOpen() const
{
    return _midiIn.isPortOpen();
}

int QRtMidiIn::getPortCount()
{
    return static_cast<int>(_midiIn.getPortCount());
}

QString QRtMidiIn::getPortName(int portNumber)
{
    return QString::fromStdString(_midiIn.getPortName(static_cast<unsigned int>(portNumber)));
}

void QRtMidiIn::ignoreTypes(bool midiSysex, bool midiTime, bool midiSense)
{
    _midiIn.ignoreTypes(midiSysex, midiTime, midiSense);
}

QMap<int, QString> QRtMidiIn::getMidiInPorts()
{
    QMap<int, QString> midiPorts;
    RtMidiIn midiIn;
    quint32 midiPortCount = midiIn.getPortCount();

    for(quint32 i = 0; i < midiPortCount; ++i)
        midiPorts.insert(static_cast<const int>(i), midiIn.getPortName(i).c_str());

    return midiPorts;
}

void QRtMidiIn::rtMidiCallback(double timeStamp, std::vector<unsigned char>* message, void* userData)
{
    QRtMidiIn* qMidiIn = static_cast<QRtMidiIn*>(userData);

    QByteArray qMessage(reinterpret_cast<const char*>(message->data()), static_cast<int>(message->size()));

    emit qMidiIn->messageReceived(timeStamp, qMessage);
}

void QRtMidiIn::rtMidiErrorCallback(RtMidiError::Type type, const std::string& errorText, void* userData)
{
    QRtMidiIn* qMidiIn = static_cast<QRtMidiIn*>(userData);

    emit qMidiIn->errorReceived(type, QString::fromStdString(errorText));
}


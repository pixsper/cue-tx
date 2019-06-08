#pragma once

#include "QCueTxInputService.h"
#include "../qmidi/QRtMidiIn.h"
#include <QVariant>

class QMidiMscInputService : public QCueTxInputService
{
    QRtMidiIn* _midiIn;

    const char* SETTINGS_PORT_NAME = "port";
    const char* SETTINGS_PORT_DEFAULT = "";

    const char* SETTINGS_ISVIRTUAL_NAME = "is_virtual";
#ifdef Q_OS_MAC
    const bool SETTINGS_ISVIRTUAL_DEFAULT = true;
#else
    const bool SETTINGS_ISVIRTUAL_DEFAULT = false;
#endif

    const char* RTMIDI_PORT_NAME = "Cue TX Input";

    Q_OBJECT
public:
    explicit QMidiMscInputService(QObject* parent = nullptr)
        : QCueTxInputService(parent),
          _midiIn(new QRtMidiIn(this, RtMidi::Api::UNSPECIFIED, "Cue TX MIDI Input Client"))
    {
        _midiIn->ignoreTypes(false, true, true);
    }

    bool start(const QVariantMap& settings) override
    {
        connect(_midiIn, &QRtMidiIn::messageReceived, this, &QMidiMscInputService::processMessage);

#ifdef Q_OS_MAC
        auto itVPort = settings.find(SETTINGS_ISVIRTUAL_NAME);

        if ((itVPort != settings.end() && itVPort.value().toBool()) || SETTINGS_ISVIRTUAL_DEFAULT)
        {
            _midiIn->openVirtualPort(RTMIDI_PORT_NAME);
            return _midiIn->isPortOpen();
        }

#endif
        auto itPort = settings.find(SETTINGS_PORT_NAME);
        if (itPort != settings.end())
        {
            auto ports = QRtMidiIn::getMidiInPorts();

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

    void stop() override
    {
        if (_midiIn->isPortOpen())
            _midiIn->closePort();

        disconnect(_midiIn, &QRtMidiIn::messageReceived, this, &QMidiMscInputService::processMessage);
    }

private slots:
    void processMessage(double timestamp, const QByteArray& message)
    {
        MscMessage mscMessage;
        if (MscMessage::FromByteArray(message, mscMessage))
            emit messageReceived(mscMessage);
    }

};

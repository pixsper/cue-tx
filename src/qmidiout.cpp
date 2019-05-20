#include "qmidiout.h"

QMidiOut::QMidiOut(QObject *parent)
    : QObject(parent),
      _midiOut(new RtMidiOut(RtMidi::Api::UNSPECIFIED, "CueTX MIDI Output Client"))
{
    _midiOut->openVirtualPort("CueTX Output");
}

void QMidiOut::sendMessage(const QByteArray& data)
{
    _midiOut->sendMessage(reinterpret_cast<const unsigned char*>(data.data()), data.size());
}

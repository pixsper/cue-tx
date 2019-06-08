#pragma once

#include <QObject>
#include <QMap>
#include <RtMidi.h>

class QRtMidiIn : public QObject
{
    RtMidiIn _midiIn;

    Q_OBJECT
public:
    QRtMidiIn(QObject *parent, RtMidi::Api api = RtMidi::Api::UNSPECIFIED, const QString& clientName = "RtMidi Input Client", int queueSizeLimit = 100);
    QRtMidiIn(RtMidi::Api api = RtMidi::Api::UNSPECIFIED, const QString& clientName = "RtMidi Input Client", int queueSizeLimit = 100);
    ~QRtMidiIn();

    QRtMidiIn(const QRtMidiIn& other) = delete;
    QRtMidiIn(QRtMidiIn&& other) noexcept = delete;
    QRtMidiIn& operator=(const QRtMidiIn& other) = delete;
    QRtMidiIn& operator=(QRtMidiIn&& other) noexcept = delete;

    RtMidi::Api getCurrentApi();
    void openPort(int portNumber = 0, const QString& portName = "RtMidi Input");
    void openVirtualPort(const QString& portName = "RtMidi Input");
    void closePort();
    bool isPortOpen() const;
    int getPortCount();
    QString getPortName(int portNumber = 0);
    void ignoreTypes(bool midiSysex = true, bool midiTime = true, bool midiSense = true);

    static QMap<int, QString> getMidiInPorts();

private:
    static void rtMidiCallback(double timeStamp, std::vector<unsigned char>* message, void* userData);
    static void rtMidiErrorCallback(RtMidiError::Type type, const std::string &errorText, void *userData);

signals:
    void messageReceived(double timeStamp, const QByteArray& message);
    void errorReceived(RtMidiError::Type type, const QString& errorText);
};

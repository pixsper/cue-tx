#pragma once

#include <QObject>
#include <QMap>
#include <RtMidi.h>

class QRtMidiOut : public QObject
{
    RtMidiOut _midiOut;

    Q_OBJECT
public:
    QRtMidiOut(QObject *parent, RtMidi::Api api = RtMidi::Api::UNSPECIFIED, const QString& clientName = "RtMidi Output Client");
    QRtMidiOut(RtMidi::Api api = RtMidi::Api::UNSPECIFIED, const QString& clientName = "RtMidi Output Client");

    RtMidi::Api getCurrentApi();
    void openPort(int portNumber = 0, const QString& portName = "RtMidi Output");
    void closePort();
    bool isPortOpen() const;
    void openVirtualPort(const QString& portName = "RtMidi Output");
    int getPortCount();
    QString getPortName(int portNumber = 0);

    static QMap<int, QString> getMidiOutPorts();

private:
    static void rtMidiErrorCallback(RtMidiError::Type type, const std::string &errorText, void *userData);

public slots:
    void sendMessage(const QByteArray& message);
    void sendMessage(const char* message, int size);

signals:
    void errorReceived(RtMidiError::Type type, const QString& errorText);
};

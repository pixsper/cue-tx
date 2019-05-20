#pragma once

#include <QObject>
#include <RtMidi.h>
#include <memory>

class QMidiOut : public QObject
{
    QScopedPointer<RtMidiOut> _midiOut;

    Q_OBJECT
public:
    explicit QMidiOut(QObject *parent = nullptr);

signals:

public slots:
    void sendMessage(const QByteArray& data);
};

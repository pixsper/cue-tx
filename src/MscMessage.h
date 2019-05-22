#pragma once
#include <QtGlobal>
#include <QString>
#include <QByteArray>

enum MscCommandType : quint8
{
        Go = 0x01,
        Stop = 0x02,
        Resume = 0x03,
        TimedGo = 0x04,
        Load = 0x05,
        Set = 0x06,
        Fire = 0x07,
        AllOff = 0x08,
        Restore = 0x09,
        Reset = 0x0A,
        GoOff = 0x0B,
        GoJamClock = 0x10,
        StandbyPlus = 0x11,
        StandbyMinus = 0x12,
        SequencePlus = 0x13,
        SequenceMinus = 0x14,
        StartClock = 0x15,
        StopClock = 0x16,
        ZeroClock = 0x17,
        SetClock = 0x18,
        MtcChaseOn = 0x19,
        MtcChaseOff = 0x1A,
        OpenCueList = 0x1B,
        CloseCueList = 0x1C,
        OpenCuePath = 0x1D,
        CloseCuePath = 0x1E
};

struct MscMessage
{
    int _deviceId;
    MscCommandType _commandType;
    int _commandFormat;

    QString _cueNumber;
    QString _cueList;
    QString _cuePath;

    int _controlNumber;
    int _controlValue;

    int _macroNumber;

public:
    static bool FromByteArray(const QByteArray& array, MscMessage& message)
    {
        return false;
    }

    MscMessage();
};



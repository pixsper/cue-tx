#pragma once

enum MscTimecodeFramerate
{
    F24 = 0,
    F25 = 1,
    F30Df = 2,
    F30Nd = 3,
};

enum MidiTimecodeStatusFlags
{
    FirstFrameIn4Or8FieldVideoSequence = 0x10,
    InvalidTimecode = 0x20,
    TachOrControlTrackUpdatedCode = 0x40
};

struct MscTimecode
{
    MscTimecodeFramerate _framerate;

    int _hours;
    int _minutes;
    int _seconds;
    int _frames;
    int _subframes;

    bool _isNegative;
    bool _isColorFrame;

public:
    MscTimecode();
};


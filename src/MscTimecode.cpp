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

#include "MscTimecode.h"

int getFrameCount(MidiTimecodeFramerate framerate)
{
    switch(framerate)
    {
        case MidiTimecodeFramerate::F24:
            return 24;

        case MidiTimecodeFramerate::F25:
            return 25;

        case MidiTimecodeFramerate::F30Df:
        case MidiTimecodeFramerate::F30Nd:
            return 30;
    }
}


MscTimecode::MscTimecode()
    : _hours(0),
    _minutes(0),
    _seconds(0),
    _frames(0),
    _isNegative(false),
    _isColorFrame(false),
    _status(MidiTimecodeStatusFlags::None)
{

}

MscTimecode::MscTimecode(int hours, int minutes, int seconds, int frames, int subFrames, bool isNegative, bool isColorFrame)
    : _hours(hours),
    _minutes(minutes),
    _seconds(seconds),
    _frames(frames),
    _subFrames(subFrames),
    _isNegative(isNegative),
    _isColorFrame(isColorFrame),
    _status(MidiTimecodeStatusFlags::None)
{

}

MscTimecode::MscTimecode(int hours, int minutes, int seconds, int frames, MidiTimecodeStatusFlags status, bool isNegative, bool isColorFrame)
    : _hours(hours),
    _minutes(minutes),
    _seconds(seconds),
    _frames(frames),
    _subFrames(0),
    _isNegative(isNegative),
    _isColorFrame(isColorFrame),
    _status(status)
{

}

bool MscTimecode::FromByteArray(const QByteArray& data, MscTimecode& value)
{
    if (data.length() != 5)
        return false;

    MidiTimecodeFramerate framerate = static_cast<MidiTimecodeFramerate>(((data[0] & 0x60) >> 5));

    int hours = data[0] & 0x1F;
    if (hours > 23)
        return false;

    bool isColorFrame = (data[1] & 0x40) > 0;

    int minutes = data[1] & 0x3F;
    if (minutes > 59)
        return false;

    // Reserved bit, must be set to 0
    if ((data[2] & 0x40) != 0)
        return false;

    int seconds = data[2] & 0x3F;
    if (seconds > 59)
        return false;

    bool isNegative = (data[3] & 0x40) > 0;
    bool isFinalBitStatus = (data[3] & 0x20) > 0;

    int frames = data[3] & 0x1F;
    if (frames > getFrameCount(framerate))
        return false;

    int subFrames = -1;
    MidiTimecodeStatusFlags status = MidiTimecodeStatusFlags::None;

    if (isFinalBitStatus)
    {
        // Reserved bits, must be set to 0
        if ((data[4] & 0x0F) != 0)
            return false;

        status = static_cast<MidiTimecodeStatusFlags>(data[4]);
    }
    else
    {
        subFrames = data[4] & 0x7F;
        if (subFrames > 99)
            return false;
    }

    value._hours = hours;
    value._minutes = minutes;
    value._seconds = seconds;
    value._frames = frames;
    value._subFrames = subFrames;
    value._isNegative = isNegative;
    value._isColorFrame = isColorFrame;
    value._status = status;

    return true;
}

QByteArray MscTimecode::ToByteArray()
{
    QByteArray data;
    data[0] = static_cast<char>((static_cast<quint8>(_framerate) << 5) + _hours);
    data[1] = static_cast<char>((_isColorFrame ? 0x40 : 0x00) + _minutes);
    data[2] = static_cast<char>(_seconds);
    data[3] = static_cast<char>((_isNegative ? 0x40 : 0x00) + (_status != MidiTimecodeStatusFlags::None ? 0x20 : 0x00) + _frames);
    data[4] = _status != MidiTimecodeStatusFlags::None ? static_cast<char>(_status) : static_cast<char>(_subFrames != -1 ? _subFrames : 0);

    return data;
}

QDataStream& operator>>(QDataStream& stream, MscTimecode& timecode)
{
    QByteArray timecodeRaw(5, 0);
    stream.readRawData(timecodeRaw.data(), 5);
    timecode.FromByteArray(timecodeRaw, timecode);
    return stream;
}

QDataStream& operator<<(QDataStream& stream, MscTimecode& timecode)
{
    QByteArray data = timecode.ToByteArray();
    stream.writeBytes(data, static_cast<uint>(data.length()));
    return stream;
}

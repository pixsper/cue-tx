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

#pragma once

#include <QByteArray>
#include <QDataStream>

enum class MidiTimecodeFramerate : quint8
{
    F24 = 0,
    F25 = 1,
    F30Df = 2,
    F30Nd = 3,
};

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

enum class MidiTimecodeStatusFlags : quint8
{
    None = 0x00,
    FirstFrameIn4Or8FieldVideoSequence = 0x10,
    InvalidTimecode = 0x20,
    TachOrControlTrackUpdatedCode = 0x40
};

struct MscTimecode
{
    MidiTimecodeFramerate _framerate;

    int _hours;
    int _minutes;
    int _seconds;
    int _frames;
    int _subFrames;

    bool _isNegative;
    bool _isColorFrame;

    MidiTimecodeStatusFlags _status;

public:
    MscTimecode();
    MscTimecode(int hours, int minutes, int seconds, int frames, int subFrames = 0,
                bool isNegative = false, bool isColorFrame = false);
    MscTimecode(int hours, int minutes, int seconds, int frames,
                MidiTimecodeStatusFlags status = MidiTimecodeStatusFlags::None, bool isNegative = false, bool isColorFrame = false );

    static bool FromByteArray(const QByteArray& data, MscTimecode& value);

    QByteArray ToByteArray();
};

QDataStream& operator>>(QDataStream& stream, MscTimecode& cueId);
QDataStream& operator<<(QDataStream& stream, MscTimecode& cueId);


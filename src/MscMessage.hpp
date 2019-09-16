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

#include <QtGlobal>
#include <QByteArray>

#include "nonstd/optional.hpp"

#include "MscCueId.hpp"
#include "MscTimecode.hpp"

enum class MscCommandType : quint8
{
        None = 0x00,
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

enum class MscCommandFormat : quint8
{
        None = 0x00,
        LightingGeneral = 0x01,
        MovingLights = 0x02,
        ColorChangers = 0x03,
        Strobes = 0x04,
        Lasers = 0x05,
        Chasers = 0x06,

        SoundGeneral = 0x10,
        Music = 0x11,
        CdPlayers = 0x12,
        EpromPlayback = 0x13,
        AudioTapeMachines = 0x14,
        Intercoms = 0x15,
        Amplifiers = 0x16,
        AudioEffectsDevices = 0x17,
        Equalizers = 0x18,

        MachineryGeneral = 0x20,
        Rigging = 0x21,
        Flies = 0x22,
        Lifts = 0x23,
        Turntables = 0x24,
        Trusses = 0x25,
        Robots = 0x26,
        Animation = 0x27,
        Floats = 0x28,
        Breakaways = 0x29,
        Barges = 0x2A,

        VideoGeneral = 0x30,
        VideoTapeMachines = 0x31,
        VideoCassetteMachines = 0x32,
        VideodiscPlayers = 0x33,
        VideoSwitchers = 0x34,
        VideoEffects = 0x35,
        VideoCharacterGenerators = 0x36,
        VideoStillStores = 0x37,
        VideoMonitors = 0x38,

        ProjectionGeneral = 0x40,
        FilmProjectors = 0x41,
        SlideProjectors = 0x42,
        VideoProjectors = 0x43,
        Dissolvers = 0x44,
        ShutterControls = 0x45,

        ProcessControlGeneral = 0x50,
        HydraulicOil = 0x51,
        H20 = 0x52,
        Co2 = 0x53,
        CompressedAir = 0x54,
        NaturalGas = 0x55,
        Fog = 0x56,
        Smoke = 0x57,
        CrackedHaze = 0x58,

        PyroGeneral = 0x60,
        Fireworks = 0x61,
        Explosions = 0x62,
        Flame = 0x63,
        Smokepots = 0x64,

        AllTypes = 0x7F
};

struct MscMessage
{
    static const quint8 SYSEX_START = 0xF0;
    static const quint8 SYSEX_END = 0xF7;
    static const quint8 SYSEX_REALTIME_ID = 0x7F;
    static const quint8 MSC_SUBID = 0x02;

    static const quint8 DEVICEID_MIN = 0;
    static const quint8 DEVICEID_MAX = 127;

    static const quint8 MACRONUMBER_MAX = 127;

    static const int MSC_MESSAGE_MAX_LENGTH = 128;

    int _deviceId;
    MscCommandType _commandType;
    MscCommandFormat _commandFormat;

    nonstd::optional<MscTimecode> _timecode;

    nonstd::optional<MscCueId> _cueNumber;
    nonstd::optional<MscCueId> _cueList;
    nonstd::optional<MscCueId> _cuePath;

    nonstd::optional<int> _controlNumber;
    nonstd::optional<int> _controlValue;

    nonstd::optional<int> _macroNumber;

    static bool fromByteArray(const QByteArray& array, MscMessage& message);

    MscMessage();

    bool toByteArray(QByteArray& array) const;

    void removeZeroPadding();
};



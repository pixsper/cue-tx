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

#include "MscMessage.hpp"
#include <QBuffer>

bool MscMessage::FromByteArray(const QByteArray& array, MscMessage& message)
{
    QDataStream data(array);

    quint8 start;
    data >> start;

    if (start != SYSEX_START)
        return false;

    quint8 realTimeid;
    data >> realTimeid;

    if (realTimeid != SYSEX_REALTIME_ID)
        return false;

    quint8 deviceId;
    data >> deviceId;

    if (deviceId > DEVICEID_MAX)
        return false;

    quint8 mscSubId;
    data >> mscSubId;

    if (mscSubId != MSC_SUBID)
        return false;

    quint8 commandFormat;
    data >> commandFormat;

    if (commandFormat > static_cast<quint8>(MscCommandFormat::AllTypes))
        return false;

    quint8 commandType;
    data >> commandType;

    std::optional<MscTimecode> timecode;

    std::optional<MscCueId> cueNumber;
    std::optional<MscCueId> cueList;
    std::optional<MscCueId> cuePath;

    std::optional<int> controlNumber = -1;
    std::optional<int> controlValue = -1;
    std::optional<int> macroNumber = -1;

    char peek;

    switch(static_cast<MscCommandType>(commandType))
    {
        case MscCommandType::Go:
        case MscCommandType::Stop:
        case MscCommandType::Resume:
        case MscCommandType::Load:
        case MscCommandType::GoOff:
        case MscCommandType::GoJamClock:

            if (data.device()->peek(&peek, 1) != 1)
                return false;

            if (peek != static_cast<char>(SYSEX_END))
            {
                data >> cueNumber.value();

                if (data.device()->peek(&peek, 1) != 1)
                    return false;

                if (peek != static_cast<char>(SYSEX_END))
                {
                    data >> cueList.value();

                    if (data.device()->peek(&peek, 1) != 1)
                        return false;

                    if (peek != static_cast<char>(SYSEX_END))
                    {
                        data >> cuePath.value();
                    }
                }
            }

            break;

        case MscCommandType::TimedGo:

            data >> timecode.value();

            if (data.device()->peek(&peek, 1) != 1)
                return false;

            if (peek != static_cast<char>(SYSEX_END))
            {
                data >> cueNumber.value();

                if (data.device()->peek(&peek, 1) != 1)
                    return false;

                if (peek != static_cast<char>(SYSEX_END))
                {
                    data >> cueList.value();

                    if (data.device()->peek(&peek, 1) != 1)
                        return false;

                    if (peek != static_cast<char>(SYSEX_END))
                    {
                        data >> cuePath.value();
                    }
                }
            }


            break;

        case MscCommandType::Set:
        {
            quint8 lsb, msb;

            data >> lsb;
            data >> msb;
            controlNumber = (lsb & 0x7F) + (msb << 7);

            data >> lsb;
            data >> msb;
            controlValue = (lsb & 0x7F) + (msb << 7);

            if (data.device()->peek(&peek, 1) != 1)
                return false;

            if (peek != static_cast<char>(SYSEX_END))
                data >> timecode.value();
        }
            break;

        case MscCommandType::Fire:
        {
            quint8 macroNumberByte;
            data >> macroNumberByte;
            macroNumber = macroNumberByte;

            if (macroNumber > MACRONUMBER_MAX)
                return false;
        }
            break;

        case MscCommandType::StandbyPlus:
        case MscCommandType::StandbyMinus:
        case MscCommandType::SequencePlus:
        case MscCommandType::SequenceMinus:
        case MscCommandType::StartClock:
        case MscCommandType::StopClock:
        case MscCommandType::ZeroClock:
        case MscCommandType::MtcChaseOn:
        case MscCommandType::MtcChaseOff:

            if (data.device()->peek(&peek, 1) != 1)
                return false;

            if (peek != static_cast<char>(SYSEX_END))
                data >> cueList.value();

            break;

        case MscCommandType::SetClock:

            data >> timecode.value();

            if (data.device()->peek(&peek, 1) != 1)
                return false;

            if (peek != static_cast<char>(SYSEX_END))
                data >> cueList.value();

            break;

        case MscCommandType::OpenCueList:
        case MscCommandType::CloseCueList:

            if (data.device()->peek(&peek, 1) != 1)
                return false;

            if (peek == static_cast<char>(SYSEX_END))
                return false;

            data >> cueList.value();

            break;

        case MscCommandType::OpenCuePath:
        case MscCommandType::CloseCuePath:

            if (data.device()->peek(&peek, 1) != 1)
                return false;

            if (peek == static_cast<char>(SYSEX_END))
                return false;

            data >> cuePath.value();

            break;

        case MscCommandType::AllOff:
        case MscCommandType::Restore:
        case MscCommandType::Reset:
            break;

        default:
            return false;
    }

    quint8 end;
    data >> end;

    if (end != SYSEX_END)
        return false;

    message._deviceId = deviceId;
    message._commandFormat = static_cast<MscCommandFormat>(commandFormat);
    message._commandType = static_cast<MscCommandType>(commandType);
    message._timecode = timecode;
    message._cueNumber = cueNumber;
    message._cueList = cueList;
    message._cuePath = cuePath;
    message._controlNumber = controlNumber;
    message._controlValue = controlValue;
    message._macroNumber = macroNumber;

    return true;
}

MscMessage::MscMessage()
    : _deviceId(0),
      _commandType(MscCommandType::Go),
    _commandFormat(MscCommandFormat::AllTypes),
    _timecode(),
    _cueNumber(),
    _cueList(),
    _cuePath(),
    _controlNumber(-1),
    _controlValue(-1),
    _macroNumber(-1)
{

}


bool MscMessage::ToByteArray(QByteArray& array)
{
    array.reserve(MSC_MESSAGE_MAX_LENGTH);

    QDataStream data(array);

    data << SYSEX_START;
    data << SYSEX_REALTIME_ID;
    data << static_cast<quint8>(_deviceId);
    data << MSC_SUBID;
    data << static_cast<quint8>(_commandFormat);
    data << static_cast<quint8>(_commandType);

    switch(static_cast<MscCommandType>(_commandType))
    {
        case MscCommandType::Go:
        case MscCommandType::Stop:
        case MscCommandType::Resume:
        case MscCommandType::Load:
        case MscCommandType::GoOff:
        case MscCommandType::GoJamClock:

            if (_cueNumber.has_value())
            {
                data << _cueNumber.value();

                if (_cueList.has_value())
                {
                    data << _cueList.value();

                    if (_cuePath.has_value())
                    {
                        data << _cuePath.value();
                    }
                }
            }

            break;

        case MscCommandType::TimedGo:

            if (!_timecode.has_value())
                return false;

            data << _timecode.value();

            if (_cueNumber.has_value())
            {
                data << _cueNumber.value();

                if (_cueList.has_value())
                {
                    data << _cueList.value();

                    if (_cuePath.has_value())
                    {
                        data << _cuePath.value();
                    }
                }
            }

            break;

        case MscCommandType::Set:

            if (!(_controlNumber.has_value() && _controlValue.has_value()))
                return false;

            data << static_cast<quint8>(_controlNumber.value() >> 7);
            data << static_cast<quint8>(_controlNumber.value() & 0x7F);
            data << static_cast<quint8>(_controlValue.value() >> 7);
            data << static_cast<quint8>(_controlValue.value() & 0x7F);

            break;

        case MscCommandType::Fire:

            if (!_macroNumber.has_value())
                return false;

            data << static_cast<quint8>(_macroNumber.value());


            break;

        case MscCommandType::StandbyPlus:
        case MscCommandType::StandbyMinus:
        case MscCommandType::SequencePlus:
        case MscCommandType::SequenceMinus:
        case MscCommandType::StartClock:
        case MscCommandType::StopClock:
        case MscCommandType::ZeroClock:
        case MscCommandType::MtcChaseOn:
        case MscCommandType::MtcChaseOff:

            if (_cueList.has_value())
                data << _cueList.value();

            break;

        case MscCommandType::SetClock:

            if (!_timecode.has_value())
                return false;

            data << _timecode.value();

            if (_cueList.has_value())
                data << _cueList.value();

            break;

        case MscCommandType::OpenCueList:
        case MscCommandType::CloseCueList:

            if (!_cueList.has_value())
                return false;

            data << _cueList.value();


            break;

        case MscCommandType::OpenCuePath:
        case MscCommandType::CloseCuePath:

            if (!_cuePath.has_value())
                return false;

            data << _cuePath.value();


            break;

        case MscCommandType::AllOff:
        case MscCommandType::Restore:
        case MscCommandType::Reset:
            break;

        default:
            break;
    }

    data << SYSEX_END;

    return true;
}

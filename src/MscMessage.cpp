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
#include "oscpp/client.hpp"

const quint8 MscMessage::MACRONUMBER_MAX;

const QMap<MscCommandType, QString> MscMessage::COMMAND_OSC_LABELS
{
    { MscCommandType::None, "none" },
    { MscCommandType::Go, "go" },
    { MscCommandType::Stop, "stop" },
    { MscCommandType::Resume, "resume" },
    { MscCommandType::TimedGo, "timedgo" },
    { MscCommandType::Load, "load" },
    { MscCommandType::Set, "set" },
    { MscCommandType::Fire, "fire" },
    { MscCommandType::AllOff, "alloff" },
    { MscCommandType::Restore, "restore" },
    { MscCommandType::Reset, "reset" },
    { MscCommandType::GoOff, "gooff" },
    { MscCommandType::GoJamClock, "gojamclock" },
    { MscCommandType::StandbyPlus, "standbyplus" },
    { MscCommandType::StandbyMinus, "standbyminus" },
    { MscCommandType::SequencePlus, "sequenceplus" },
    { MscCommandType::SequenceMinus, "sequenceminus" },
    { MscCommandType::StartClock, "startclock" },
    { MscCommandType::StopClock, "stopclock" },
    { MscCommandType::ZeroClock, "zeroclock" },
    { MscCommandType::SetClock, "setclock" },
    { MscCommandType::MtcChaseOn, "mtcchaseon" },
    { MscCommandType::MtcChaseOff, "mtcchaseoff" },
    { MscCommandType::OpenCueList, "opencuelist" },
    { MscCommandType::CloseCueList, "closecuelist" },
    { MscCommandType::OpenCuePath, "opencuepath" },
    { MscCommandType::CloseCuePath, "closecuepath" }
};

bool MscMessage::fromByteArray(const QByteArray& array, MscMessage& message)
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

    nonstd::optional<MscTimecode> timecode;

    nonstd::optional<MscCueId> cueNumber;
    nonstd::optional<MscCueId> cueList;
    nonstd::optional<MscCueId> cuePath;

    nonstd::optional<int> controlNumber = -1;
    nonstd::optional<int> controlValue = -1;
    nonstd::optional<int> macroNumber = -1;

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
                data >> cueNumber;

                if (data.device()->peek(&peek, 1) != 1)
                    return false;

                if (peek != static_cast<char>(SYSEX_END))
                {
                    data >> cueList;

                    if (data.device()->peek(&peek, 1) != 1)
                        return false;

                    if (peek != static_cast<char>(SYSEX_END))
                    {
                        data >> cuePath;
                    }
                }
            }

            break;

        case MscCommandType::TimedGo:

            data >> timecode;

            if (data.device()->peek(&peek, 1) != 1)
                return false;

            if (peek != static_cast<char>(SYSEX_END))
            {
                data >> cueNumber;

                if (data.device()->peek(&peek, 1) != 1)
                    return false;

                if (peek != static_cast<char>(SYSEX_END))
                {
                    data >> cueList;

                    if (data.device()->peek(&peek, 1) != 1)
                        return false;

                    if (peek != static_cast<char>(SYSEX_END))
                    {
                        data >> cuePath;
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
                data >> timecode;
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
                data >> cueList;

            break;

        case MscCommandType::SetClock:

            data >> timecode;

            if (data.device()->peek(&peek, 1) != 1)
                return false;

            if (peek != static_cast<char>(SYSEX_END))
                data >> cueList;

            break;

        case MscCommandType::OpenCueList:
        case MscCommandType::CloseCueList:

            if (data.device()->peek(&peek, 1) != 1)
                return false;

            if (peek == static_cast<char>(SYSEX_END))
                return false;

            data >> cueList;

            break;

        case MscCommandType::OpenCuePath:
        case MscCommandType::CloseCuePath:

            if (data.device()->peek(&peek, 1) != 1)
                return false;

            if (peek == static_cast<char>(SYSEX_END))
                return false;

            data >> cuePath;

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
      _commandType(MscCommandType::None),
    _commandFormat(MscCommandFormat::None)
{

}


bool MscMessage::toByteArray(QByteArray& array) const
{
    array.reserve(MSC_MESSAGE_MAX_LENGTH);

    QDataStream stream(&array, QIODevice::OpenModeFlag::WriteOnly);

    stream << SYSEX_START;
    stream << SYSEX_REALTIME_ID;
    stream << static_cast<quint8>(_deviceId);
    stream << MSC_SUBID;
    stream << static_cast<quint8>(_commandFormat);
    stream << static_cast<quint8>(_commandType);

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
                stream << _cueNumber.value();

                if (_cueList.has_value())
                {
                    stream << static_cast<quint8>(0);
                    stream << _cueList.value();

                    if (_cuePath.has_value())
                    {
                        stream << static_cast<quint8>(0);
                        stream << _cuePath.value();
                    }
                }
            }

            break;

        case MscCommandType::TimedGo:

            if (!_timecode.has_value())
                return false;

            stream << _timecode.value();

            if (_cueNumber.has_value())
            {
                stream << _cueNumber.value();

                if (_cueList.has_value())
                {
                    stream << static_cast<quint8>(0);
                    stream << _cueList.value();

                    if (_cuePath.has_value())
                    {
                        stream << static_cast<quint8>(0);
                        stream << _cuePath.value();
                    }
                }
            }

            break;

        case MscCommandType::Set:

            if (!(_controlNumber.has_value() && _controlValue.has_value()))
                return false;

            stream << static_cast<quint8>(_controlNumber.value() & 0x7F);
            stream << static_cast<quint8>(_controlNumber.value() >> 7);
            stream << static_cast<quint8>(_controlValue.value() & 0x7F);
            stream << static_cast<quint8>(_controlValue.value() >> 7);

            break;

        case MscCommandType::Fire:

            if (!_macroNumber.has_value())
                return false;

            stream << static_cast<quint8>(_macroNumber.value());


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
                stream << _cueList.value();

            break;

        case MscCommandType::SetClock:

            if (!_timecode.has_value())
                return false;

            if (!_cueList.has_value())
                stream << _cueList.value();

            break;

        case MscCommandType::OpenCueList:
        case MscCommandType::CloseCueList:

            if (!_cueList.has_value())
                return false;

            stream << _cueList.value();


            break;

        case MscCommandType::OpenCuePath:
        case MscCommandType::CloseCuePath:

            if (!_cuePath.has_value())
                return false;

            stream << _cuePath.value();


            break;

        case MscCommandType::AllOff:
        case MscCommandType::Restore:
        case MscCommandType::Reset:
            break;

        default:
            return false;
    }

    stream << SYSEX_END;

    return true;
}

bool MscMessage::toOscPacket(QByteArray& array) const
{
    QByteArray buffer(512, 0);
    OSCPP::Client::Packet packet(buffer.data(), static_cast<size_t>(buffer.size()));

    std::string address = QString("msc/format/%1/device/%2/%3")
                      .arg(QString::number(static_cast<quint8>(_commandFormat)),
                           QString::number(_deviceId),
                           COMMAND_OSC_LABELS[_commandType]).toStdString();

    size_t tag_count = 0;

    switch (_commandType)
    {
        case MscCommandType::Go:
        case MscCommandType::Stop:
        case MscCommandType::Resume:
        case MscCommandType::Load:
        case MscCommandType::GoOff:
        case MscCommandType::GoJamClock:

            if (_cueNumber.has_value())
            {
                tag_count++;
                if (_cueList.has_value())
                {
                    tag_count++;
                    if (_cuePath.has_value())
                        tag_count++;
                }
            }

            switch(tag_count)
            {
                case 0:
                    packet.openMessage(address.c_str(), tag_count).closeMessage();
                    break;
                case 1:
                    packet.openMessage(address.c_str(), tag_count)
                            .string(_cueNumber->toString().toStdString().c_str())
                            .closeMessage();
                    break;
                case 2:
                    packet.openMessage(address.c_str(), tag_count)
                            .string(_cueNumber->toString().toStdString().c_str())
                            .string(_cueList->toString().toStdString().c_str())
                            .closeMessage();
                    break;
                case 3:
                    packet.openMessage(address.c_str(), tag_count)
                            .string(_cueNumber->toString().toStdString().c_str())
                            .string(_cueList->toString().toStdString().c_str())
                            .string(_cuePath->toString().toStdString().c_str())
                            .closeMessage();
                    break;
                default:
                    return false;
            }

            break;

        case MscCommandType::TimedGo:

            if (!_timecode.has_value())
                return false;

            tag_count++;

            if (_cueNumber.has_value())
            {
                tag_count++;
                if (_cueList.has_value())
                {
                    tag_count++;
                    if (_cuePath.has_value())
                        tag_count++;
                }
            }

            switch(tag_count)
            {
                case 1:
                    packet.openMessage(address.c_str(), tag_count)
                            .string(_timecode->toString().toStdString().c_str())
                            .closeMessage();
                    break;
                case 2:
                    packet.openMessage(address.c_str(), tag_count)
                            .string(_timecode->toString().toStdString().c_str())
                            .string(_cueNumber->toString().toStdString().c_str())
                            .closeMessage();
                    break;
                case 3:
                    packet.openMessage(address.c_str(), tag_count)
                            .string(_timecode->toString().toStdString().c_str())
                            .string(_cueNumber->toString().toStdString().c_str())
                            .string(_cueList->toString().toStdString().c_str())
                            .closeMessage();
                    break;
                case 4:
                    packet.openMessage(address.c_str(), tag_count)
                            .string(_timecode->toString().toStdString().c_str())
                            .string(_cueNumber->toString().toStdString().c_str())
                            .string(_cueList->toString().toStdString().c_str())
                            .string(_cuePath->toString().toStdString().c_str())
                            .closeMessage();
                    break;
                default:
                    return false;
            }

            break;

        case MscCommandType::Set:

            if (!_controlNumber.has_value() || !_controlValue.has_value())
                return false;

            tag_count = 2;

            packet.openMessage(address.c_str(), tag_count)
                    .int32(_controlNumber.value())
                    .int32(_controlValue.value())
                    .closeMessage();

            break;

        case MscCommandType::Fire:

            if (!_macroNumber.has_value())
                return false;

            tag_count = 1;

            packet.openMessage(address.c_str(), tag_count)
                    .int32(_macroNumber.value())
                    .closeMessage();

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
            {
                tag_count = 1;
                packet.openMessage(address.c_str(), tag_count)
                        .string(_cueList->toString().toStdString().c_str())
                        .closeMessage();
            }
            else
            {
                tag_count = 0;
                packet.openMessage(address.c_str(), tag_count).closeMessage();
            }

            break;

        case MscCommandType::SetClock:

            if (!_timecode.has_value())
                return false;

            if (_cueList.has_value())
            {
                tag_count = 2;
                packet.openMessage(address.c_str(), tag_count)
                        .string(_timecode->toString().toStdString().c_str())
                        .string(_cueList->toString().toStdString().c_str())
                        .closeMessage();
            }
            else
            {
                tag_count = 1;
                packet.openMessage(address.c_str(), tag_count)
                        .string(_timecode->toString().toStdString().c_str())
                        .closeMessage();
            }

            break;

        case MscCommandType::OpenCueList:
        case MscCommandType::CloseCueList:

            if (!_cueList.has_value())
                return false;

            tag_count = 1;
            packet.openMessage(address.c_str(), tag_count)
                    .string(_cueList->toString().toStdString().c_str())
                    .closeMessage();

            break;

        case MscCommandType::OpenCuePath:
        case MscCommandType::CloseCuePath:

            if (!_cuePath.has_value())
                return false;

            tag_count = 1;
            packet.openMessage(address.c_str(), tag_count)
                    .string(_cuePath->toString().toStdString().c_str())
                    .closeMessage();

            break;

        case MscCommandType::AllOff:
        case MscCommandType::Restore:
        case MscCommandType::Reset:

            tag_count = 0;
            packet.openMessage(address.c_str(), tag_count).closeMessage();

            break;

        default:
            return false;
    }

    array.resize(static_cast<int>(packet.size()));
    return true;
}

void MscMessage::removeZeroPadding()
{
    if (_cueNumber.has_value())
        _cueNumber.value().removeZeroPadding();

    if (_cueList.has_value())
        _cueList.value().removeZeroPadding();

    if (_cuePath.has_value())
        _cuePath.value().removeZeroPadding();
}

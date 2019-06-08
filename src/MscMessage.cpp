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

    MscTimecode timecode;

    MscCueId cueNumber;
    MscCueId cueList;
    MscCueId cuePath;

    int controlNumber = -1;
    int controlValue = -1;
    int macroNumber = -1;

    switch(static_cast<MscCommandType>(commandType))
    {
        case MscCommandType::Go:
        case MscCommandType::Stop:
        case MscCommandType::Resume:
        case MscCommandType::Load:
        case MscCommandType::GoOff:
        case MscCommandType::GoJamClock:

            data >> cueNumber;

            if (cueNumber.isValid())
            {
                 data >> cueList;

                if (cueList.isValid())
                     data >> cuePath;
            }

            break;

        case MscCommandType::TimedGo:

            data >> timecode;

            data >> cueNumber;

            if (cueNumber.isValid())
            {
                 data >> cueList;

                if (cueList.isValid())
                     data >> cuePath;
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
            data >> cueList;
            break;

        case MscCommandType::SetClock:
            data >> timecode;
            // TODO: Check timecode is not null

            data >> cueList;

            break;

        case MscCommandType::OpenCueList:
        case MscCommandType::CloseCueList:
            data >> cueList;

            if (!cueList.isValid())
                return false;

            break;

        case MscCommandType::OpenCuePath:
        case MscCommandType::CloseCuePath:
            data >> cuePath;

            if (!cuePath.isValid())
                return false;

            break;

        case MscCommandType::AllOff:
        case MscCommandType::Restore:
        case MscCommandType::Reset:
            break;

        default:
            return false;
    }

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
{

}

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

#include "MscCueId.hpp"
#include "MscMessage.hpp"

const QRegularExpression MscCueId::VALID_CUEID(R"(^\d+(?:\.\d+)*$)");

bool MscCueId::isValidMscCue(const QString& mscCue)
{
	const auto match = VALID_CUEID.match(mscCue);
    return match.hasMatch();
}

MscCueId::MscCueId()
= default;

MscCueId::MscCueId(const QString& cueString)
    : _cueParts(cueString.split('.'))
{

}

MscCueId::MscCueId(std::initializer_list<const char*> cueParts)
{
    for (const char* p : cueParts)
        _cueParts.append(p);
}

bool MscCueId::operator==(const MscCueId& rhs) const
{
    if (_cueParts.size() != rhs._cueParts.size())
        return false;

    for (int i = 0; i < _cueParts.size(); ++i)
    {
        if (_cueParts[i] != rhs._cueParts[i])
            return false;
    }

    return true;
}

bool MscCueId::operator!=(const MscCueId& rhs) const
{
    return !(*this == rhs);
}

bool MscCueId::isValid() const
{
    if (_cueParts.empty())
        return false;

    const QRegularExpression cuePartRegex("\\d+");

    for(const auto& part : _cueParts)
    {
        auto match = cuePartRegex.match(part);
        if (!match.hasMatch())
            return false;
    }

    return true;
}

QString MscCueId::toString() const
{
    return _cueParts.join(".");
}

void MscCueId::removeZeroPadding()
{
    if (_cueParts.size() == 2)
    {
        while(_cueParts[1].size() > 0 && _cueParts[1].back() == '0')
            _cueParts[1].chop(1);

        if (_cueParts[1].isEmpty())
            _cueParts.removeLast();
    }
}

QDataStream& operator>>(QDataStream& stream, MscCueId& cueId)
{
    QString s;
    quint8 b;
    stream >> b;

    while(b != 0 && b != MscMessage::SYSEX_END)
    {
        s.append(b);
        stream >> b;
    }

    cueId = s;

    return stream;
}

QDataStream& operator<<(QDataStream& stream, const MscCueId& cueId)
{
    std::string s = cueId.toString().toStdString();
    stream.writeBytes(s.data(), (uint)s.length());
    return stream;
}

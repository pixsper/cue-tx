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

MscCueId::MscCueId(const char* cuePart...)
{
    va_list args;
    va_start(args, cuePart);

    while (const char* v = va_arg(args, const char*))
        _cueParts.append(v);

    va_end(args);
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

QDataStream& operator>>(QDataStream& stream, MscCueId& cueId)
{
    char* cueIdRaw;
    stream >> cueIdRaw;
    cueId = cueIdRaw;
    delete [] cueIdRaw;

    return stream;
}

QDataStream& operator<<(QDataStream& stream, MscCueId& cueId)
{
    return stream << cueId.toString();
}

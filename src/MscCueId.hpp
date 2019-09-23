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

#include <QRegularExpression>
#include <QDataStream>
#include "nonstd/optional.hpp"

class MscCueId
{
    static const QRegularExpression VALID_CUEID;

    QStringList _cueParts;

public:
    static bool isValidMscCue(const QString& mscCue);

    MscCueId();
    MscCueId(const QString& cueString);
    MscCueId(std::initializer_list<const char*> cueParts);

    bool operator==(const MscCueId& rhs) const;
    bool operator!=(const MscCueId& rhs) const;

    bool isValid() const;

    QString toString() const;

    void removeZeroPadding();
};

QDataStream& operator>>(QDataStream& stream, MscCueId& cueId);
QDataStream& operator>>(QDataStream& stream, nonstd::optional<MscCueId>& cueId);
QDataStream& operator<<(QDataStream& stream, const MscCueId& cueId);

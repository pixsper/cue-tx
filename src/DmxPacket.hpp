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

using DmxValue = quint8;

const size_t DMX_UNIVERSE_LENGTH = 512;

struct DmxPacket
{
    DmxValue _data[DMX_UNIVERSE_LENGTH];



    DmxPacket();
    virtual ~DmxPacket() { }

    virtual bool toByteArray(QByteArray& array) const = 0;
};

struct ArtNetPacket : DmxPacket
{
    static bool fromByteArray(const QByteArray& array, ArtNetPacket& message);

    bool toByteArray(QByteArray& array) const override;
};

struct SAcnPacket : DmxPacket
{
    static bool fromByteArray(const QByteArray& array, SAcnPacket& message);

    bool toByteArray(QByteArray& array) const override;
};

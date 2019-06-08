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

#include <QObject>
#include "../MscMessage.hpp"

class QCueTxOutputService : public QObject
{
    Q_OBJECT
protected:
    explicit QCueTxOutputService(QObject *parent)
        : QObject(parent)
    {

    }

public:
    virtual bool start(const QVariantMap& configuration) = 0;
    virtual void stop() = 0;

public slots:
    virtual void sendMessage(const MscMessage& message) = 0;
};

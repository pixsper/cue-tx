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
#include "ServiceTypes.h"
#include "ServiceFactory.h"
#include "input_services/QCueTxInputService.h"
#include "output_services/QCueTxOutputService.h"

class QMscRouter : public QObject
{
    Q_OBJECT

    InputServiceType _inputServiceType;
    QCueTxInputService* _inputService;

    OutputServiceType _outputServiceType;
    QCueTxOutputService* _outputService;

public:
    explicit QMscRouter(QObject *parent = nullptr);

    void start(const QVariantMap& inputConfiguration, const QVariantMap& outputConfiguration);

    void stop();
};

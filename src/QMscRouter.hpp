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
#include "ServiceTypes.hpp"
#include "ServiceFactory.hpp"
#include "input_services/QCueTxInputService.hpp"
#include "output_services/QCueTxOutputService.hpp"

class QMscRouter : public QObject
{
    Q_OBJECT

    QCueTxInputService* _inputService;
    QCueTxOutputService* _outputService;

public:
    static const QString SETTINGS_INPUTSERVICETYPE_KEY;
    static const InputServiceType SETTINGS_INPUTSERVICETYPE_DEFAULT = InputServiceType::MidiMsc;
    static const QString SETTINGS_OUTPUTSERVICETYPE_KEY;
    static const OutputServiceType SETTINGS_OUTPUTSERVICETYPE_DEFAULT = OutputServiceType::None;
    static const QString SETTINGS_INPUTSERVICESETTINGS_KEY;
    static const QString SETTINGS_OUTPUTSERVICESETTINGS_KEY;

    explicit QMscRouter(QObject *parent = nullptr);


    bool start(const QVariantMap& settings);

    void stop();
};

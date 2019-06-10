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

#include "QMscRouter.hpp"

const QString QMscRouter::SETTINGS_INPUTSERVICETYPE_KEY = "inputServiceType";
const QString QMscRouter::SETTINGS_OUTPUTSERVICETYPE_KEY = "outputServiceType";
const QString QMscRouter::SETTINGS_INPUTSERVICESETTINGS_KEY = "inputServiceSettings";
const QString QMscRouter::SETTINGS_OUTPUTSERVICESETTINGS_KEY = "outputServiceSettings";

QMscRouter::QMscRouter(QObject* parent)
    : QObject(parent),
      _inputService(nullptr),
      _outputService(nullptr)
{

}

bool QMscRouter::start(const QVariantMap& settings)
{
    InputServiceType inputServiceType = static_cast<InputServiceType>(
                settings.value(SETTINGS_INPUTSERVICETYPE_KEY, QVariant(static_cast<int>(SETTINGS_INPUTSERVICETYPE_DEFAULT))).toInt());
    OutputServiceType outputServiceType = static_cast<OutputServiceType>(
                settings.value(SETTINGS_OUTPUTSERVICETYPE_KEY, QVariant(static_cast<int>(SETTINGS_OUTPUTSERVICETYPE_DEFAULT))).toInt());

    _inputService = ServiceFactory::createInputService(inputServiceType, this);
    _outputService = ServiceFactory::createOutputService(outputServiceType, this);

    if (_inputService == nullptr || _outputService == nullptr)
        return false;

    connect(_inputService, &QCueTxInputService::messageReceived, _outputService, &QCueTxOutputService::sendMessage);


    if (_inputService != nullptr)
    {
        QVariantMap inputSettings = settings.value(SETTINGS_INPUTSERVICESETTINGS_KEY, QVariant(_inputService->defaultSettings())).toMap();
        _inputService->start(inputSettings);
    }

    if (_outputService != nullptr)
    {
        QVariantMap outputSettings = settings.value(SETTINGS_OUTPUTSERVICESETTINGS_KEY, QVariant(_outputService->defaultSettings())).toMap();
        _outputService->start(outputSettings);
    }

    return true;
}

void QMscRouter::stop()
{
    if (_inputService != nullptr && _outputService != nullptr)
        disconnect(_inputService, &QCueTxInputService::messageReceived, _outputService, &QCueTxOutputService::sendMessage);

    if (_inputService != nullptr)
    {
        _inputService->stop();
        delete _inputService;
        _inputService = nullptr;
    }

    if (_outputService != nullptr)
    {
        _outputService->stop();
        delete _outputService;
        _outputService = nullptr;
    }
}

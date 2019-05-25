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

#include "QMscRouter.h"

QMscRouter::QMscRouter(QObject* parent)
    : QObject(parent),
      _inputServiceType(InputServiceType::None),
      _inputService(nullptr),
      _outputServiceType(OutputServiceType::None),
      _outputService(nullptr)
{

}

void QMscRouter::start(const QVariantMap& inputConfiguration, const QVariantMap& outputConfiguration)
{
    _inputService = ServiceFactory::createInputService(this, _inputServiceType);
    _outputService = ServiceFactory::createOutputService(this, _outputServiceType);

    if (_inputService == nullptr || _outputService == nullptr)
        return;

    connect(_inputService, &QCueTxInputService::messageReceived, _outputService, &QCueTxOutputService::sendMessage);

    _inputService->start(inputConfiguration);
    _outputService->start(outputConfiguration);
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

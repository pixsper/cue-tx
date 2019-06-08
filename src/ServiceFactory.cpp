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

#include "ServiceFactory.hpp"
#include "input_services/QMidiMscInputService.hpp"
#include "input_services/QGmaMscInputService.hpp"
#include "input_services/QEosOscInputService.hpp"
#include "output_services/QMidiMscOutputService.hpp"
#include "output_services/QGmaMscOutputService.hpp"
#include "output_services/QDisguiseOscOutputService.hpp"
#include "output_services/QQLabOscOutputService.hpp"

namespace ServiceFactory
{
    QCueTxInputService* createInputService(QObject* parent, InputServiceType serviceType)
    {
        switch (serviceType)
        {
            case InputServiceType::None:
                return nullptr;

            case InputServiceType::MidiMsc:
                return new QMidiMscInputService(parent);

            case InputServiceType::GmaMsc:
                return new QGmaMscInputService(parent);

            case InputServiceType::EosOsc:
                return new QEosOscInputService(parent);

            default:
                Q_ASSERT(false); // Unknown input service type
                return nullptr;
        }
    }

    QCueTxOutputService* createOutputService(QObject* parent, OutputServiceType serviceType)
    {
        switch (serviceType)
        {
            case OutputServiceType::None:
                return nullptr;

            case OutputServiceType::MidiMsc:
                return new QMidiMscOutputService(parent);

            case OutputServiceType::GmaMsc:
                return new QGmaMscOutputService(parent);

            case OutputServiceType::DisguiseOsc:
                return new QDisguiseOscOutputService(parent);

            case OutputServiceType::QLabOsc:
                return new QQLabOscOutputService(parent);

            default:
                Q_ASSERT(false); // Unknown output service type
                return nullptr;
        }
    }
}

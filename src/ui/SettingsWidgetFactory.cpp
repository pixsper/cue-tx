#include "SettingsWidgetFactory.hpp"

#include "input_services/QMidiMscInputServiceSettingsWidget.hpp"
#include "input_services/QGmaMscInputServiceSettingsWidget.hpp"
#include "input_services/QEosOscInputServiceSettingsWidget.hpp"
#include "output_services/QDisguiseOscOutputServiceSettingsWidget.hpp"
#include "output_services/QMidiMscOutputServiceSettingsWidget.hpp"
#include "output_services/QGmaMscOutputServiceSettingsWidget.hpp"
#include "output_services/QQLabOscOutputServiceSettingsWidget.hpp"

QSettingsWidget* SettingsWidgetFactory::createInputSettingsWidget(InputServiceType serviceType, QWidget* parent)
{
    switch (serviceType)
    {
        case InputServiceType::None:
            return nullptr;

        case InputServiceType::MidiMsc:
            return new QMidiMscInputServiceSettingsWidget(parent);

        case InputServiceType::GmaMsc:
            return new QGmaMscInputServiceSettingsWidget(parent);

        case InputServiceType::EosOsc:
            return new QEosOscInputServiceSettingsWidget(parent);

        case InputServiceType::CustomOsc:
            return nullptr;

        default:
            Q_ASSERT(false); // Unknown input service type
            return nullptr;
    }
}

QSettingsWidget* SettingsWidgetFactory::createOutputSettingsWidget(OutputServiceType serviceType, QWidget* parent)
{
    switch (serviceType)
    {
        case OutputServiceType::None:
            return nullptr;

        case OutputServiceType::MidiMsc:
            return new QMidiMscOutputServiceSettingsWidget(parent);

        case OutputServiceType::GmaMsc:
            return new QGmaMscOutputServiceSettingsWidget(parent);

        case OutputServiceType::DisguiseOsc:
            return new QDisguiseOscOutputServiceSettingsWidget(parent);

        case OutputServiceType::QLabOsc:
            return new QQLabOscOutputServiceSettingsWidget(parent);

        default:
            Q_ASSERT(false); // Unknown output service type
            return nullptr;
    }
}

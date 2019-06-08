#pragma once

#include "../ServiceTypes.hpp"
#include "QSettingsWidget.hpp"

namespace SettingsWidgetFactory
{
    QSettingsWidget* createInputSettingsWidget(InputServiceType serviceType, QWidget* parent = nullptr);
    QSettingsWidget* createOutputSettingsWidget(OutputServiceType serviceType, QWidget* parent = nullptr);
}

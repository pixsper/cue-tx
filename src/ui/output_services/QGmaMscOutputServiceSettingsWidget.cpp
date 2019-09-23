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

#include "QGmaMscOutputServiceSettingsWidget.hpp"
#include "ui_QGmaMscOutputServiceSettingsWidget.h"
#include "../../output_services/QGmaMscOutputService.hpp"

QGmaMscOutputServiceSettingsWidget::QGmaMscOutputServiceSettingsWidget(QWidget *parent) :
    QSettingsWidget(parent),
    ui(new Ui::QGmaMscOutputServiceSettingsWidget)
{
    ui->setupUi(this);

    ui->spinBoxOutputUdpPort->setMinimum(QGmaMscOutputService::SETTINGS_HOSTPORT_MIN);
    ui->spinBoxOutputUdpPort->setMaximum(QGmaMscOutputService::SETTINGS_HOSTPORT_MAX);

    connect(ui->spinBoxOutputUdpPort, qOverload<int>(&QSpinBox::valueChanged),
            [&](int) { settingsChanged(getSettings()); });

    connect(ui->checkBoxAddZeroPadding, &QCheckBox::stateChanged,
            [&](int) { settingsChanged(getSettings()); });
}

QGmaMscOutputServiceSettingsWidget::~QGmaMscOutputServiceSettingsWidget()
{
    delete ui;
}

void QGmaMscOutputServiceSettingsWidget::setSettings(const QVariantMap& settings)
{
    {
        const auto it = settings.find(QGmaMscOutputService::SETTINGS_HOSTPORT_KEY);
        if (it != settings.end())
            ui->spinBoxOutputUdpPort->setValue(it.value().toInt());
    }

    {
        const auto it = settings.find(QGmaMscOutputService::SETTINGS_ADDZEROPADDING_KEY);
        if (it != settings.end())
            ui->checkBoxAddZeroPadding->setChecked(it.value().toBool());
    }
}

QVariantMap QGmaMscOutputServiceSettingsWidget::getSettings()
{
    return QVariantMap
    {
        { QGmaMscOutputService::SETTINGS_HOSTPORT_KEY, ui->spinBoxOutputUdpPort->value() },
        { QGmaMscOutputService::SETTINGS_ADDZEROPADDING_KEY, ui->checkBoxAddZeroPadding->isChecked() }
    };
}

void QGmaMscOutputServiceSettingsWidget::setDefaultSettings()
{
    ui->spinBoxOutputUdpPort->setValue(QGmaMscOutputService::SETTINGS_HOSTPORT_DEFAULT);
    ui->checkBoxAddZeroPadding->setChecked(QGmaMscOutputService::SETTINGS_ADDZEROPADDING_DEFAULT);
}

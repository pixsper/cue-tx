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

#include "QEosOscInputServiceSettingsWidget.hpp"
#include "ui_QEosOscInputServiceSettingsWidget.h"
#include "../../input_services/QEosOscInputService.hpp"

QEosOscInputServiceSettingsWidget::QEosOscInputServiceSettingsWidget(QWidget *parent) :
    QSettingsWidget(parent),
    ui(new Ui::QEosOscInputServiceSettingsWidget)
{
    ui->setupUi(this);

    QRegExp rx(R"(^(?:(?:\d|[1-9]\d|1\d\d|2[0-4]\d|25[0-5])\.){3}(?:\d|[1-9]\d|1\d\d|2[0-4]\d|25[0-5])$)");
    QValidator* validator = new QRegExpValidator(rx, this);
    ui->lineEditIpAddress->setValidator(validator);

    ui->spinBoxInputUdpPort->setMaximum(QEosOscInputService::SETTINGS_HOSTPORT_MIN);
    ui->spinBoxInputUdpPort->setMaximum(QEosOscInputService::SETTINGS_HOSTPORT_MAX);

    connect(ui->comboBoxMode, qOverload<int>(&QComboBox::currentIndexChanged),
            [&](int)
    {
        ui->lineEditIpAddress->setEnabled(ui->comboBoxMode->currentIndex() != (int)QEosOscInputService::ReceiveMode::Udp);
        ui->spinBoxInputUdpPort->setEnabled(ui->comboBoxMode->currentIndex() == (int)QEosOscInputService::ReceiveMode::Udp);
        settingsChanged(getSettings());
    });

    connect(ui->lineEditIpAddress, &QLineEdit::textChanged,
            [&](const QString&) { settingsChanged(getSettings()); });

    connect(ui->spinBoxInputUdpPort, qOverload<int>(&QSpinBox::valueChanged),
            [&](int) { settingsChanged(getSettings()); });
}

QEosOscInputServiceSettingsWidget::~QEosOscInputServiceSettingsWidget()
{
    delete ui;
}

void QEosOscInputServiceSettingsWidget::setSettings(const QVariantMap& settings)
{
    {
        const auto it = settings.find(QEosOscInputService::SETTINGS_MODE_KEY);
        if (it != settings.end())
            ui->comboBoxMode->setCurrentIndex(it.value().toInt());
    }

    {
        const auto it = settings.find(QEosOscInputService::SETTINGS_HOSTIP_KEY);
        if (it != settings.end())
            ui->lineEditIpAddress->setText(it.value().toString());
    }

    {
        const auto it = settings.find(QEosOscInputService::SETTINGS_HOSTPORT_KEY);
        if (it != settings.end())
            ui->spinBoxInputUdpPort->setValue(it.value().toInt());
    }

    ui->lineEditIpAddress->setEnabled(ui->comboBoxMode->currentIndex() != (int)QEosOscInputService::ReceiveMode::Udp);
    ui->spinBoxInputUdpPort->setEnabled(ui->comboBoxMode->currentIndex() == (int)QEosOscInputService::ReceiveMode::Udp);
}

QVariantMap QEosOscInputServiceSettingsWidget::getSettings()
{
    return QVariantMap
    {
        { QEosOscInputService::SETTINGS_MODE_KEY, ui->comboBoxMode->currentIndex() },
        { QEosOscInputService::SETTINGS_HOSTIP_KEY, ui->lineEditIpAddress->text() },
        { QEosOscInputService::SETTINGS_HOSTPORT_KEY, ui->spinBoxInputUdpPort->value() }
    };
}

void QEosOscInputServiceSettingsWidget::setDefaultSettings()
{
    ui->comboBoxMode->setCurrentIndex((int)QEosOscInputService::SETTINGS_MODE_DEFAULT);
    ui->lineEditIpAddress->setText(QEosOscInputService::SETTINGS_HOSTIP_DEFAULT);
    ui->spinBoxInputUdpPort->setValue(QEosOscInputService::SETTINGS_HOSTPORT_DEFAULT);
}

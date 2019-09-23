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

#include "QMidiMscInputServiceSettingsWidget.hpp"
#include "ui_QMidiMscInputServiceSettingsWidget.h"
#include "../../input_services/QMidiMscInputService.hpp"
#include "../../output_services/QMidiMscOutputService.hpp"
#include "../../qrtmidi/QRtMidiIn.hpp"

QMidiMscInputServiceSettingsWidget::QMidiMscInputServiceSettingsWidget(QWidget *parent) :
    QSettingsWidget(parent),
    ui(new Ui::QMidiMscInputServiceSettingsWidget),
    _currentMidiPortName("")
{
    ui->setupUi(this);

#ifndef Q_OS_MAC
    ui->checkBoxUseVirtualPort->setVisible(false);
#endif

    updateMidiPortList();

    connect(ui->checkBoxUseVirtualPort, qOverload<int>(&QCheckBox::stateChanged),
            [&](int)
    {
        ui->comboBoxMidiInPort->setEnabled(!ui->checkBoxUseVirtualPort->isChecked());
        settingsChanged(getSettings());
    });

    connect(ui->comboBoxMidiInPort, qOverload<int>(&QComboBox::currentIndexChanged),
            [&](int) { settingsChanged(getSettings()); });
}

QMidiMscInputServiceSettingsWidget::~QMidiMscInputServiceSettingsWidget()
{
    delete ui;
}

void QMidiMscInputServiceSettingsWidget::refresh()
{
    updateMidiPortList();
    selectCurrentMidiPort();
}

void QMidiMscInputServiceSettingsWidget::setSettings(const QVariantMap& settings)
{
    updateMidiPortList();

    {
        const auto it = settings.find(QMidiMscInputService::SETTINGS_ISVIRTUAL_KEY);
        if (it != settings.end())
            ui->checkBoxUseVirtualPort->setChecked(it->toBool());
    }

    {
        const auto it = settings.find(QMidiMscInputService::SETTINGS_PORTNAME_KEY);
        if (it != settings.end())
        {
            _currentMidiPortName = it->toString();
            selectCurrentMidiPort();
        }
    }
}

QVariantMap QMidiMscInputServiceSettingsWidget::getSettings()
{
    return QVariantMap
    {
        { QMidiMscInputService::SETTINGS_ISVIRTUAL_KEY, ui->checkBoxUseVirtualPort->isChecked() },
        { QMidiMscInputService::SETTINGS_PORTNAME_KEY, ui->comboBoxMidiInPort->currentData() }
    };
}

void QMidiMscInputServiceSettingsWidget::setDefaultSettings()
{
    updateMidiPortList();

    ui->checkBoxUseVirtualPort->setChecked(QMidiMscInputService::SETTINGS_ISVIRTUAL_DEFAULT);
    ui->comboBoxMidiInPort->setCurrentIndex(0);
}

void QMidiMscInputServiceSettingsWidget::updateMidiPortList()
{
    ui->comboBoxMidiInPort->clear();

    QRtMidiIn midiIn;
    auto ports = midiIn.getMidiInPorts();

    ui->comboBoxMidiInPort->addItem("None", QString(""));

    for(const auto& port : ports)
    {
        if (port != QMidiMscOutputService::VIRTUAL_MIDI_PORT_NAME)
            ui->comboBoxMidiInPort->addItem(port, port);
    }
}

void QMidiMscInputServiceSettingsWidget::selectCurrentMidiPort()
{
    int index = ui->comboBoxMidiInPort->findData(_currentMidiPortName);
    if (index == -1)
        ui->comboBoxMidiInPort->setCurrentIndex(0);
    else
        ui->comboBoxMidiInPort->setCurrentIndex(index);
}

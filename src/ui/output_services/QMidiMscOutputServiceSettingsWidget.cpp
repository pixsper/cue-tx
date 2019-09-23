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

#include "QMidiMscOutputServiceSettingsWidget.hpp"
#include "ui_QMidiMscOutputServiceSettingsWidget.h"
#include "../../output_services/QMidiMscOutputService.hpp"
#include "../../input_services/QMidiMscInputService.hpp"
#include "../../qrtmidi/QRtMidiOut.hpp"

QMidiMscOutputServiceSettingsWidget::QMidiMscOutputServiceSettingsWidget(QWidget *parent) :
    QSettingsWidget(parent),
    ui(new Ui::QMidiMscOutputServiceSettingsWidget),
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
        ui->comboBoxMidiOutPort->setEnabled(!ui->checkBoxUseVirtualPort->isChecked());
        settingsChanged(getSettings());
    });

    connect(ui->comboBoxMidiOutPort, qOverload<int>(&QComboBox::currentIndexChanged),
            [&](int) { settingsChanged(getSettings()); });
}

QMidiMscOutputServiceSettingsWidget::~QMidiMscOutputServiceSettingsWidget()
{
    delete ui;
}

void QMidiMscOutputServiceSettingsWidget::refresh()
{
    updateMidiPortList();
    selectCurrentMidiPort();
}

void QMidiMscOutputServiceSettingsWidget::setSettings(const QVariantMap& settings)
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

QVariantMap QMidiMscOutputServiceSettingsWidget::getSettings()
{
    return QVariantMap
    {
        { QMidiMscInputService::SETTINGS_ISVIRTUAL_KEY, ui->checkBoxUseVirtualPort->isChecked() },
        { QMidiMscInputService::SETTINGS_PORTNAME_KEY, ui->comboBoxMidiOutPort->currentData() }
    };
}

void QMidiMscOutputServiceSettingsWidget::setDefaultSettings()
{
    updateMidiPortList();

    ui->checkBoxUseVirtualPort->setChecked(QMidiMscInputService::SETTINGS_ISVIRTUAL_DEFAULT);
    ui->comboBoxMidiOutPort->setCurrentIndex(0);
}

void QMidiMscOutputServiceSettingsWidget::updateMidiPortList()
{
    ui->comboBoxMidiOutPort->clear();

    QRtMidiOut midiOut;
    auto ports = midiOut.getMidiOutPorts();

    ui->comboBoxMidiOutPort->addItem("None", QString(""));

    for(const auto& port : ports)
    {
        if (port != QMidiMscInputService::VIRTUAL_MIDI_PORT_NAME)
            ui->comboBoxMidiOutPort->addItem(port, port);
    }
}

void QMidiMscOutputServiceSettingsWidget::selectCurrentMidiPort()
{
    int index = ui->comboBoxMidiOutPort->findData(_currentMidiPortName);
    if (index == -1)
        ui->comboBoxMidiOutPort->setCurrentIndex(0);
    else
        ui->comboBoxMidiOutPort->setCurrentIndex(index);
}

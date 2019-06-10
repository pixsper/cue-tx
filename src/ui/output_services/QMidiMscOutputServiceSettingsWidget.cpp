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
#include "../../qrtmidi/QRtMidiOut.hpp"

QMidiMscOutputServiceSettingsWidget::QMidiMscOutputServiceSettingsWidget(QWidget *parent) :
    QSettingsWidget(parent),
    ui(new Ui::QMidiMscOutputServiceSettingsWidget)
{
    ui->setupUi(this);

    updateMidiPortList();
}

QMidiMscOutputServiceSettingsWidget::~QMidiMscOutputServiceSettingsWidget()
{
    delete ui;
}

void QMidiMscOutputServiceSettingsWidget::refresh()
{
    updateMidiPortList();
}

void QMidiMscOutputServiceSettingsWidget::setSettings(const QVariantMap& settings)
{

}

QVariantMap QMidiMscOutputServiceSettingsWidget::getSettings()
{
    return QVariantMap();
}

void QMidiMscOutputServiceSettingsWidget::setDefaultSettings()
{

}

void QMidiMscOutputServiceSettingsWidget::updateMidiPortList()
{
    ui->comboBoxMidiOutPort->clear();

    QRtMidiOut midiOut;
    auto ports = midiOut.getMidiOutPorts();

#ifdef Q_OS_MAC

    ui->comboBoxMidiOutPort->addItem("Virtual Port (from Cue TX)", QVariant(0))

    for(const auto& port : ports)
        ui->comboBoxMidiOutPort->addItem(port, port);

#else

    if (ports.count() == 0)
    {
        ui->comboBoxMidiOutPort->setEnabled(false);
        ui->comboBoxMidiOutPort->addItem("No MIDI Out Ports available", QVariant(-1));
    }
    else
    {
        for(const auto& port : ports)
            ui->comboBoxMidiOutPort->addItem(port, port);
    }

#endif


}

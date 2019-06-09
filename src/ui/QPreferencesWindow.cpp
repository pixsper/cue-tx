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

#include "QPreferencesWindow.hpp"
#include "ui_QPreferencesWindow.h"
#include "SettingsWidgetFactory.hpp"

#include <QFormLayout>


const QList<QPair<InputServiceType, QString>> QPreferencesWindow::inputServiceOptionList
{
    { InputServiceType::MidiMsc, "MSC - MIDI" },
    { InputServiceType::GmaMsc, "MSC - GrandMA2 via Ethernet" },
    { InputServiceType::EosOsc, "OSC - ETC Eos" },
    { InputServiceType::CustomOsc, "OSC - Custom" }
};

const QList<QPair<OutputServiceType, QString>> QPreferencesWindow::outputServiceOptionList
{
    { OutputServiceType::None, "None (Log Only)" },
    { OutputServiceType::MidiMsc, "MSC - MIDI" },
    { OutputServiceType::GmaMsc, "MSC - GrandMA2 via Ethernet" },
    { OutputServiceType::EosOsc, "OSC - ETC Eos" },
    { OutputServiceType::QLabOsc, "OSC - QLab" },
    { OutputServiceType::DisguiseOsc, "OSC - disguise/d3" },
    { OutputServiceType::CustomOsc, "OSC - Custom" },
};

QPreferencesWindow::QPreferencesWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QPreferencesWindow)
{
    ui->setupUi(this);

    for(const auto& item : inputServiceOptionList)
        ui->comboBoxInput->addItem(item.second, static_cast<int>(item.first));

    for(const auto& item : outputServiceOptionList)
        ui->comboBoxOutput->addItem(item.second, static_cast<int>(item.first));

    QMetaObject::connectSlotsByName(this);
}

QPreferencesWindow::~QPreferencesWindow()
{
    delete ui;
}

void QPreferencesWindow::refreshAndShow()
{
    if (ui->groupBoxInputSettings->layout()->children().count() != 0)
        reinterpret_cast<QSettingsWidget*>(ui->groupBoxInputSettings->layout()->children().front())->refresh();

    if (ui->groupBoxOutputSettings->layout()->children().count() != 0)
        reinterpret_cast<QSettingsWidget*>(ui->groupBoxOutputSettings->layout()->children().front())->refresh();

    show();
}

void QPreferencesWindow::updateVisibility()
{
    ui->groupBoxInputSettings->setVisible(ui->groupBoxInputSettings->layout()->count() != 0);
    ui->groupBoxOutputSettings->setVisible(ui->groupBoxOutputSettings->layout()->count() != 0);
}

void QPreferencesWindow::updateInputSettingsWidget()
{
    QLayoutItem* child;
    while ((child = ui->groupBoxInputSettings->layout()->takeAt(0)) != nullptr)
    {
        delete child->widget();
        delete child;
    }

    auto pair = inputServiceOptionList[ui->comboBoxInput->currentIndex()];
    auto inputSettingsWidget = SettingsWidgetFactory::createInputSettingsWidget(pair.first);

    if (inputSettingsWidget != nullptr)
        ui->groupBoxInputSettings->layout()->addWidget(inputSettingsWidget);
}

void QPreferencesWindow::updateOutputSettingsWidget()
{
    QLayoutItem* child;
    while ((child = ui->groupBoxOutputSettings->layout()->takeAt(0)) != nullptr)
    {
        delete child->widget();
        delete child;
    }

    auto pair = outputServiceOptionList[ui->comboBoxOutput->currentIndex()];
    auto outputSettingsWidget = SettingsWidgetFactory::createOutputSettingsWidget(pair.first);

    if (outputSettingsWidget != nullptr)
        ui->groupBoxOutputSettings->layout()->addWidget(outputSettingsWidget);
}

void QPreferencesWindow::on_comboBoxInput_currentIndexChanged()
{
    updateInputSettingsWidget();
    updateVisibility();
}

void QPreferencesWindow::on_comboBoxOutput_currentIndexChanged()
{
    updateOutputSettingsWidget();
    updateVisibility();
}

void QPreferencesWindow::on_buttonBox_clicked(QAbstractButton* button)
{

}

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
#include "../QCueRouter.hpp"

#include <QPushButton>


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

    connect(ui->comboBoxInput, qOverload<int>(&QComboBox::currentIndexChanged), this, &QPreferencesWindow::onComboBoxInputCurrentIndexChanged);
    connect(ui->comboBoxOutput, qOverload<int>(&QComboBox::currentIndexChanged), this, &QPreferencesWindow::onComboBoxOutputCurrentIndexChanged);
    connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &QPreferencesWindow::onOkButtonClicked);
    connect(ui->buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &QPreferencesWindow::onApplyButtonClicked);
    connect(ui->buttonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &QPreferencesWindow::onCancelButtonClicked);

    updateInputSettingsWidget();
    updateOutputSettingsWidget();
    updateVisibility();
}

QPreferencesWindow::~QPreferencesWindow()
{
    delete ui;
}

void QPreferencesWindow::refreshAndShow()
{
    if (_inputSettingsWidget)
        _inputSettingsWidget->refresh();

    if (_outputSettingsWidget)
        _outputSettingsWidget->refresh();

    show();
}

void QPreferencesWindow::setSettings(const QVariantMap& settings)
{
    {
         auto it = settings.find(QCueRouter::SETTINGS_INPUTSERVICETYPE_KEY);
         if (it != settings.end())
         {
             InputServiceType inputServiceType = static_cast<InputServiceType>(it->toInt());

             for(int i = 0; i < inputServiceOptionList.size(); ++i)
             {
                 if (inputServiceOptionList[i].first == inputServiceType)
                 {
                     ui->comboBoxInput->setCurrentIndex(i);
                     break;
                 }
             }
         }
    }

    updateInputSettingsWidget();

    if (_inputSettingsWidget)
    {
        auto it = settings.find(QCueRouter::SETTINGS_INPUTSERVICESETTINGS_KEY);
        if (it != settings.end())
            _inputSettingsWidget->setSettings(it->toMap());
    }

    {
        auto it = settings.find(QCueRouter::SETTINGS_OUTPUTSERVICETYPE_KEY);
        if (it != settings.end())
        {
            OutputServiceType outputServiceType = static_cast<OutputServiceType>(it->toInt());

            for(int i = 0; i < outputServiceOptionList.size(); ++i)
            {
                if (outputServiceOptionList[i].first == outputServiceType)
                {
                    ui->comboBoxOutput->setCurrentIndex(i);
                    break;
                }
            }
        }
    }

    updateOutputSettingsWidget();

    if (_outputSettingsWidget)
    {
        auto it = settings.find(QCueRouter::SETTINGS_OUTPUTSERVICESETTINGS_KEY);
        if (it != settings.end())
            _outputSettingsWidget->setSettings(it->toMap());
    }

    setDirty(false);
}

QVariantMap QPreferencesWindow::getSettings()
{
    auto settings = QVariantMap
    {
        { QCueRouter::SETTINGS_INPUTSERVICETYPE_KEY, ui->comboBoxInput->currentData() },
        { QCueRouter::SETTINGS_OUTPUTSERVICETYPE_KEY, ui->comboBoxOutput->currentData() }
    };

    if (_inputSettingsWidget)
        settings[QCueRouter::SETTINGS_INPUTSERVICESETTINGS_KEY] = _inputSettingsWidget->getSettings();

    if (_outputSettingsWidget)
        settings[QCueRouter::SETTINGS_OUTPUTSERVICESETTINGS_KEY] = _outputSettingsWidget->getSettings();

    return settings;
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

    _inputSettingsWidget = nullptr;

    auto pair = inputServiceOptionList[ui->comboBoxInput->currentIndex()];
    _inputSettingsWidget = SettingsWidgetFactory::createInputSettingsWidget(pair.first);

    if (_inputSettingsWidget != nullptr)
    {
        _inputSettingsWidget->setDefaultSettings();
        connect(_inputSettingsWidget, &QSettingsWidget::settingsChanged,
                [&](const QVariantMap&) { setDirty(true); });
        ui->groupBoxInputSettings->layout()->addWidget(_inputSettingsWidget);
    }
}

void QPreferencesWindow::updateOutputSettingsWidget()
{
    QLayoutItem* child;
    while ((child = ui->groupBoxOutputSettings->layout()->takeAt(0)) != nullptr)
    {
        delete child->widget();
        delete child;
    }

     _outputSettingsWidget = nullptr;

    auto pair = outputServiceOptionList[ui->comboBoxOutput->currentIndex()];
    _outputSettingsWidget = SettingsWidgetFactory::createOutputSettingsWidget(pair.first);

    if (_outputSettingsWidget != nullptr)
    {
        _outputSettingsWidget->setDefaultSettings();
        connect(_outputSettingsWidget, &QSettingsWidget::settingsChanged,
                [&](const QVariantMap&) { setDirty(true); });
        ui->groupBoxOutputSettings->layout()->addWidget(_outputSettingsWidget);
    }
}

void QPreferencesWindow::setDirty(bool isDirty)
{
    _isDirty = isDirty;
    ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(_isDirty);
}

void QPreferencesWindow::onComboBoxInputCurrentIndexChanged(int)
{
    setDirty(true);
    updateInputSettingsWidget();
    updateVisibility();
}

void QPreferencesWindow::onComboBoxOutputCurrentIndexChanged(int)
{
    setDirty(true);
    updateOutputSettingsWidget();
    updateVisibility();
}

void QPreferencesWindow::onOkButtonClicked()
{
    emit settingsChanged(getSettings());
    setDirty(false);
    close();
}

void QPreferencesWindow::onApplyButtonClicked()
{
    emit settingsChanged(getSettings());
    setDirty(false);
}

void QPreferencesWindow::onCancelButtonClicked()
{
    close();
    setDirty(false);
}

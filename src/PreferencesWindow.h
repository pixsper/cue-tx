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

#pragma once

#include <QtGlobal>
#include <QVBoxLayout>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QGroupBox>
#include <QSettings>
#include <QVariant>

class PreferencesWindow : public QWidget
{
    Q_OBJECT

    QVBoxLayout _mainLayout;

    QComboBox _inputSourceComboBox;
    QComboBox _inputMidiPortComboBox;
    QSpinBox _inputUdpPortSpinBox;
    QCheckBox _inputGmaStripTrailingZerosCheckBox;

    QComboBox _outputDestinationComboBox;

    QGroupBox _inputMidiMscControlsGroupBox;
    QGroupBox _inputGmaMscControlsGroupBox;

    QGroupBox _outputMidiMscControlsGroupBox;
    QGroupBox _outputGmaMscControlsGroupBox;
    QGroupBox _outputQLabOscControlsGroupBox;
    QGroupBox _outputDisguiseOscControlsGroupBox;

public:
    explicit PreferencesWindow(QWidget *parent = nullptr);

private:
    void loadSettings()
    {
        QSettings s;

        _inputSourceComboBox.setCurrentIndex(s.value("inputSource", 0).value<int>());

    }

    void saveSettings()
    {
        QSettings s;

        s.setValue("inputSource", _inputSourceComboBox.currentIndex());
    }

    void updateHiddenControls()
    {
        _inputMidiMscControlsGroupBox.setHidden(_inputSourceComboBox.currentIndex() != 0);
        _inputMidiMscControlsGroupBox.setEnabled(_inputSourceComboBox.currentIndex() == 0);

        _inputGmaMscControlsGroupBox.setHidden(_inputSourceComboBox.currentIndex() != 1);
        _inputGmaMscControlsGroupBox.setEnabled(_inputSourceComboBox.currentIndex() == 1);
    }

private slots:
    void onInputSourceChanged(int value)
    {
        updateHiddenControls();
        saveSettings();
    }
};

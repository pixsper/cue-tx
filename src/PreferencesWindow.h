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

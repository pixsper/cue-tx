#pragma once

#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>

class PreferencesWindow : public QWidget
{
    Q_OBJECT

    QComboBox _inputSourceComboBox;
    QComboBox _inputMidiPortComboBox;
    QSpinBox _inputUdpPortSpinBox;
    QCheckBox _inputGmaStripTrailingZerosCheckBox;

public:
    explicit PreferencesWindow(QWidget *parent = nullptr);
};

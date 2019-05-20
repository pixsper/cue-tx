#include "preferenceswindow.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>

#include <RtMidi.h>

PreferencesWindow::PreferencesWindow(QWidget *parent) :
    QWidget(parent)
{
    setWindowFlags(Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::CustomizeWindowHint);
    setWindowTitle("Cue TX");

    QVBoxLayout* mainLayout = new QVBoxLayout;

    QFormLayout* inputSourceFormLayout = new QFormLayout;

    _inputSourceComboBox.addItem("MSC (MIDI)");
    _inputSourceComboBox.addItem("MSC (GrandMA2 Ethernet)");
    inputSourceFormLayout->addRow(new QLabel("Input Source"), &_inputSourceComboBox);

    mainLayout->addLayout(inputSourceFormLayout);


    QGroupBox* inputSettingsGroupBox = new QGroupBox("Input Settings");
    QFormLayout* inputSettingsFormLayout = new QFormLayout;

    _inputMidiPortComboBox.addItem("CueTX Virtual Port");
    _inputMidiPortComboBox.insertSeparator(1);

    {
        QScopedPointer<RtMidiIn> midiIn(new RtMidiIn);
        quint32 midiPortCount = midiIn->getPortCount();

        for(quint32 i = 0; i < midiPortCount; ++i)
            _inputMidiPortComboBox.addItem(midiIn->getPortName(i).c_str());

        inputSettingsFormLayout->addRow(new QLabel("MIDI Port"), &_inputMidiPortComboBox);
    }

    _inputUdpPortSpinBox.setRange(6000, 6100);
    _inputUdpPortSpinBox.setButtonSymbols(QSpinBox::ButtonSymbols::NoButtons);
    inputSettingsFormLayout->addRow(new QLabel("UDP Port"), &_inputUdpPortSpinBox);

    _inputGmaStripTrailingZerosCheckBox.setText("Strip Trailing Zeros from Cue Numbers");
    inputSettingsFormLayout->addRow(&_inputGmaStripTrailingZerosCheckBox);

    inputSettingsGroupBox->setLayout(inputSettingsFormLayout);

    mainLayout->addWidget(inputSettingsGroupBox);

    this->setLayout(mainLayout);
}

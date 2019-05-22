#include "PreferencesWindow.h"

#include <QFormLayout>
#include <QLabel>
#include <QFrame>

#include <RtMidi.h>

PreferencesWindow::PreferencesWindow(QWidget *parent) :
    QWidget(parent)
{
    setWindowFlags(Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::CustomizeWindowHint);
    setWindowTitle("Cue TX");

    QFormLayout* inputSourceFormLayout = new QFormLayout;

    _inputSourceComboBox.addItem("MSC (MIDI)");
    _inputSourceComboBox.addItem("MSC (GrandMA2 Ethernet)");
    connect(&_inputSourceComboBox, qOverload<int>(&QComboBox::currentIndexChanged), this, &PreferencesWindow::onInputSourceChanged);

    inputSourceFormLayout->addRow(new QLabel("Input Source"), &_inputSourceComboBox);

    _mainLayout.addLayout(inputSourceFormLayout);


    QFormLayout* inputMidiMscControlsFormLayout = new QFormLayout;

    _inputMidiPortComboBox.addItem("CueTX Virtual Port");
    _inputMidiPortComboBox.insertSeparator(1);

    {
        QScopedPointer<RtMidiIn> midiIn(new RtMidiIn);
        quint32 midiPortCount = midiIn->getPortCount();

        for(quint32 i = 0; i < midiPortCount; ++i)
            _inputMidiPortComboBox.addItem(midiIn->getPortName(i).c_str());

        inputMidiMscControlsFormLayout->addRow(new QLabel("MIDI Port"), &_inputMidiPortComboBox);
    }

    _inputMidiMscControlsGroupBox.setLayout(inputMidiMscControlsFormLayout);
    _mainLayout.addWidget(&_inputMidiMscControlsGroupBox);


    QFormLayout* inputGmaMscControlsFormLayout = new QFormLayout;

    _inputUdpPortSpinBox.setRange(6000, 6100);
    _inputUdpPortSpinBox.setButtonSymbols(QSpinBox::ButtonSymbols::NoButtons);
    inputGmaMscControlsFormLayout->addRow(new QLabel("UDP Port"), &_inputUdpPortSpinBox);

    _inputGmaStripTrailingZerosCheckBox.setText("Strip Trailing Zeros from Cue Numbers");
    inputGmaMscControlsFormLayout->addRow(&_inputGmaStripTrailingZerosCheckBox);

    _inputGmaMscControlsGroupBox.setLayout(inputGmaMscControlsFormLayout);
    _mainLayout.addWidget(&_inputGmaMscControlsGroupBox);

    QFormLayout* outputDestinationFormLayout = new QFormLayout;

    _outputDestinationComboBox.addItem("MSC (MIDI)");
    _outputDestinationComboBox.addItem("MSC (GrandMA2 Ethernet)");
    _outputDestinationComboBox.addItem("OSC (QLab)");
    _outputDestinationComboBox.addItem("OSC (Disguise/d3)");
    outputDestinationFormLayout->addRow(new QLabel("Output Destination"), &_outputDestinationComboBox);

    _mainLayout.addLayout(outputDestinationFormLayout);

    this->setLayout(&_mainLayout);

    updateHiddenControls();
    loadSettings();
}

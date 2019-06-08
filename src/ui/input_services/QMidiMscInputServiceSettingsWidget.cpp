#include "QMidiMscInputServiceSettingsWidget.hpp"
#include "ui_QMidiMscInputServiceSettingsWidget.h"
#include "../../qrtmidi/QRtMidiIn.hpp"

QMidiMscInputServiceSettingsWidget::QMidiMscInputServiceSettingsWidget(QWidget *parent) :
    QSettingsWidget(parent),
    ui(new Ui::QMidiMscInputServiceSettingsWidget)
{
    ui->setupUi(this);

    updateMidiPortList();
}

QMidiMscInputServiceSettingsWidget::~QMidiMscInputServiceSettingsWidget()
{
    delete ui;
}

void QMidiMscInputServiceSettingsWidget::refresh()
{
    updateMidiPortList();
}

void QMidiMscInputServiceSettingsWidget::updateMidiPortList()
{
    ui->comboBoxMidiInPort->clear();

    QRtMidiIn midiIn;
    auto ports = midiIn.getMidiInPorts();

    for(const auto& port : ports)
        ui->comboBoxMidiInPort->addItem(port);
}

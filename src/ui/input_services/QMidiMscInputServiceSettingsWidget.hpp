#pragma once

#include "../QSettingsWidget.hpp"

namespace Ui
{
    class QMidiMscInputServiceSettingsWidget;
}

class QMidiMscInputServiceSettingsWidget : public QSettingsWidget
{
    Q_OBJECT

    Ui::QMidiMscInputServiceSettingsWidget* ui;

public:
    explicit QMidiMscInputServiceSettingsWidget(QWidget *parent = nullptr);
    ~QMidiMscInputServiceSettingsWidget() override;

    void refresh() override;

private:
    void updateMidiPortList();
};


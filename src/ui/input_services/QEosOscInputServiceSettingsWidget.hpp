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

#include "../QSettingsWidget.hpp"

namespace Ui
{
    class QEosOscInputServiceSettingsWidget;
}

class QEosOscInputServiceSettingsWidget : public QSettingsWidget
{
    Q_OBJECT

    Ui::QEosOscInputServiceSettingsWidget* ui;

public:
    explicit QEosOscInputServiceSettingsWidget(QWidget* parent = nullptr);
    ~QEosOscInputServiceSettingsWidget() override;

    void setSettings(const QVariantMap& settings) override;
    QVariantMap getSettings() override;
    void setDefaultSettings() override;

};


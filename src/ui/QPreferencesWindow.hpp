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

#include <QMainWindow>
#include <QList>
#include <QAbstractButton>
#include "../ServiceTypes.hpp"

namespace Ui
{
    class QPreferencesWindow;
}

class QPreferencesWindow : public QMainWindow
{
    Q_OBJECT

    static const QList<QPair<InputServiceType, QString>> inputServiceOptionList;
    static const QList<QPair<OutputServiceType, QString>> outputServiceOptionList;

    Ui::QPreferencesWindow* ui;

public:
    explicit QPreferencesWindow(QWidget *parent = nullptr);
    ~QPreferencesWindow();

    void refreshAndShow();

private:
    void updateVisibility();
    void updateInputSettingsWidget();
    void updateOutputSettingsWidget();

private slots:
    void on_comboBoxInput_currentIndexChanged();
    void on_comboBoxOutput_currentIndexChanged();

    void on_buttonBox_clicked(QAbstractButton* button);
};

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

#include "QEosOscInputServiceSettingsWidget.hpp"
#include "ui_QEosOscInputServiceSettingsWidget.h"
#include "../../input_services/QEosOscInputService.hpp"

QEosOscInputServiceSettingsWidget::QEosOscInputServiceSettingsWidget(QWidget *parent) :
    QSettingsWidget(parent),
    ui(new Ui::QEosOscInputServiceSettingsWidget)
{
    ui->setupUi(this);
}

QEosOscInputServiceSettingsWidget::~QEosOscInputServiceSettingsWidget()
{
    delete ui;
}

void QEosOscInputServiceSettingsWidget::setSettings(const QVariantMap& settings)
{

}

QVariantMap QEosOscInputServiceSettingsWidget::getSettings()
{
    return QVariantMap();
}

void QEosOscInputServiceSettingsWidget::setDefaultSettings()
{
	
}

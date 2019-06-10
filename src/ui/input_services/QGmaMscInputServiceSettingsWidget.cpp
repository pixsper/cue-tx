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

#include "QGmaMscInputServiceSettingsWidget.hpp"
#include "ui_QGmaMscInputServiceSettingsWidget.h"
#include "../../input_services/QGmaMscInputService.hpp"

QGmaMscInputServiceSettingsWidget::QGmaMscInputServiceSettingsWidget(QWidget *parent) :
    QSettingsWidget(parent),
    ui(new Ui::QGmaMscInputServiceSettingsWidget)
{
    ui->setupUi(this);

    ui->spinBoxInputUdpPort->setMinimum(QGmaMscInputService::SETTINGS_HOSTPORT_MIN);
    ui->spinBoxInputUdpPort->setMaximum(QGmaMscInputService::SETTINGS_HOSTPORT_MAX);
}

QGmaMscInputServiceSettingsWidget::~QGmaMscInputServiceSettingsWidget()
{
    delete ui;
}

void QGmaMscInputServiceSettingsWidget::setSettings(const QVariantMap& settings)
{

}

QVariantMap QGmaMscInputServiceSettingsWidget::getSettings()
{
    return QVariantMap();
}

void QGmaMscInputServiceSettingsWidget::setDefaultSettings()
{
    ui->spinBoxInputUdpPort->setValue(QGmaMscInputService::SETTINGS_HOSTPORT_DEFAULT);
    ui->checkBoxRemoveZeroPadding->setCheckState(
                QGmaMscInputService::SETTINGS_REMOVEZEROPADDING_DEFAULT ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
}

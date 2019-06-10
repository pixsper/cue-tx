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

#include "QSettingsManager.hpp"

QSettingsManager::QSettingsManager(QObject *parent)
    : QObject(parent)
{

}

void QSettingsManager::saveSettings(const QVariantMap& map)
{
    QMapIterator<QString, QVariant> it(map);
    while (it.hasNext())
    {
        it.next();

        if (it.value().userType() == QMetaType::QVariantMap)
        {
            _settings.beginGroup(it.key());

            saveSettings(it.value().toMap());

            _settings.endGroup();
        }

        _settings.setValue(it.key(), it.value());
    }
}

QVariantMap QSettingsManager::loadSettings()
{
    QVariantMap map;

    for(const auto& key : _settings.childKeys())
    {
        QVariant value = _settings.value(key);

        if (value.userType() != QMetaType::QVariantMap)
        {
            _settings.beginGroup(key);
            value = loadSettings();
            _settings.endGroup();
        }

        map.insert(key, value);
    }

    return map;
}

void QSettingsManager::clearSettings()
{
    _settings.clear();
}

bool QSettingsManager::isSettingsEmpty()
{
    return _settings.allKeys().size() == 0;
}

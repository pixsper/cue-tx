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
        _settings.setValue(it.key(), it.value());
    }
}

QVariantMap QSettingsManager::loadSettings()
{
    QVariantMap map;

    for(const auto& key : _settings.childKeys())
        map.insert(key, _settings.value(key));

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

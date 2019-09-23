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

#include "QGmaMscOutputService.hpp"

const QString QGmaMscOutputService::SETTINGS_HOSTPORT_KEY = "out_gma_host_port";
const quint16 QGmaMscOutputService::SETTINGS_HOSTPORT_DEFAULT;


const QString QGmaMscOutputService::SETTINGS_ADDZEROPADDING_KEY = "out_gma_add_zero_padding";
const bool QGmaMscOutputService::SETTINGS_ADDZEROPADDING_DEFAULT;

QVariantMap QGmaMscOutputService::staticDefaultSettings()
{
    return QVariantMap
    {
        { SETTINGS_HOSTPORT_KEY, SETTINGS_HOSTPORT_DEFAULT },
        { SETTINGS_ADDZEROPADDING_KEY, SETTINGS_ADDZEROPADDING_DEFAULT }
    };
}

QGmaMscOutputService::QGmaMscOutputService(QObject* parent)
    : QCueTxOutputService(parent),
      _udpSocket(new QUdpSocket(this)),
      _udpPort(SETTINGS_HOSTPORT_DEFAULT),
      _isAddZeroPadding(SETTINGS_ADDZEROPADDING_DEFAULT)
{
}

bool QGmaMscOutputService::start(const QVariantMap& settings)
{
    const auto itPort = settings.find(SETTINGS_HOSTPORT_KEY);

    if (itPort != settings.end())
        _udpPort = static_cast<quint16>(itPort.value().toInt());
    else
        _udpPort = SETTINGS_HOSTPORT_DEFAULT;

    return true;
}

void QGmaMscOutputService::stop()
{
    _udpSocket->close();
}

void QGmaMscOutputService::sendMessage(const MscMessage& message)
{
    QByteArray array;
    if (message.toByteArray(array))
    {
        QByteArray header(MSC_GMA_HEADER_LENGTH, 0);
        QDataStream stream(&header, QIODevice::OpenModeFlag::WriteOnly);
        stream.setByteOrder(QDataStream::LittleEndian);

        stream.writeRawData(MSC_GMAHEADER_ID1, 4);
        stream.writeRawData(MSC_GMAHEADER_ID2, 4);
        stream << static_cast<qint32>(array.size() + MSC_GMA_HEADER_LENGTH);

        array.insert(0, header);

        _udpSocket->writeDatagram(array, QHostAddress::Broadcast, _udpPort);
    }
}

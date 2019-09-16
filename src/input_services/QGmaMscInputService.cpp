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

#include "QGmaMscInputService.hpp"

const QString QGmaMscInputService::SETTINGS_HOSTPORT_KEY = "in_gma_host_port";
const quint16 QGmaMscInputService::SETTINGS_HOSTPORT_DEFAULT;
const quint16 QGmaMscInputService::SETTINGS_HOSTPORT_MIN;
const quint16 QGmaMscInputService::SETTINGS_HOSTPORT_MAX;
const QString QGmaMscInputService::SETTINGS_REMOVEZEROPADDING_KEY = "in_gma_remove_zero_padding";
const bool QGmaMscInputService::SETTINGS_REMOVEZEROPADDING_DEFAULT;


QVariantMap QGmaMscInputService::staticDefaultSettings()
{
    return QVariantMap
    {
        { SETTINGS_HOSTPORT_KEY, SETTINGS_HOSTPORT_DEFAULT },
        { SETTINGS_REMOVEZEROPADDING_KEY, SETTINGS_REMOVEZEROPADDING_DEFAULT }
    };
}

QGmaMscInputService::QGmaMscInputService(QObject* parent)
    : QCueTxInputService(parent),
      _udpSocket(new QUdpSocket(this)),
      _udpPort(SETTINGS_HOSTPORT_DEFAULT),
      _isRemoveZeroPadding(SETTINGS_REMOVEZEROPADDING_DEFAULT)
{

}

bool QGmaMscInputService::start(const QVariantMap& settings)
{
    const auto itPort = settings.find(SETTINGS_HOSTPORT_KEY);
    if (itPort != settings.end())
        _udpPort = static_cast<quint16>(itPort.value().toInt());
    else
        _udpPort = SETTINGS_HOSTPORT_DEFAULT;

    const auto itPadding = settings.find(SETTINGS_REMOVEZEROPADDING_KEY);
    if (itPadding != settings.end())
        _isRemoveZeroPadding = itPadding.value().toBool();
    else
        _isRemoveZeroPadding = SETTINGS_REMOVEZEROPADDING_DEFAULT;

    connect(_udpSocket, &QIODevice::readyRead, this, &QGmaMscInputService::readPendingDatagrams);
    return _udpSocket->bind(_udpPort, QAbstractSocket::ShareAddress);
}

void QGmaMscInputService::stop()
{
    _udpSocket->disconnectFromHost();
    disconnect(_udpSocket, &QIODevice::readyRead, this, &QGmaMscInputService::readPendingDatagrams);
}

void QGmaMscInputService::processDatagram(const QNetworkDatagram& datagram)
{
    if (datagram.data().length() < MSC_GMA_HEADER_LENGTH)
        return;

    QDataStream data(datagram.data());
    data.setByteOrder(QDataStream::LittleEndian);

    const int ID_LENGTH = 4;

    char headerId1[ID_LENGTH];
    data.readRawData(headerId1, ID_LENGTH);

    if (strncmp(headerId1, MSC_GMAHEADER_ID1, ID_LENGTH) != 0)
        return;

    char headerId2[ID_LENGTH];
    data.readRawData(headerId2, ID_LENGTH);

    if (strncmp(headerId2, MSC_GMAHEADER_ID2, ID_LENGTH) != 0)
        return;

    qint32 packetLength;
    data >> packetLength;

    if (packetLength != datagram.data().length())
        return;

    QByteArray mscData = datagram.data();
    mscData.remove(0, MSC_GMA_HEADER_LENGTH);

    MscMessage message;
    if (MscMessage::fromByteArray(mscData, message))
    {
        if (_isRemoveZeroPadding)
            message.removeZeroPadding();

        emit messageReceived(message);
    }
}

void QGmaMscInputService::readPendingDatagrams()
{
    while (_udpSocket->hasPendingDatagrams())
    {
        QNetworkDatagram datagram = _udpSocket->receiveDatagram();
        processDatagram(datagram);
    }
}



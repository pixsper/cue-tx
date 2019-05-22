#pragma once

#include <QObject>
#include "../MscMessage.h"

class QCueTxInputService : public QObject
{
    Q_OBJECT

protected:
    explicit QCueTxInputService(QObject* parent);

public:
    virtual bool start(const QVariantMap& configuration) = 0;
    virtual void stop() = 0;

signals:
    void messageReceived(const MscMessage& message);
};

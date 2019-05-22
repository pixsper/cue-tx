#pragma once

#include <QObject>
#include "../MscMessage.h"

class QCueTxOutputService : public QObject
{
    Q_OBJECT
protected:
    explicit QCueTxOutputService(QObject *parent);

public:
    virtual bool start(const QVariantMap& configuration) = 0;
    virtual void stop() = 0;

public slots:
    virtual void sendMessage(const MscMessage& message) = 0;
};

#pragma once

#include <QWidget>

class QSettingsWidget : public QWidget
{
    Q_OBJECT

protected:
    QSettingsWidget(QWidget* parent)
        : QWidget(parent)
    {

    }

public:
    virtual ~QSettingsWidget() { };
    virtual void refresh() { }

signals:

public slots:
};


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
    virtual ~QSettingsWidget() { }
    virtual void refresh() { }
    virtual void setDefaults() = 0;

signals:

public slots:
};


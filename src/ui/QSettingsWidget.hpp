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

    virtual void setSettings(const QVariantMap& settings) = 0;
    virtual QVariantMap getSettings() = 0;
    virtual void setDefaultSettings() = 0;

signals:
    void settingsChanged(const QVariantMap& settings);
};


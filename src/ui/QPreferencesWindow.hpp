#pragma once

#include <QMainWindow>
#include <QList>
#include "../ServiceTypes.hpp"

namespace Ui
{
    class QPreferencesWindow;
}

class QPreferencesWindow : public QMainWindow
{
    Q_OBJECT

    static const QList<QPair<InputServiceType, QString>> inputServiceOptionList;
    static const QList<QPair<OutputServiceType, QString>> outputServiceOptionList;

    Ui::QPreferencesWindow* ui;

public:
    explicit QPreferencesWindow(QWidget *parent = nullptr);
    ~QPreferencesWindow();

    void refreshAndShow();

private:
    void updateVisibility();
    void updateInputSettingsWidget();
    void updateOutputSettingsWidget();

private slots:
    void on_comboBoxInput_currentIndexChanged();
    void on_comboBoxOutput_currentIndexChanged();

    void on_pushButtonOk_clicked();
    void on_pushButtonCancel_clicked();
    void on_pushButtonApply_clicked();
};


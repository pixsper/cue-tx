#pragma once

#include <QMainWindow>

namespace Ui {
    class QPreferencesWindow;
}

class QPreferencesWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit QPreferencesWindow(QWidget *parent = nullptr);
    ~QPreferencesWindow();

private:
    Ui::QPreferencesWindow *ui;
};


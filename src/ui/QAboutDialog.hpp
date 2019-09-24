#pragma once

#include <QDialog>

namespace Ui {
class QAboutDialog;
}

class QAboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QAboutDialog(QWidget *parent = nullptr);
    ~QAboutDialog();

private:
    Ui::QAboutDialog *ui;
};


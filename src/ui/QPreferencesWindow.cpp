#include "QPreferencesWindow.hpp"
#include "ui_QPreferencesWindow.h"

QPreferencesWindow::QPreferencesWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QPreferencesWindow)
{
    ui->setupUi(this);
}

QPreferencesWindow::~QPreferencesWindow()
{
    delete ui;
}

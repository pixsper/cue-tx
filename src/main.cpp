#include <QApplication>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QMessageBox>

#include "PreferencesWindow.h"

int main(int argc, char *argv[])
{
    QString version(GIT_VERSION);

    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);

    QCoreApplication::setOrganizationName("The Impersonal Stereo");
    QCoreApplication::setOrganizationDomain("theimpersonalstereo.com");
    QCoreApplication::setApplicationName("CueTX");

    QIcon icon(":/trayicon.png");
    QSystemTrayIcon tray(icon);

    PreferencesWindow preferencesWindow;

    QMenu* trayMenu = new QMenu();
    trayMenu->addAction("&Preferences...", [&](){ preferencesWindow.show(); });
    trayMenu->addSeparator();
    trayMenu->addAction("&About CueTX", [&]() { QMessageBox::about(nullptr, "CueTX", "CueTX by David Butler\n" + version); });
    trayMenu->addSeparator();
    trayMenu->addAction("&Quit", [](){ QApplication::quit(); });

    tray.show();
    tray.setContextMenu(trayMenu);

    return a.exec();
}

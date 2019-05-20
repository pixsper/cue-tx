#include <QApplication>
#include <QMenu>
#include <QSystemTrayIcon>

#include "preferenceswindow.h"
#include "qmscudpreceiver.h"
#include "qmidiout.h"

int main(int argc, char *argv[])
{
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
    trayMenu->addAction("&About CueTX");
    trayMenu->addSeparator();
    trayMenu->addAction("&Quit", [](){ QApplication::quit(); });

    tray.show();
    tray.setContextMenu(trayMenu);

    QMidiOut midiOut;
    QMscUdpReceiver receiver;

    QObject::connect(&receiver,&QMscUdpReceiver::packetReceived, &midiOut, &QMidiOut::sendMessage);

    receiver.startListening(6004);

    return a.exec();
}

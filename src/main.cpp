// This file is part of CueTX.
//
// CueTX is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation, either version 3 of
// the License, or (at your option) any later version.
//
// CueTX is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with CueTX.  If not, see <https://www.gnu.org/licenses/>.

#include <QApplication>
#include <QCommandLineParser>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QMessageBox>
#include <QLoggingCategory>

#include "ui/QPreferencesWindow.hpp"
#include "QCueRouter.hpp"
#include "QSettingsManager.hpp"

#ifdef Q_OS_MAC
    // TODO: See below
    #include "qrtmidi/QRtMidiIn.hpp"
#endif

int main(int argc, char* argv[])
{
#ifdef Q_OS_MAC
    // On Mac OS, creating an RtMidiIn instance after closing one causes a CoreMIDI error.
    // To avoid this, we keep an instance open for the application lifetime.
    // TODO: Remove this once the RtMidi bug is fixed. https://github.com/thestk/rtmidi/issues/155
    QRtMidiIn midiInDummy;
#endif

	QString version(GIT_VERSION);

	QApplication a(argc, argv);
	QApplication::setQuitOnLastWindowClosed(false);

    QCoreApplication::setOrganizationName("David Butler Design");
    QCoreApplication::setOrganizationDomain("davidbutlerdesign.co.uk");
	QCoreApplication::setApplicationName("CueTX");
    QCoreApplication::setApplicationVersion(version);




    QCommandLineParser parser;
    parser.setApplicationDescription("Cue TX show control routing utility");
    parser.addVersionOption();

    QCommandLineOption cleanStartOption({ "c", "clean" }, "Clear all application settings and start as if a clean installation" );
    parser.addOption(cleanStartOption);

    parser.process(a);



    QSettingsManager settingsManager;
    QCueRouter router;

    if (parser.isSet(cleanStartOption))
        settingsManager.clearSettings();



    QPreferencesWindow preferencesWindow;
#ifdef Q_OS_MAC
    preferencesWindow.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::CustomizeWindowHint);
#else
    preferencesWindow.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
#endif

    QObject::connect(&preferencesWindow, &QPreferencesWindow::settingsChanged, [&](const QVariantMap& map)
    {
        settingsManager.clearSettings();
        settingsManager.saveSettings(map);

        router.stop();
        router.start(settingsManager.loadSettings());
    });


    const QIcon icon(":/trayicon.png");
    QSystemTrayIcon tray(icon);

	auto trayMenu = new QMenu();
    trayMenu->addAction("&Preferences...", [&]() {
        preferencesWindow.setSettings(settingsManager.loadSettings());
        preferencesWindow.refreshAndShow();
    });
	trayMenu->addSeparator();
	trayMenu->addAction("&About CueTX", [&]()
	{
		QMessageBox::about(nullptr, "CueTX", "CueTX by David Butler\n" + version);
	});
	trayMenu->addSeparator();
	trayMenu->addAction("&Quit", []() { QApplication::quit(); });

	tray.show();
	tray.setContextMenu(trayMenu);

    router.start(settingsManager.loadSettings());

    if (settingsManager.isSettingsEmpty())
        preferencesWindow.refreshAndShow();

	return QApplication::exec();
}

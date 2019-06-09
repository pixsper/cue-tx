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
#include <QMenu>
#include <QSystemTrayIcon>
#include <QMessageBox>

#include "ui/QPreferencesWindow.hpp"
#include "QMscRouter.hpp"
#include "qrtmidi/QRtMidiIn.hpp"

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

	QCoreApplication::setOrganizationName("The Impersonal Stereo");
	QCoreApplication::setOrganizationDomain("theimpersonalstereo.com");
	QCoreApplication::setApplicationName("CueTX");

	const QIcon icon(":/trayicon.png");
	QSystemTrayIcon tray(icon);

    QPreferencesWindow preferencesWindow;
#ifdef Q_OS_MAC
    preferencesWindow.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::CustomizeWindowHint);
#else
    preferencesWindow.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
#endif

	auto trayMenu = new QMenu();
    trayMenu->addAction("&Preferences...", [&]() { preferencesWindow.refreshAndShow(); });
	trayMenu->addSeparator();
	trayMenu->addAction("&About CueTX", [&]()
	{
		QMessageBox::about(nullptr, "CueTX", "CueTX by David Butler\n" + version);
	});
	trayMenu->addSeparator();
	trayMenu->addAction("&Quit", []() { QApplication::quit(); });

	tray.show();
	tray.setContextMenu(trayMenu);

	QMscRouter router;

	return QApplication::exec();
}

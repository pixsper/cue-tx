QT       += core gui \
        network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cue-tx
TEMPLATE = app

include($$PWD/build/gitversion.pri)

DEFINES += QT_DEPRECATED_WARNINGS

QMAKE_INFO_PLIST = $$PWD/build/Info.plist


CONFIG += c++17

SOURCES += \
        src/MscCueId.cpp \
        src/MscMessage.cpp \
        src/MscTimecode.cpp \
        src/QCueLogger.cpp \
        src/QCueRouter.cpp \
        src/QSettingsManager.cpp \
        src/ServiceFactory.cpp \
        src/input_services/QEosOscInputService.cpp \
        src/input_services/QGmaMscInputService.cpp \
        src/input_services/QMidiMscInputService.cpp \
        src/output_services/QDisguiseOscOutputService.cpp \
        src/output_services/QGmaMscOutputService.cpp \
        src/output_services/QMidiMscOutputService.cpp \
        src/output_services/QQLabOscOutputService.cpp \
        src/qrtmidi/QRtMidiIn.cpp \
        src/qrtmidi/QRtMidiOut.cpp \
        src/main.cpp \
        src/ui/QPreferencesWindow.cpp \
        src/ui/SettingsWidgetFactory.cpp \
        src/ui/input_services/QGmaMscInputServiceSettingsWidget.cpp \
        src/ui/input_services/QEosOscInputServiceSettingsWidget.cpp \
        src/ui/input_services/QMidiMscInputServiceSettingsWidget.cpp \
        src/ui/output_services/QMidiMscOutputServiceSettingsWidget.cpp

HEADERS += \
        src/MscCueId.hpp \
        src/MscMessage.hpp \
        src/MscTimecode.hpp \
        src/QCueLogger.hpp \
        src/QCueRouter.hpp \
        src/QSettingsManager.hpp \
        src/ServiceFactory.hpp \
        src/ServiceTypes.hpp \
        src/input_services/QCueTxInputService.hpp \
        src/input_services/QEosOscInputService.hpp \
        src/input_services/QGmaMscInputService.hpp \
        src/input_services/QMidiMscInputService.hpp \
        src/output_services/QCueTxOutputService.hpp \
        src/output_services/QDisguiseOscOutputService.hpp \
        src/output_services/QGmaMscOutputService.hpp \
        src/output_services/QMidiMscOutputService.hpp \
        src/output_services/QQLabOscOutputService.hpp \
        src/qrtmidi/QRtMidiIn.hpp \
        src/qrtmidi/QRtMidiOut.hpp \
        src/ui/QPreferencesWindow.hpp \
        src/ui/QSettingsWidget.hpp \
        src/ui/SettingsWidgetFactory.hpp \
        src/ui/input_services/QGmaMscInputServiceSettingsWidget.hpp \
        src/ui/input_services/QEosOscInputServiceSettingsWidget.hpp \
        src/ui/input_services/QMidiMscInputServiceSettingsWidget.hpp \
        src/ui/output_services/QMidiMscOutputServiceSettingsWidget.hpp

FORMS += \
    src/ui/QPreferencesWindow.ui \
    src/ui/input_services/QGmaMscInputServiceSettingsWidget.ui \
    src/ui/input_services/QEosOscInputServiceSettingsWidget.ui \
    src/ui/input_services/QMidiMscInputServiceSettingsWidget.ui \
    src/ui/output_services/QMidiMscOutputServiceSettingsWidget.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    resources/resources.qrc

macx: LIBS += -framework CoreFoundation
macx: LIBS += -framework CoreAudio
macx: LIBS += -framework CoreMidi
win32: LIBS += -lwinmm

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/third-party/rtmidi/lib/ -lrtmidi
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/third-party/rtmidi/lib/ -lrtmidid
else:macx: LIBS += -L$$PWD/third-party/rtmidi/lib/ -lrtmidi

INCLUDEPATH += $$PWD/third-party/rtmidi/include \
               $$PWD/third-party/oscpp/include

DEPENDPATH += $$PWD/third-party/rtmidi/include \
              $$PWD/third-party/oscpp/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/third-party/rtmidi/lib/librtmidi.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/third-party/rtmidi/lib/librtmidid.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/third-party/rtmidi/lib/rtmidi.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/third-party/rtmidi/lib/rtmidid.lib
else:macx: PRE_TARGETDEPS += $$PWD/third-party/rtmidi/lib/librtmidi.a


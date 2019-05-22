QT       += core gui \
        network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cue-tx
TEMPLATE = app

include($$PWD/build/gitversion.pri)

DEFINES += QT_DEPRECATED_WARNINGS

QMAKE_INFO_PLIST = $$PWD/build/Info.plist


CONFIG += c++14

SOURCES += \
        src/MscMessage.cpp \
        src/MscTimecode.cpp \
        src/PreferencesWindow.cpp \
        src/QMscRouterService.cpp \
        src/qmidi/QRtMidiIn.cpp \
        src/qmidi/QRtMidiOut.cpp \
        src/main.cpp

HEADERS += \
        src/MscMessage.h \
        src/MscTimecode.h \
        src/PreferencesWindow.h \
        src/input_services/QCueTxInputService.h \
        src/input_services/QEosOscInputService.h \
        src/input_services/QGmaMscInputService.h \
        src/input_services/QMidiMscInputService.h \
        src/output_services/QCueTxOutputService.h \
        src/output_services/QDisguiseOscOutputService.h \
        src/QMscRouterService.h \
        src/output_services/QQLabOscOutputService.h \
        src/qmidi/QRtMidiIn.h \
        src/qmidi/QRtMidiOut.h


qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    resources/resources.qrc

macx|win32: LIBS += -L$$PWD/third-party/rtmidi/lib/ -lrtmidi

macx: LIBS += -framework CoreFoundation
macx: LIBS += -framework CoreAudio
macx: LIBS += -framework CoreMidi


INCLUDEPATH += $$PWD/third-party/rtmidi/include \
               $$PWD/third-party/oscpp/include



DEPENDPATH += $$PWD/third-party/rtmidi/include \
              $$PWD/third-party/oscpp/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/third-party/rtmidi/lib/rtmidi.lib
else:macx|win32-g++: PRE_TARGETDEPS += $$PWD/third-party/rtmidi/lib/librtmidi.a


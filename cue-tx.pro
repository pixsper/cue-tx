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
        src/MscCueId.cpp \
        src/MscMessage.cpp \
        src/MscTimecode.cpp \
        src/PreferencesWindow.cpp \
        src/QMscRouter.cpp \
        src/ServiceFactory.cpp \
        src/qmidi/QRtMidiIn.cpp \
        src/qmidi/QRtMidiOut.cpp \
        src/main.cpp

HEADERS += \
        src/MscCueId.h \
        src/MscMessage.h \
        src/MscTimecode.h \
        src/PreferencesWindow.h \
        src/QMscRouter.h \
        src/ServiceFactory.h \
        src/ServiceTypes.h \
        src/input_services/QCueTxInputService.h \
        src/input_services/QEosOscInputService.h \
        src/input_services/QGmaMscInputService.h \
        src/input_services/QMidiMscInputService.h \
        src/output_services/QCueTxOutputService.h \
        src/output_services/QDisguiseOscOutputService.h \
        src/output_services/QGmaMscOutputService.h \
        src/output_services/QMidiMscOutputService.h \
        src/output_services/QQLabOscOutputService.h \
        src/qmidi/QRtMidiIn.h \
        src/qmidi/QRtMidiOut.h


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


QT       += core gui \
        network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cue-tx
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

QMAKE_INFO_PLIST = $$PWD/build/Info.plist

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        src/mscmessage.cpp \
        src/msctimecode.cpp \
        src/preferenceswindow.cpp \
        src/qmidiout.cpp \
        src/qmscrouterservice.cpp \
        src/qmscudpreceiver.cpp \
        src/main.cpp

HEADERS += \
        src/mscmessage.h \
        src/msctimecode.h \
        src/preferenceswindow.h \
        src/qmidiout.h \
        src/qmscrouterservice.h \
        src/qmscudpreceiver.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    resources/resources.qrc

macx|win32: LIBS += -L$$PWD/third-party/rtmidi/lib/ -lrtmidi

INCLUDEPATH += $$PWD/third-party/rtmidi/include
DEPENDPATH += $$PWD/third-party/rtmidi/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/third-party/rtmidi/lib/rtmidi.lib
else:macx|win32-g++: PRE_TARGETDEPS += $$PWD/third-party/rtmidi/lib/librtmidi.a

macx: LIBS += -framework CoreFoundation
macx: LIBS += -framework CoreAudio
macx: LIBS += -framework CoreMidi

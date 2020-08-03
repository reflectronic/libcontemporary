#-------------------------------------------------
#
# Project created by QtCreator 2016-12-30T16:59:03
#
#-------------------------------------------------

QT       += widgets multimedia svg

TARGET = the-libs
TEMPLATE = lib

unix:!android {
    CONFIG   += c++14
    system("pkg-config --version") {
        CONFIG += link_pkgconfig
        packagesExist(libunwind) {
            message("Building with libunwind support");
            PKGCONFIG += libunwind
            DEFINES += HAVE_LIBUNWIND
        } else {
            message("libunwind not found on this system.");
        }

        !macx:packagesExist(x11) {
            message("Building with X11 support");
            PKGCONFIG += x11
            DEFINES += HAVE_X11
            QT += x11extras
        } else {
            message("X11 not found on this system.");
        }

        packagesExist(gsettings-qt) {
            message("Building with GSettings support");
            PKGCONFIG += gsettings-qt
            DEFINES += HAVE_GSETTINGS
        } else {
            message("GSettings not found on this system.");
        }
    }
}

macx {
    CONFIG   += c++14
    LIBS += -framework CoreFoundation -framework Cocoa
}

win32 {
    CONFIG   += c++17
    LIBS += -lUser32 -lKernel32 -lDbgHelp -lwindowsapp
    DEFINES += _WIN32_WINNT=0x0601 # Windows 7 or up
}

DEFINES += THELIBS_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += tvariantanimation.cpp \
    taboutdialog.cpp \
    tconditionalwidget.cpp \
    tcsdtools.cpp \
    tcsdtools/csdbuttonbox.cpp \
    tcsdtools/csdsizegrip.cpp \
    private/nativeeventfilter.cpp \
    tdatetimepicker.cpp \
    tdatetimepicker/datetimepart.cpp \
    tdatetimepicker/datetimepartbutton.cpp \
    terrorflash.cpp \
    tlocale.cpp \
    tpromise.cpp \
    tpropertyanimation.cpp \
    thelibsglobal.cpp \
    tsettings.cpp \
    tstatusframe.cpp \
    ttitlelabel.cpp \
    ttoast.cpp \
    tvirtualkeyboard.cpp \
    tcircularspinner.cpp \
    tnotification/tnotification-common.cpp \
    tapplication.cpp \
    tshortcuthud.cpp \
    tstackedwidget.cpp \
    tpopover.cpp \
    tmessagebox.cpp \
    tswitch.cpp \
    tsystemsound.cpp

HEADERS += tvariantanimation.h\
    taboutdialog.h \
    tconditionalwidget.h \
    tcsdtools.h \
    tcsdtools/csdbuttonbox.h \
    tcsdtools/csdsizegrip.h \
    private/nativeeventfilter.h \
    tdatetimepicker.h \
    tdatetimepicker/datetimepart.h \
    tdatetimepicker/datetimepartbutton.h \
    terrorflash.h \
    the-libs_global.h \
    tlocale.h \
    tpropertyanimation.h \
    tsettings.h \
    tstatusframe.h \
    ttitlelabel.h \
    ttoast.h \
    tnotification.h \
    tvirtualkeyboard.h \
    tcircularspinner.h \
    tapplication.h \
    tpromise.h \
    tshortcuthud.h \
    tstackedwidget.h \
    tpopover.h \
    tmessagebox.h \
    tswitch.h \
    tsystemsound.h

# Include required build tools
include($$PWD/prifiles/gentranslations.pri)

# Translations are compiled in so installation is not required

# Install rules
header.files = *.h
module.path = $$[QMAKE_MKSPECS]/modules
prifiles.files = prifiles/*

unix {
    module.files = qt_thelib.pri
}

unix:!macx:!android {
    QT += dbus

    target.path = $$[QT_INSTALL_LIBS]
    header.path = $$[QT_INSTALL_HEADERS]/the-libs
    module.files = qt_thelib.pri
    prifiles.path = /usr/share/the-libs/pri

    HEADERS += tnotification/tnotification-linux.h
    SOURCES += tnotification/tnotification-linux.cpp
}

macx {
    CONFIG(debug, debug|release): TARGET = the-libs_debug

    target.path = /usr/local/lib
    header.path = /usr/local/include/the-libs
    prifiles.path = /usr/local/share/the-libs/pri
    module.files = qt_thelib_mac.pri

    SOURCES += tnotification/tnotification-mac.mm
}

win32 {
    CONFIG(debug, debug|release): TARGET = the-libsd

    module.files = qt_thelib.pri
    header.path = "C:/Program Files/thelibs/include"
    target.path = "C:/Program Files/thelibs/lib"
    prifiles.path = "C:/Program Files/thelibs/pri"

    SOURCES += tnotification/tnotification-win.cpp
    HEADERS += tnotification/tnotification-win.h
}

android {
    target.path = /libs/armeabi-v7a
    header.path = /include/the-libs
    module.files = qt_thelib.pri
    module.path = /mkspecs/modules
    prifiles.path = /share/the-libs/pri

    SOURCES += tnotification/tnotification-android.cpp
}

INSTALLS += target module header prifiles

DISTFILES += \
    prifiles/buildmaster.pri \
    prifiles/gentranslations.pri \
    prifiles/installtranslations.pri \
    prifiles/checkblueprint.pri \
    qt_thelib.pri \
    qt_thelib_mac.pri

FORMS += \
    taboutdialog.ui \
    tcsdtools/csdbuttonbox.ui \
    tshortcuthud.ui \
    tstatusframe.ui

RESOURCES += \
    thelibs_icons.qrc \
    thelibs_translations.qrc

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/xfilesystemmodel.h \
    $$PWD/xfileexplorer.h

SOURCES += \
    $$PWD/xfilesystemmodel.cpp \
    $$PWD/xfileexplorer.cpp

FORMS += \
    $$PWD/xfileexplorer.ui

!contains(XCONFIG, xcomboboxex) {
    XCONFIG += xcomboboxex
    include($$PWD/../../Controls/xcomboboxex.pri)
}

DISTFILES += \
    $$PWD/xfileexplorer.cmake

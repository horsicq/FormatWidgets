QT       += concurrent

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/dialogmultisearchprocess.h \
    $$PWD/multisearch.h

SOURCES += \
    $$PWD/dialogmultisearchprocess.cpp \
    $$PWD/multisearch.cpp

!contains(XCONFIG, xoptions) {
    XCONFIG += xoptions
    include($$PWD/../../XOptions/xoptions.pri)
}

!contains(XCONFIG, xshortcuts) {
    XCONFIG += xshortcuts
    include($$PWD/../../XShortcuts/xshortcuts.pri)
}

!contains(XCONFIG, xdialogprocess) {
    XCONFIG += xdialogprocess
    include($$PWD/../../FormatDialogs/xdialogprocess.pri)
}



DISTFILES += \
    $$PWD/multisearch.cmake

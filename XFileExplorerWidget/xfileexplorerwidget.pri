INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/xfilesystemmodel.h \
    $$PWD/xfileexplorerwidget.h

SOURCES += \
    $$PWD/xfilesystemmodel.cpp \
    $$PWD/xfileexplorerwidget.cpp

FORMS += \
    $$PWD/xfileexplorerwidget.ui

!contains(XCONFIG, xcomboboxex) {
    XCONFIG += xcomboboxex
    include($$PWD/../../Controls/xcomboboxex.pri)
}

!contains(XCONFIG, use_dex) {
    XCONFIG += use_dex
}

!contains(XCONFIG, use_pdf) {
    XCONFIG += use_pdf
}

!contains(XCONFIG, use_archive) {
    XCONFIG += use_archive
}

!contains(XCONFIG, xfileinfo) {
    XCONFIG += xfileinfo
    include($$PWD/../../XFileInfo/xfileinfo.pri)
}

!contains(XCONFIG, xshortcuts) {
    XCONFIG += xshortcuts
    include($$PWD/../../XShortcuts/xshortcuts.pri)
}

!contains(XCONFIG, dialogwidget) {
    XCONFIG += dialogwidget
    include($$PWD/../../FormatDialogs/dialogwidget.pri)
}

!contains(XCONFIG, xdialogprocess) {
    XCONFIG += xdialogprocess
    include($$PWD/../../FormatDialogs/xdialogprocess.pri)
}

DISTFILES += \
    $$PWD/xfileexplorerwidget.cmake

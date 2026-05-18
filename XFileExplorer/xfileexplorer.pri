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

!contains(XCONFIG, dialogwidget) {
    XCONFIG += dialogwidget
    include($$PWD/../../FormatDialogs/dialogwidget.pri)
}

!contains(XCONFIG, xdialogprocess) {
    XCONFIG += xdialogprocess
    include($$PWD/../../FormatDialogs/xdialogprocess.pri)
}

DISTFILES += \
    $$PWD/xfileexplorer.cmake

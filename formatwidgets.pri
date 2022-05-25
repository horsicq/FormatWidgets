INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

!contains(XCONFIG, use_dex) {
    XCONFIG += use_dex
}

!contains(XCONFIG, use_archive) {
    XCONFIG += use_archive
}

HEADERS += \
    $$PWD/formatswidget.h

SOURCES += \
    $$PWD/formatswidget.cpp

FORMS += \
    $$PWD/formatswidget.ui

!contains(XCONFIG, binarywidget) {
    XCONFIG += binarywidget
    include($$PWD/Binary/binarywidget.pri)
}

!contains(XCONFIG, msdoswidget) {
    XCONFIG += msdoswidget
    include($$PWD/MSDOS/msdoswidget.pri)
}

!contains(XCONFIG, elfwidget) {
    XCONFIG += elfwidget
    include($$PWD/ELF/elfwidget.pri)
}

!contains(XCONFIG, msdoswidget) {
    XCONFIG += lewidget
    include($$PWD/LE/lewidget.pri)
}

!contains(XCONFIG, machwidget) {
    XCONFIG += machwidget
    include($$PWD/MACH/machwidget.pri)
}

!contains(XCONFIG, machofatwidget) {
    XCONFIG += machofatwidget
    include($$PWD/MACHOFAT/machofatwidget.pri)
}

!contains(XCONFIG, newidget) {
    XCONFIG += newidget
    include($$PWD/NE/newidget.pri)
}

!contains(XCONFIG, lewidget) {
    XCONFIG += lewidget
    include($$PWD/LE/lewidget.pri)
}

!contains(XCONFIG, pewidget) {
    XCONFIG += pewidget
    include($$PWD/PE/pewidget.pri)
}

!contains(XCONFIG, dexwidget) {
    XCONFIG += dexwidget
    include($$PWD/DEX/dexwidget.pri)
}

!contains(XCONFIG, die_widget) {
    XCONFIG += die_widget
    include($$PWD/../die_widget/die_widget.pri)
}

!contains(XCONFIG, nfd_widget) {
    XCONFIG += nfd_widget
    include($$PWD/../nfd_widget/nfd_widget.pri)
}

!contains(XCONFIG, archive_widget) {
    XCONFIG += archive_widget
    include($$PWD/../archive_widget/archive_widget.pri)
}

!contains(XCONFIG, xshortcuts) {
    XCONFIG += xshortcuts
    include($$PWD/../XShortcuts/xshortcuts.pri)
}

DISTFILES += \
    $$PWD/formatwidgets.cmake


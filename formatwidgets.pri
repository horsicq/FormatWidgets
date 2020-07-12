INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

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


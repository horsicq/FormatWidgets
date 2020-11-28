INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

!contains(XCONFIG, binarywidget) {
    XCONFIG += binarywidget
    include($$PWD/Binary/binarywidget.pri)
}

!contains(XCONFIG, msdoswidget) {
    XCONFIG += msdoswidget
    include($$PWD/MSDOS/msdoswidget.pri)
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

!contains(XCONFIG, elfwidget) {
    XCONFIG += elfwidget
    include($$PWD/ELF/elfwidget.pri)
}

!contains(XCONFIG, machwidget) {
    XCONFIG += machwidget
    include($$PWD/MACH/machwidget.pri)
}

!contains(XCONFIG, searchstringswidget) {
    XCONFIG += searchstringswidget
    include($$PWD/SearchStrings/searchstringswidget.pri)
}

contains(XCONFIG, use_dex) {
    !contains(XCONFIG, dexwidget) {
        XCONFIG += dexwidget
        include($$PWD/DEX/dexwidget.pri)
    }
}

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

!contains(XCONFIG, xmsdos) {
    XCONFIG += xmsdos
    include(../../Formats/xmsdos.pri)
}

!contains(XCONFIG, formatwidget) {
    XCONFIG += formatwidget
    include(../formatwidget.pri)
}

FORMS += \
    $$PWD/msdoswidget.ui \
    $$PWD/dialogmsdos.ui

HEADERS += \
    $$PWD/msdoswidget.h \
    $$PWD/msdos_defs.h \
    $$PWD/dialogmsdos.h

SOURCES += \
    $$PWD/msdoswidget.cpp \
    $$PWD/msdos_defs.cpp \
    $$PWD/dialogmsdos.cpp

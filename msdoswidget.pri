INCLUDEPATH += $$PWD/MSDOS
DEPENDPATH += $$PWD/MSDOS

!contains(XCONFIG, qmsdos) {
    XCONFIG += qmsdos
    include(../Formats/qmsdos.pri)
}

!contains(XCONFIG, formatwidget) {
    XCONFIG += formatwidget
    include(formatwidget.pri)
}

FORMS += \
    $$PWD/MSDOS/msdoswidget.ui \
    $$PWD/MSDOS/dialogmsdos.ui

HEADERS += \
    $$PWD/MSDOS/msdoswidget.h \
    $$PWD/MSDOS/msdos_defs.h \
    $$PWD/MSDOS/dialogmsdos.h

SOURCES += \
    $$PWD/MSDOS/msdoswidget.cpp \
    $$PWD/MSDOS/msdos_defs.cpp \
    $$PWD/MSDOS/dialogmsdos.cpp

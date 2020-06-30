INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

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

!contains(XCONFIG, formatwidget) {
    XCONFIG += formatwidget
    include($$PWD/../formatwidget.pri)
}

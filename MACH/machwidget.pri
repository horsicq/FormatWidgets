INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

FORMS += \
    $$PWD/machwidget.ui \
    $$PWD/dialogmach.ui

HEADERS += \
    $$PWD/machwidget.h \
    $$PWD/mach_defs.h \
    $$PWD/dialogmach.h

SOURCES += \
    $$PWD/machwidget.cpp \
    $$PWD/mach_defs.cpp \
    $$PWD/dialogmach.cpp

!contains(XCONFIG, xmach) {
    XCONFIG += xmach
    include($$PWD/../../Formats/xmach.pri)
}

!contains(XCONFIG, formatwidget) {
    XCONFIG += formatwidget
    include($$PWD/../formatwidget.pri)
}

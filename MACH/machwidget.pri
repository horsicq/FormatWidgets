INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

FORMS += \
    $$PWD/machwidget.ui \
    $$PWD/dialogmach.ui \
    $$PWD/sectionheaderwidget.ui

HEADERS += \
    $$PWD/machprocessdata.h \
    $$PWD/machwidget.h \
    $$PWD/mach_defs.h \
    $$PWD/dialogmach.h \
    $$PWD/sectionheaderwidget.h

SOURCES += \
    $$PWD/machprocessdata.cpp \
    $$PWD/machwidget.cpp \
    $$PWD/mach_defs.cpp \
    $$PWD/dialogmach.cpp \
    $$PWD/sectionheaderwidget.cpp

!contains(XCONFIG, xmach) {
    XCONFIG += xmach
    include($$PWD/../../Formats/xmach.pri)
}

!contains(XCONFIG, formatwidget) {
    XCONFIG += formatwidget
    include($$PWD/../formatwidget.pri)
}

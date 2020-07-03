INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

FORMS += \
    $$PWD/machsectionheaderwidget.ui \
    $$PWD/machwidget.ui \
    $$PWD/dialogmach.ui

HEADERS += \
    $$PWD/machprocessdata.h \
    $$PWD/machsectionheaderwidget.h \
    $$PWD/machwidget.h \
    $$PWD/mach_defs.h \
    $$PWD/dialogmach.h

SOURCES += \
    $$PWD/machprocessdata.cpp \
    $$PWD/machsectionheaderwidget.cpp \
    $$PWD/machwidget.cpp \
    $$PWD/mach_defs.cpp \
    $$PWD/dialogmach.cpp

!contains(XCONFIG, formatwidget) {
    XCONFIG += formatwidget
    include($$PWD/../formatwidget.pri)
}

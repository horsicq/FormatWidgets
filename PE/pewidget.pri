INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/pesectionheaderwidget.h \
    $$PWD/petoolswidget.h \
    $$PWD/pewidget.h \
    $$PWD/pe_defs.h \
    $$PWD/dialogpe.h \
    $$PWD/peprocessdata.h

SOURCES += \
    $$PWD/pesectionheaderwidget.cpp \
    $$PWD/petoolswidget.cpp \
    $$PWD/pewidget.cpp \
    $$PWD/pe_defs.cpp \
    $$PWD/dialogpe.cpp \
    $$PWD/peprocessdata.cpp

FORMS += \
    $$PWD/pesectionheaderwidget.ui \
    $$PWD/petoolswidget.ui \
    $$PWD/pewidget.ui \
    $$PWD/dialogpe.ui

!contains(XCONFIG, formatwidget) {
    XCONFIG += formatwidget
    include($$PWD/../formatwidget.pri)
}

DISTFILES += \
    $$PWD/pewidget.cmake

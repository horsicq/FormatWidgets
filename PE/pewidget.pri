INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

FORMS += \
    $$PWD/pesectionheaderwidget.ui \
    $$PWD/pewidget.ui \
    $$PWD/dialogpe.ui

HEADERS += \
    $$PWD/pesectionheaderwidget.h \
    $$PWD/pewidget.h \
    $$PWD/pe_defs.h \
    $$PWD/dialogpe.h \
    $$PWD/peprocessdata.h

SOURCES += \
    $$PWD/pesectionheaderwidget.cpp \
    $$PWD/pewidget.cpp \
    $$PWD/pe_defs.cpp \
    $$PWD/dialogpe.cpp \
    $$PWD/peprocessdata.cpp

!contains(XCONFIG, formatwidget) {
    XCONFIG += formatwidget
    include($$PWD/../formatwidget.pri)
}

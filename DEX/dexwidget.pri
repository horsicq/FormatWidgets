INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

FORMS += \
    $$PWD/dexsectionheaderwidget.ui \
    $$PWD/dexwidget.ui \
    $$PWD/dialogdex.ui

HEADERS += \
    $$PWD/dexsectionheaderwidget.h \
    $$PWD/dexwidget.h \
    $$PWD/dex_defs.h \
    $$PWD/dialogdex.h \
    $$PWD/dexprocessdata.h

SOURCES += \
    $$PWD/dexsectionheaderwidget.cpp \
    $$PWD/dexwidget.cpp \
    $$PWD/dex_defs.cpp \
    $$PWD/dialogdex.cpp \
    $$PWD/dexprocessdata.cpp

!contains(XCONFIG, formatwidget) {
    XCONFIG += formatwidget
    include($$PWD/../formatwidget.pri)
}

!contains(XCONFIG, xdex) {
    XCONFIG += xdex
    include($$PWD/../../XDEX/xdex.pri)
}

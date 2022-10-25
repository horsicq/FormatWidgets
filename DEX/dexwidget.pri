INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

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

FORMS += \
    $$PWD/dexsectionheaderwidget.ui \
    $$PWD/dexwidget.ui \
    $$PWD/dialogdex.ui

!contains(XCONFIG, formatwidget) {
    XCONFIG += formatwidget
    include($$PWD/../formatwidget.pri)
}

DISTFILES += \
    $$PWD/dexwidget.cmake

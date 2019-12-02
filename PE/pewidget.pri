INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

!contains(XCONFIG, xpe) {
    XCONFIG += xpe
    include(../../Formats/xpe.pri)
}

!contains(XCONFIG, formatwidget) {
    XCONFIG += formatwidget
    include(../formatwidget.pri)
}

FORMS += \
    $$PWD/pewidget.ui \
    $$PWD/sectionheaderwidget.ui \
    $$PWD/dialogpe.ui

HEADERS += \
    $$PWD/pewidget.h \
    $$PWD/pe_defs.h \
    $$PWD/sectionheaderwidget.h \
    $$PWD/dialogpe.h

SOURCES += \
    $$PWD/pewidget.cpp \
    $$PWD/pe_defs.cpp \
    $$PWD/sectionheaderwidget.cpp \
    $$PWD/dialogpe.cpp

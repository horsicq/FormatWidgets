INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

FORMS += \
    $$PWD/pewidget.ui \
    $$PWD/sectionheaderwidget.ui \
    $$PWD/importheaderwidget.ui \
    $$PWD/dialogpe.ui

HEADERS += \
    $$PWD/pewidget.h \
    $$PWD/pe_defs.h \
    $$PWD/sectionheaderwidget.h \
    $$PWD/importheaderwidget.h \
    $$PWD/dialogpe.h

SOURCES += \
    $$PWD/pewidget.cpp \
    $$PWD/pe_defs.cpp \
    $$PWD/sectionheaderwidget.cpp \
    $$PWD/importheaderwidget.cpp \
    $$PWD/dialogpe.cpp

!contains(XCONFIG, xpe) {
    XCONFIG += xpe
    include($$PWD/../../Formats/xpe.pri)
}

!contains(XCONFIG, formatwidget) {
    XCONFIG += formatwidget
    include($$PWD/../formatwidget.pri)
}

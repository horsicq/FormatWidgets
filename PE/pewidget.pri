INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

FORMS += \
    $$PWD/debugheaderwidget.ui \
    $$PWD/pewidget.ui \
    $$PWD/sectionheaderwidget.ui \
    $$PWD/importheaderwidget.ui \
    $$PWD/dialogpe.ui

HEADERS += \
    $$PWD/debugheaderwidget.h \
    $$PWD/pewidget.h \
    $$PWD/pe_defs.h \
    $$PWD/sectionheaderwidget.h \
    $$PWD/importheaderwidget.h \
    $$PWD/dialogpe.h

SOURCES += \
    $$PWD/debugheaderwidget.cpp \
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

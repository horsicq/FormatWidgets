INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

FORMS += \
    $$PWD/exceptionheaderwidget.ui \
    $$PWD/pewidget.ui \
    $$PWD/sectionheaderwidget.ui \
    $$PWD/dialogpe.ui

HEADERS += \
    $$PWD/exceptionheaderwidget.h \
    $$PWD/pewidget.h \
    $$PWD/pe_defs.h \
    $$PWD/sectionheaderwidget.h \
    $$PWD/dialogpe.h \
    $$PWD/peprocessdata.h

SOURCES += \
    $$PWD/exceptionheaderwidget.cpp \
    $$PWD/pewidget.cpp \
    $$PWD/pe_defs.cpp \
    $$PWD/sectionheaderwidget.cpp \
    $$PWD/dialogpe.cpp \
    $$PWD/peprocessdata.cpp

!contains(XCONFIG, xpe) {
    XCONFIG += xpe
    include($$PWD/../../Formats/xpe.pri)
}

!contains(XCONFIG, formatwidget) {
    XCONFIG += formatwidget
    include($$PWD/../formatwidget.pri)
}

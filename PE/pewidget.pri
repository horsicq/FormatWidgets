INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

FORMS += \
    $$PWD/debugheaderwidget.ui \
    $$PWD/exceptionheaderwidget.ui \
    $$PWD/pewidget.ui \
    $$PWD/relocsheaderwidget.ui \
    $$PWD/sectionheaderwidget.ui \
    $$PWD/dialogpe.ui

HEADERS += \
    $$PWD/debugheaderwidget.h \
    $$PWD/exceptionheaderwidget.h \
    $$PWD/pewidget.h \
    $$PWD/pe_defs.h \
    $$PWD/relocsheaderwidget.h \
    $$PWD/sectionheaderwidget.h \
    $$PWD/dialogpe.h \
    $$PWD/peprocessdata.h

SOURCES += \
    $$PWD/debugheaderwidget.cpp \
    $$PWD/exceptionheaderwidget.cpp \
    $$PWD/pewidget.cpp \
    $$PWD/pe_defs.cpp \
    $$PWD/relocsheaderwidget.cpp \
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

INCLUDEPATH += $$PWD/PE
DEPENDPATH += $$PWD/PE

!contains(XCONFIG, xpe) {
    XCONFIG += xpe
    include(../Formats/xpe.pri)
}

!contains(XCONFIG, formatwidget) {
    XCONFIG += formatwidget
    include(formatwidget.pri)
}

FORMS += \
    $$PWD/PE/pewidget.ui \
    $$PWD/PE/sectionheaderwidget.ui \
    $$PWD/PE/dialogsectionheader.ui \
    $$PWD/PE/dialogpe.ui

HEADERS += \
    $$PWD/PE/pewidget.h \
    $$PWD/PE/pe_defs.h \
    $$PWD/PE/sectionheaderwidget.h \
    $$PWD/PE/dialogsectionheader.h \
    $$PWD/PE/dialogpe.h

SOURCES += \
    $$PWD/PE/pewidget.cpp \
    $$PWD/PE/pe_defs.cpp \
    $$PWD/PE/sectionheaderwidget.cpp \
    $$PWD/PE/dialogsectionheader.cpp\
    $$PWD/PE/dialogpe.cpp

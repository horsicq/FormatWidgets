INCLUDEPATH += $$PWD/PE
DEPENDPATH += $$PWD/PE

!contains(XCONFIG, qpe) {
    XCONFIG += qpe
    include(../Formats/qpe.pri)
}

!contains(XCONFIG, Format_widgets) {
    XCONFIG += Format_widgets
    include(Format_widgets.pri)
}

FORMS += \
    $$PWD/PE/dialogpe.ui \
    $$PWD/PE/dialogsectionheader.ui

HEADERS += \
    $$PWD/PE/dialogpe.h \
    $$PWD/PE/dialogsectionheader.h \
    $$PWD/PE/pe_defs.h

SOURCES += \
    $$PWD/PE/dialogpe.cpp \
    $$PWD/PE/dialogsectionheader.cpp \
    $$PWD/PE/pe_defs.cpp

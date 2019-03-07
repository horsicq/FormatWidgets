INCLUDEPATH += $$PWD/PE
DEPENDPATH += $$PWD/PE

!contains(XCONFIG, qlineedithex) {
    XCONFIG += qlineedithex
    include(../Controls/qlineedithex.pri)
}

!contains(XCONFIG, qcomboboxex) {
    XCONFIG += qcomboboxex
    include(../Controls/qcomboboxex.pri)
}

!contains(XCONFIG, qdatetimeeditx) {
    XCONFIG += qdatetimeeditx
    include(../Controls/qdatetimeeditx.pri)
}

!contains(XCONFIG, qhexview) {
    XCONFIG += qhexview
    include(../QHexView/qhexview.pri)
}

HEADERS += \
    $$PWD/dialogformat.h 

SOURCES += \
    $$PWD/dialogformat.cpp 

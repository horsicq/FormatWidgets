INCLUDEPATH += $$PWD/PE
DEPENDPATH += $$PWD/PE

!contains(XCONFIG, xlineedithex) {
    XCONFIG += xlineedithex
    include(../Controls/xlineedithex.pri)
}

!contains(XCONFIG, xcomboboxex) {
    XCONFIG += xcomboboxex
    include(../Controls/xcomboboxex.pri)
}

!contains(XCONFIG, xdatetimeeditx) {
    XCONFIG += xdatetimeeditx
    include(../Controls/xdatetimeeditx.pri)
}

!contains(XCONFIG, qhexview) {
    XCONFIG += qhexview
    include(../QHexView/qhexview.pri)
}

HEADERS += \
    $$PWD/formatwidget.h

SOURCES += \
    $$PWD/formatwidget.cpp


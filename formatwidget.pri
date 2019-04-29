INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

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

!contains(XCONFIG, searchstringswidget) {
    XCONFIG += searchstringswidget
    include(SearchStrings/searchstringswidget.pri)
}

HEADERS += \
    $$PWD/formatwidget.h \
    $$PWD/toolswidget.h

SOURCES += \
    $$PWD/formatwidget.cpp \
    $$PWD/toolswidget.cpp

FORMS += \
    $$PWD/toolswidget.ui


INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

FORMS += \
    $$PWD/searchstringswidget.ui

HEADERS += \
    $$PWD/searchstringswidget.h

SOURCES += \
    $$PWD/searchstringswidget.cpp

!contains(XCONFIG, dialogsearch) {
    XCONFIG += dialogsearch
    include(../FormatDialogs/dialogsearchstrings.pri)
}

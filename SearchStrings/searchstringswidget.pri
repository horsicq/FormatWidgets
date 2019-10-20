INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

FORMS += \
    $$PWD/searchstringswidget.ui

HEADERS += \
    $$PWD/searchstringswidget.h

SOURCES += \
    $$PWD/searchstringswidget.cpp

!contains(XCONFIG, dialogsearchstrings) {
    XCONFIG += dialogsearchstrings
    include(../../FormatDialogs/dialogsearchstrings.pri)
}

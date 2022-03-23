INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

FORMS += \
    $$PWD/dialogsearchstrings.ui \
    $$PWD/searchstringswidget.ui

HEADERS += \
    $$PWD/dialogsearchstrings.h \
    $$PWD/searchstringswidget.h

SOURCES += \
    $$PWD/dialogsearchstrings.cpp \
    $$PWD/searchstringswidget.cpp

!contains(XCONFIG, multisearch) {
    XCONFIG += multisearch
    include($$PWD/../MultiSearch/multisearch.pri)
}

!contains(XCONFIG, dialogeditstring) {
    XCONFIG += dialogeditstring
    include($$PWD/../../FormatDialogs/dialogeditstring.pri)
}

DISTFILES += \
    $$PWD/searchstringswidget.cmake

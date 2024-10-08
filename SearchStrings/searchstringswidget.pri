INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/dialogsearchstrings.h \
    $$PWD/searchstringswidget.h

SOURCES += \
    $$PWD/dialogsearchstrings.cpp \
    $$PWD/searchstringswidget.cpp

FORMS += \
    $$PWD/dialogsearchstrings.ui \
    $$PWD/searchstringswidget.ui

!contains(XCONFIG, multisearch) {
    XCONFIG += multisearch
    include($$PWD/../MultiSearch/multisearch.pri)
}

!contains(XCONFIG, dialogeditstring) {
    XCONFIG += dialogeditstring
    include($$PWD/../../FormatDialogs/dialogeditstring.pri)
}

!contains(XCONFIG, xtableview) {
    XCONFIG += xtableview
    include($$PWD/../../Controls/xtableview.pri)
}

DISTFILES += \
    $$PWD/searchstringswidget.cmake

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

FORMS += \
    $$PWD/dialogsearchsignatures.ui \
    $$PWD/searchsignatureswidget.ui

HEADERS += \
    $$PWD/dialogsearchsignatures.h \
    $$PWD/searchsignatureswidget.h

SOURCES += \
    $$PWD/dialogsearchsignatures.cpp \
    $$PWD/searchsignatureswidget.cpp
    
!contains(XCONFIG, multisearch) {
    XCONFIG += multisearch
    include($$PWD/../MultiSearch/multisearch.pri)
}

DISTFILES += \
    $$PWD/searchsignatureswidget.cmake

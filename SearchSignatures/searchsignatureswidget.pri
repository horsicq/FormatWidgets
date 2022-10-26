INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/dialogsearchsignatures.h \
    $$PWD/searchsignaturesoptionswidget.h \
    $$PWD/searchsignatureswidget.h

SOURCES += \
    $$PWD/dialogsearchsignatures.cpp \
    $$PWD/searchsignaturesoptionswidget.cpp \
    $$PWD/searchsignatureswidget.cpp

FORMS += \
    $$PWD/dialogsearchsignatures.ui \
    $$PWD/searchsignaturesoptionswidget.ui \
    $$PWD/searchsignatureswidget.ui
    
!contains(XCONFIG, multisearch) {
    XCONFIG += multisearch
    include($$PWD/../MultiSearch/multisearch.pri)
}

!contains(XCONFIG, xoptions) {
    XCONFIG += xoptions
    include($$PWD/../../XOptions/xoptions.pri)
}

!contains(XCONFIG, xshortcuts) {
    XCONFIG += xshortcuts
    include($$PWD/../../XShortcuts/xshortcuts.pri)
}

DISTFILES += \
    $$PWD/searchsignatureswidget.cmake

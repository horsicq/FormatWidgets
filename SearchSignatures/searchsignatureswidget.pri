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
    
include($$PWD/../MultiSearch/multisearch.pri)

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
    
include($$PWD/../MultiSearch/multisearch.pri)

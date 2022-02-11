QT       += concurrent

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

FORMS += \
    $$PWD/dialogmultisearchprocess.ui

HEADERS += \
    $$PWD/dialogmultisearchprocess.h \
    $$PWD/multisearch.h

SOURCES += \
    $$PWD/dialogmultisearchprocess.cpp \
    $$PWD/multisearch.cpp

!contains(XCONFIG, xoptions) {
    XCONFIG += xoptions
    include($$PWD/../../XOptions/xoptions.pri)
}

!contains(XCONFIG, xshortcuts) {
    XCONFIG += xshortcuts
    include($$PWD/../../XShortcuts/xshortcuts.pri)
}

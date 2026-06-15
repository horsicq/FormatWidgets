INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

!contains(XCONFIG, xformats) {
    XCONFIG += xformats
    include($$PWD/../../Formats/xformats.pri)
}

!contains(XCONFIG, xfcontrols) {
    XCONFIG += xfcontrols
    include($$PWD/../../Controls/XFControls/xfcontrols.pri)
}

!contains(XCONFIG, xshortcuts) {
    XCONFIG += xshortcuts
    include($$PWD/../../XShortcuts/xshortcuts.pri)
}

HEADERS += \
    $$PWD/xfwidget.h \
    $$PWD/xfwidgetadvanced.h

SOURCES += \
    $$PWD/xfwidget.cpp \
    $$PWD/xfwidgetadvanced.cpp

FORMS += \
    $$PWD/xfwidget.ui \
    $$PWD/xfwidgetadvanced.ui

DISTFILES += \
    $$PWD/xfwidgets.cmake

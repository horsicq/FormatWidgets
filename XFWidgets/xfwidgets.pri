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

HEADERS += \
    $$PWD/xfwidget.h

SOURCES += \
    $$PWD/xfwidget.cpp

FORMS += \
    $$PWD/xfwidget.ui

DISTFILES += \
    $$PWD/xfwidgets.cmake

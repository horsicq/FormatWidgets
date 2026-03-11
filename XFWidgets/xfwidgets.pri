INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

!contains(XCONFIG, xfcontrols) {
    XCONFIG += xfcontrols
    include($$PWD/../../Controls/XFControls/xfcontrols.pri)
}

HEADERS += \
    $$PWD/xfwidget.h

SOURCES += \
    $$PWD/xfwidget.cpp

DISTFILES += \
    $$PWD/xfwidgets.cmake

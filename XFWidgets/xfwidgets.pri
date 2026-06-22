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
    $$PWD/xfwidgetadvanced.h \
    $$PWD/xfwidget_header.h \
    $$PWD/xfwidget_table.h

SOURCES += \
    $$PWD/xfwidget.cpp \
    $$PWD/xfwidgetadvanced.cpp \
    $$PWD/xfwidget_header.cpp \
    $$PWD/xfwidget_table.cpp

FORMS += \
    $$PWD/xfwidget.ui \
    $$PWD/xfwidgetadvanced.ui \
    $$PWD/xfwidget_header.ui \
    $$PWD/xfwidget_table.ui

DISTFILES += \
    $$PWD/xfwidgets.cmake

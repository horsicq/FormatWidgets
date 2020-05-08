INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

!contains(XCONFIG, xle) {
    XCONFIG += xle
    include(../../Formats/xle.pri)
}

!contains(XCONFIG, formatwidget) {
    XCONFIG += formatwidget
    include(../formatwidget.pri)
}

FORMS += \
    $$PWD/lewidget.ui \
    $$PWD/dialogle.ui

HEADERS += \
    $$PWD/lewidget.h \
    $$PWD/le_defs.h \
    $$PWD/dialogle.h

SOURCES += \
    $$PWD/lewidget.cpp \
    $$PWD/le_defs.cpp \
    $$PWD/dialogle.cpp

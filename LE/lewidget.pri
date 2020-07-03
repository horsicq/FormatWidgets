INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

FORMS += \
    $$PWD/lesectionheaderwidget.ui \
    $$PWD/lewidget.ui \
    $$PWD/dialogle.ui

HEADERS += \
    $$PWD/leprocessdata.h \
    $$PWD/lesectionheaderwidget.h \
    $$PWD/lewidget.h \
    $$PWD/le_defs.h \
    $$PWD/dialogle.h

SOURCES += \
    $$PWD/leprocessdata.cpp \
    $$PWD/lesectionheaderwidget.cpp \
    $$PWD/lewidget.cpp \
    $$PWD/le_defs.cpp \
    $$PWD/dialogle.cpp

!contains(XCONFIG, formatwidget) {
    XCONFIG += formatwidget
    include($$PWD/../formatwidget.pri)
}

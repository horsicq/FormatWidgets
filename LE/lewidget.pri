INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

FORMS += \
    $$PWD/lewidget.ui \
    $$PWD/dialogle.ui \
    $$PWD/sectionheaderwidget.ui

HEADERS += \
    $$PWD/leprocessdata.h \
    $$PWD/lewidget.h \
    $$PWD/le_defs.h \
    $$PWD/dialogle.h \
    $$PWD/sectionheaderwidget.h

SOURCES += \
    $$PWD/leprocessdata.cpp \
    $$PWD/lewidget.cpp \
    $$PWD/le_defs.cpp \
    $$PWD/dialogle.cpp \
    $$PWD/sectionheaderwidget.cpp

!contains(XCONFIG, formatwidget) {
    XCONFIG += formatwidget
    include($$PWD/../formatwidget.pri)
}

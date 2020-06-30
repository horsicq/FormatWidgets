INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

FORMS += \
    $$PWD/newidget.ui \
    $$PWD/dialogne.ui \
    $$PWD/sectionheaderwidget.ui

HEADERS += \
    $$PWD/neprocessdata.h \
    $$PWD/newidget.h \
    $$PWD/ne_defs.h \
    $$PWD/dialogne.h \
    $$PWD/sectionheaderwidget.h

SOURCES += \
    $$PWD/neprocessdata.cpp \
    $$PWD/newidget.cpp \
    $$PWD/ne_defs.cpp \
    $$PWD/dialogne.cpp \
    $$PWD/sectionheaderwidget.cpp

!contains(XCONFIG, formatwidget) {
    XCONFIG += formatwidget
    include($$PWD/../formatwidget.pri)
}

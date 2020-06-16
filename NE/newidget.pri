INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

!contains(XCONFIG, xne) {
    XCONFIG += xne
    include($$PWD/../../Formats/xne.pri)
}

!contains(XCONFIG, formatwidget) {
    XCONFIG += formatwidget
    include($$PWD/../formatwidget.pri)
}

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

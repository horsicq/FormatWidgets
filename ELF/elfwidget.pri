INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

FORMS += \
    $$PWD/dynamicarraytagwidget.ui \
    $$PWD/elfwidget.ui \
    $$PWD/dialogelf.ui \
    $$PWD/sectionheaderwidget.ui

HEADERS += \
    $$PWD/dynamicarraytagwidget.h \
    $$PWD/elfwidget.h \
    $$PWD/elf_defs.h \
    $$PWD/dialogelf.h \
    $$PWD/sectionheaderwidget.h \
    $$PWD/elfprocessdata.h

SOURCES += \
    $$PWD/dynamicarraytagwidget.cpp \
    $$PWD/elfwidget.cpp \
    $$PWD/elf_defs.cpp \
    $$PWD/dialogelf.cpp \
    $$PWD/sectionheaderwidget.cpp \
    $$PWD/elfprocessdata.cpp

!contains(XCONFIG, xelf) {
    XCONFIG += xelf
    include($$PWD/../../Formats/xelf.pri)
}

!contains(XCONFIG, formatwidget) {
    XCONFIG += formatwidget
    include($$PWD/../formatwidget.pri)
}

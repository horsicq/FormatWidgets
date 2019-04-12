INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

!contains(XCONFIG, xelf) {
    XCONFIG += xelf
    include(../../Formats/xelf.pri)
}

!contains(XCONFIG, formatwidget) {
    XCONFIG += formatwidget
    include(../formatwidget.pri)
}

FORMS += \
    $$PWD/elfwidget.ui \
    $$PWD/dialogelf.ui

HEADERS += \
    $$PWD/elfwidget.h \
    $$PWD/elf_defs.h \
    $$PWD/dialogelf.h

SOURCES += \
    $$PWD/elfwidget.cpp \
    $$PWD/elf_defs.cpp \
    $$PWD/dialogelf.cpp

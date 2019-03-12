INCLUDEPATH += $$PWD/ELF
DEPENDPATH += $$PWD/ELF

!contains(XCONFIG, qelf) {
    XCONFIG += qelf
    include(../Formats/qelf.pri)
}

!contains(XCONFIG, formatwidget) {
    XCONFIG += formatwidget
    include(formatwidget.pri)
}

FORMS += \
    $$PWD/ELF/elfwidget.ui \
    $$PWD/ELF/dialogelf.ui

HEADERS += \
    $$PWD/ELF/elfwidget.h \
    $$PWD/ELF/elf_defs.h \
    $$PWD/ELF/dialogelf.h

SOURCES += \
    $$PWD/ELF/elfwidget.cpp \
    $$PWD/ELF/elf_defs.cpp \
    $$PWD/ELF/dialogelf.cpp

INCLUDEPATH += $$PWD/ELF
DEPENDPATH += $$PWD/ELF

!contains(XCONFIG, qelf) {
    XCONFIG += qelf
    include(../Formats/qelf.pri)
}

!contains(XCONFIG, Format_widgets) {
    XCONFIG += Format_widgets
    include(Format_widgets.pri)
}

FORMS += \
    $$PWD/ELF/dialogelf.ui

HEADERS += \
    $$PWD/ELF/dialogelf.h \
    $$PWD/ELF/elf_defs.h

SOURCES += \
    $$PWD/ELF/dialogelf.cpp \
    $$PWD/ELF/elf_defs.cpp

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

FORMS += \
    $$PWD/elfsectionheaderwidget.ui \
    $$PWD/elfwidget.ui \
    $$PWD/dialogelf.ui

HEADERS += \
    $$PWD/elfsectionheaderwidget.h \
    $$PWD/elfwidget.h \
    $$PWD/elf_defs.h \
    $$PWD/dialogelf.h \
    $$PWD/elfprocessdata.h

SOURCES += \
    $$PWD/elfsectionheaderwidget.cpp \
    $$PWD/elfwidget.cpp \
    $$PWD/elf_defs.cpp \
    $$PWD/dialogelf.cpp \
    $$PWD/elfprocessdata.cpp

!contains(XCONFIG, formatwidget) {
    XCONFIG += formatwidget
    include($$PWD/../formatwidget.pri)
}

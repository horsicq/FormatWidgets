INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

FORMS += \
    $$PWD/binarywidget.ui \
    $$PWD/dialogbinary.ui

HEADERS += \
    $$PWD/binarywidget.h \
    $$PWD/binary_defs.h \
    $$PWD/dialogbinary.h

SOURCES += \
    $$PWD/binarywidget.cpp \
    $$PWD/binary_defs.cpp \
    $$PWD/dialogbinary.cpp

!contains(XCONFIG, formatwidget) {
    XCONFIG += formatwidget
    include($$PWD/../formatwidget.pri)
}

DISTFILES += \
    $$PWD/binarywidget.cmake

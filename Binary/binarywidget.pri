INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

!contains(XCONFIG, xbinary) {
    XCONFIG += xbinary
    include(../Formats/xbinary.pri)
}

!contains(XCONFIG, formatwidget) {
    XCONFIG += formatwidget
    include(../formatwidget.pri)
}

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

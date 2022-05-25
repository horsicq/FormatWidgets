INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

!contains(XCONFIG, formatwidget) {
    XCONFIG += formatwidget
    include($$PWD/../formatwidget.pri)
}

FORMS += \
    $$PWD/dialogmachofat.ui \
    $$PWD/machofatwidget.ui

HEADERS += \
    $$PWD/dialogmachofat.h \
    $$PWD/machofatwidget.h

SOURCES += \
    $$PWD/dialogmachofat.cpp \
    $$PWD/machofatwidget.cpp

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/dialogmachofat.h \
    $$PWD/machofatwidget.h

SOURCES += \
    $$PWD/dialogmachofat.cpp \
    $$PWD/machofatwidget.cpp

FORMS += \
    $$PWD/dialogmachofat.ui \
    $$PWD/machofatwidget.ui

!contains(XCONFIG, formatwidget) {
    XCONFIG += formatwidget
    include($$PWD/../formatwidget.pri)
}

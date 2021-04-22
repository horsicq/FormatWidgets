INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

!contains(XCONFIG, formatwidget) {
    XCONFIG += formatwidget
    include($$PWD/../formatwidget.pri)
}

FORMS += \
    $$PWD/machofatwidget.ui

HEADERS += \
    $$PWD/machofatwidget.h

SOURCES += \
    $$PWD/machofatwidget.cpp

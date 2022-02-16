INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

FORMS += \
    $$PWD/pdfwidget.ui \
    $$PWD/dialogpdf.ui

HEADERS += \
    $$PWD/pdfwidget.h \
    $$PWD/pdf_defs.h \
    $$PWD/dialogpdf.h

SOURCES += \
    $$PWD/pdfwidget.cpp \
    $$PWD/pdf_defs.cpp \
    $$PWD/dialogpdf.cpp

!contains(XCONFIG, formatwidget) {
    XCONFIG += formatwidget
    include($$PWD/../formatwidget.pri)
}

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

!contains(XCONFIG, use_dex) {
    XCONFIG += use_dex
}

!contains(XCONFIG, use_archive) {
    XCONFIG += use_archive
}

HEADERS += \
    $$PWD/dialogmodelinfo.h \
    $$PWD/dialogprocessdata.h \
    $$PWD/processdata.h \
    $$PWD/formatwidget.h \
    $$PWD/toolswidget.h \
    $$PWD/invwidget.h \
    $$PWD/formatwidget_def.h \
    $$PWD/dialogsectionheader.h

SOURCES += \
    $$PWD/dialogmodelinfo.cpp \
    $$PWD/dialogprocessdata.cpp \
    $$PWD/processdata.cpp \
    $$PWD/formatwidget.cpp \
    $$PWD/toolswidget.cpp \
    $$PWD/invwidget.cpp \
    $$PWD/dialogsectionheader.cpp

FORMS += \
    $$PWD/dialogmodelinfo.ui \
    $$PWD/dialogprocessdata.ui \
    $$PWD/toolswidget.ui \
    $$PWD/dialogsectionheader.ui

!contains(XCONFIG, xlineedithex) {
    XCONFIG += xlineedithex
    include($$PWD/../Controls/xlineedithex.pri)
}

!contains(XCONFIG, xcomboboxex) {
    XCONFIG += xcomboboxex
    include($$PWD/../Controls/xcomboboxex.pri)
}

!contains(XCONFIG, xdatetimeeditx) {
    XCONFIG += xdatetimeeditx
    include($$PWD/../Controls/xdatetimeeditx.pri)
}

!contains(XCONFIG, xhexview) {
    XCONFIG += xhexview
    include($$PWD/../XHexView/xhexview.pri)
}

!contains(XCONFIG, searchstringswidget) {
    XCONFIG += searchstringswidget
    include($$PWD/SearchStrings/searchstringswidget.pri)
}

!contains(XCONFIG, searchsignatureswidget) {
    XCONFIG += searchsignatureswidget
    include($$PWD/SearchSignatures/searchsignatureswidget.pri)
}

!contains(XCONFIG, xmemorymapwidget) {
    XCONFIG += xmemorymapwidget
    include($$PWD/../XMemoryMapWidget/xmemorymapwidget.pri)
}

!contains(XCONFIG, xentropywidget) {
    XCONFIG += xentropywidget
    include($$PWD/../XEntropyWidget/xentropywidget.pri)
}

!contains(XCONFIG, heurwidget) {
    XCONFIG += heurwidget
    include($$PWD/../StaticScan/heurwidget.pri)
}

!contains(XCONFIG, xhashwidget) {
    XCONFIG += xhashwidget
    include($$PWD/../XHashWidget/xhashwidget.pri)
}

!contains(XCONFIG, xdisasmview) {
    XCONFIG += xdisasmview
    include($$PWD/../XDisasmView/xdisasmview.pri)
}

!contains(XCONFIG, xdemanglewidget) {
    XCONFIG += xdemanglewidget
    include($$PWD/../XDemangleWidget/xdemanglewidget.pri)
}

!contains(XCONFIG, xfileinfowidget) {
    XCONFIG += xfileinfowidget
    include($$PWD/../XFileInfo/xfileinfowidget.pri)
}

DISTFILES += \
    $$PWD/LICENSE \
    $$PWD/README.md \
    $$PWD/formatwidget.cmake

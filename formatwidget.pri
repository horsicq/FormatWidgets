INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

!contains(XCONFIG, use_dex) {
    XCONFIG += use_dex
}

!contains(XCONFIG, use_archive) {
    XCONFIG += use_archive
}

HEADERS += \
    $$PWD/dialogprocessdata.h \
    $$PWD/processdata.h \
    $$PWD/formatwidget.h \
    $$PWD/toolswidget.h \
    $$PWD/invwidget.h \
    $$PWD/formatwidget_def.h \
    $$PWD/dialogsectionheader.h

SOURCES += \
    $$PWD/dialogprocessdata.cpp \
    $$PWD/processdata.cpp \
    $$PWD/formatwidget.cpp \
    $$PWD/toolswidget.cpp \
    $$PWD/invwidget.cpp \
    $$PWD/dialogsectionheader.cpp

FORMS += \
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

!contains(XCONFIG, qhexview) {
    XCONFIG += qhexview
    include($$PWD/../QHexView/qhexview.pri)
}

!contains(XCONFIG, xhexview) {
    XCONFIG += xhexview
    include($$PWD/../XHexView/xhexview.pri)
}

!contains(XCONFIG, searchstringswidget) {
    XCONFIG += searchstringswidget
    include($$PWD/SearchStrings/searchstringswidget.pri)
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

contains(XCONFIG, use_disasm) {
    DEFINES += USE_DISASM

    !contains(XCONFIG, xdisasm) {
        XCONFIG += xdisasm
        include($$PWD/../XDisasm/xdisasm.pri)
    }

    !contains(XCONFIG, xdisasmview) {
        XCONFIG += xdisasmview
        include($$PWD/../XDisasmView/xdisasmview.pri)
    }
}

contains(XCONFIG, use_extrabuttons) {
    DEFINES += USE_EXTRABUTTONS
}

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

!contains(XCONFIG, use_dex) {
    XCONFIG += use_dex
}

!contains(XCONFIG, use_pdf) {
    XCONFIG += use_pdf
}

!contains(XCONFIG, use_archive) {
    XCONFIG += use_archive
}

!contains(XCONFIG, use_yara) {
    XCONFIG += use_yara
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
    $$PWD/toolswidget.ui \
    $$PWD/dialogsectionheader.ui

!contains(XCONFIG, xlineedithex) {
    XCONFIG += xlineedithex
    include($$PWD/../Controls/xlineedithex.pri)
}

!contains(XCONFIG, xtableview) {
    XCONFIG += xtableview
    include($$PWD/../Controls/xtableview.pri)
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

!contains(XCONFIG, searchvalueswidget) {
    XCONFIG += searchvalueswidget
    include($$PWD/SearchValues/searchvalueswidget.pri)
}

!contains(XCONFIG, xmemorymapwidget) {
    XCONFIG += xmemorymapwidget
    include($$PWD/../XMemoryMapWidget/xmemorymapwidget.pri)
}

!contains(XCONFIG, xentropywidget) {
    XCONFIG += xentropywidget
    include($$PWD/../XEntropyWidget/xentropywidget.pri)
}

!contains(XCONFIG, die_widget) {
    XCONFIG += die_widget
    include($$PWD/../die_widget/die_widget.pri)
}

!contains(XCONFIG, nfd_widget) {
    XCONFIG += nfd_widget
    include($$PWD/../nfd_widget/nfd_widget.pri)
}

!contains(XCONFIG, yara_widget) {
    XCONFIG += yara_widget
    include($$PWD/../yara_widget/yara_widget.pri)
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

!contains(XCONFIG, xvirustotalwidget) {
    XCONFIG += xvirustotalwidget
    include($$PWD/../XOnlineTools/xvirustotalwidget.pri)
}

!contains(XCONFIG, xextractorwidget) {
    XCONFIG += xextractorwidget
    include($$PWD/../XExtractorWidget/xextractorwidget.pri)
}

!contains(XCONFIG, xvisualizationwidget) {
    XCONFIG += xvisualizationwidget
    include($$PWD/../XVisualizationWidget/xvisualizationwidget.pri)
}

!contains(XCONFIG, xoptionswidget) {
    XCONFIG += xoptionswidget
    include($$PWD/../XOptions/xoptionswidget.pri)
}

DISTFILES += \
    $$PWD/LICENSE \
    $$PWD/README.md \
    $$PWD/formatwidget.cmake

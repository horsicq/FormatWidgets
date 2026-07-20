INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

!contains(XCONFIG, xformats) {
    XCONFIG += xformats
    include($$PWD/../../Formats/xformats.pri)
}

!contains(XCONFIG, xfcontrols) {
    XCONFIG += xfcontrols
    include($$PWD/../../Controls/XFControls/xfcontrols.pri)
}

!contains(XCONFIG, xmodel_binary) {
    XCONFIG += xmodel_binary
    include($$PWD/../../Controls/xmodel_binary.pri)
}

!contains(XCONFIG, xshortcuts) {
    XCONFIG += xshortcuts
    include($$PWD/../../XShortcuts/xshortcuts.pri)
}

!contains(XCONFIG, xdialogprocess) {
    XCONFIG += xdialogprocess
    include($$PWD/../../FormatDialogs/xdialogprocess.pri)
}

!contains(XCONFIG, xregionswidget) {
    XCONFIG += xregionswidget
    include($$PWD/../../XRegionsWidget/xregionswidget.pri)
}

!contains(XCONFIG, xvisualizationwidget) {
    XCONFIG += xvisualizationwidget
    include($$PWD/../../XVisualizationWidget/xvisualizationwidget.pri)
}

!contains(XCONFIG, xhexview) {
    XCONFIG += xhexview
    include($$PWD/../../XHexView/xhexview.pri)
}

!contains(XCONFIG, xdisasmview) {
    XCONFIG += xdisasmview
    include($$PWD/../../XDisasmView/xdisasmview.pri)
}

!contains(XCONFIG, nfd_widget) {
    XCONFIG += nfd_widget
    include($$PWD/../../nfd_widget/nfd_widget.pri)
}

!contains(XCONFIG, xhashwidget) {
    XCONFIG += xhashwidget
    include($$PWD/../../XHashWidget/xhashwidget.pri)
}

!contains(XCONFIG, searchsignatureswidget) {
    XCONFIG += searchsignatureswidget
    include($$PWD/../SearchSignatures/searchsignatureswidget.pri)
}

!contains(XCONFIG, xmemorymapwidget) {
    XCONFIG += xmemorymapwidget
    include($$PWD/../../XMemoryMapWidget/xmemorymapwidget.pri)
}

!contains(XCONFIG, xentropywidget) {
    XCONFIG += xentropywidget
    include($$PWD/../../XEntropyWidget/xentropywidget.pri)
}

!contains(XCONFIG, xextractorwidget) {
    XCONFIG += xextractorwidget
    include($$PWD/../../XExtractorWidget/xextractorwidget.pri)
}

!contains(XCONFIG, searchvalueswidget) {
    XCONFIG += searchvalueswidget
    include($$PWD/../SearchValues/searchvalueswidget.pri)
}

HEADERS += \
    $$PWD/xfwidget.h \
    $$PWD/xfwidgetadvanced.h \
    $$PWD/xfwidget_header.h \
    $$PWD/xfwidget_table.h \
    $$PWD/xfwidget_strings.h \
    $$PWD/xfwidget_regions.h \
    $$PWD/xfwidget_visualization.h \
    $$PWD/xfwidget_hex.h \
    $$PWD/xfwidget_disasm.h \
    $$PWD/xfwidget_nfdscan.h \
    $$PWD/xfwidget_hash.h \
    $$PWD/xfwidget_signatures.h \
    $$PWD/xfwidget_memorymap.h \
    $$PWD/xfwidget_entropy.h \
    $$PWD/xfwidget_extractor.h \
    $$PWD/xfwidget_search.h

SOURCES += \
    $$PWD/xfwidget.cpp \
    $$PWD/xfwidgetadvanced.cpp \
    $$PWD/xfwidget_header.cpp \
    $$PWD/xfwidget_table.cpp \
    $$PWD/xfwidget_strings.cpp \
    $$PWD/xfwidget_regions.cpp \
    $$PWD/xfwidget_visualization.cpp \
    $$PWD/xfwidget_hex.cpp \
    $$PWD/xfwidget_disasm.cpp \
    $$PWD/xfwidget_nfdscan.cpp \
    $$PWD/xfwidget_hash.cpp \
    $$PWD/xfwidget_signatures.cpp \
    $$PWD/xfwidget_memorymap.cpp \
    $$PWD/xfwidget_entropy.cpp \
    $$PWD/xfwidget_extractor.cpp \
    $$PWD/xfwidget_search.cpp

FORMS += \
    $$PWD/xfwidget.ui \
    $$PWD/xfwidgetadvanced.ui \
    $$PWD/xfwidget_header.ui \
    $$PWD/xfwidget_table.ui \
    $$PWD/xfwidget_strings.ui \
    $$PWD/xfwidget_regions.ui

DISTFILES += \
    $$PWD/xfwidgets.cmake

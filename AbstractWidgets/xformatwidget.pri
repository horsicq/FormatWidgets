INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

!contains(XCONFIG, abstractwidgets) {
    XCONFIG += abstractwidgets
    include($$PWD/abstractwidgets.pri)
}

!contains(XCONFIG, xcppfilt) {
    XCONFIG += xcppfilt
    include($$PWD/../../XCppfilt/xcppfilt.pri)
}

!contains(XCONFIG, searchsignatureswidget) {
    XCONFIG += searchsignatureswidget
    include($$PWD/../SearchSignatures/searchsignatureswidget.pri)
}

!contains(XCONFIG, searchstringswidget) {
    XCONFIG += searchstringswidget
    include($$PWD/../SearchStrings/searchstringswidget.pri)
}

!contains(XCONFIG, searchvalueswidget) {
    XCONFIG += searchvalueswidget
    include($$PWD/../SearchValues/searchvalueswidget.pri)
}

!contains(XCONFIG, xlineedithex) {
    XCONFIG += xlineedithex
    include($$PWD/../../Controls/xlineedithex.pri)
}

!contains(XCONFIG, xcomboboxex) {
    XCONFIG += xcomboboxex
    include($$PWD/../../Controls/xcomboboxex.pri)
}

!contains(XCONFIG, xtableview) {
    XCONFIG += xtableview
    include($$PWD/../../Controls/xtableview.pri)
}

!contains(XCONFIG, xdatetimeeditx) {
    XCONFIG += xdatetimeeditx
    include($$PWD/../../Controls/xdatetimeeditx.pri)
}

!contains(XCONFIG, xhexview) {
    XCONFIG += xhexview
    include($$PWD/../../XHexView/xhexview.pri)
}

!contains(XCONFIG, xmemorymapwidget) {
    XCONFIG += xmemorymapwidget
    include($$PWD/../../XMemoryMapWidget/xmemorymapwidget.pri)
}

!contains(XCONFIG, xentropywidget) {
    XCONFIG += xentropywidget
    include($$PWD/../../XEntropyWidget/xentropywidget.pri)
}

!contains(XCONFIG, nfd_widget) {
    XCONFIG += nfd_widget
    include($$PWD/../../nfd_widget/nfd_widget.pri)
}

!contains(XCONFIG, yara_widget) {
    XCONFIG += yara_widget
    include($$PWD/../../yara_widget/yara_widget.pri)
}

!contains(XCONFIG, die_widget) {
    XCONFIG += die_widget
    include($$PWD/../../die_widget/die_widget.pri)
}

!contains(XCONFIG, xhashwidget) {
    XCONFIG += xhashwidget
    include($$PWD/../../XHashWidget/xhashwidget.pri)
}

!contains(XCONFIG, xdisasmview) {
    XCONFIG += xdisasmview
    include($$PWD/../../XDisasmView/xdisasmview.pri)
}

!contains(XCONFIG, xdemanglewidget) {
    XCONFIG += xdemanglewidget
    include($$PWD/../../XDemangleWidget/xdemanglewidget.pri)
}

!contains(XCONFIG, xfileinfowidget) {
    XCONFIG += xfileinfowidget
    include($$PWD/../../XFileInfo/xfileinfowidget.pri)
}

!contains(XCONFIG, xformats) {
    XCONFIG += xformats
    include($$PWD/../../Formats/xformats.pri)
}

!contains(XCONFIG, xdex) {
    XCONFIG += xdex
    include($$PWD/../../XDEX/xdex.pri)
}

!contains(XCONFIG, xarchives) {
    XCONFIG += xarchives
    include($$PWD/../../XArchive/xarchives.pri)
}

!contains(XCONFIG, xpdf) {
    XCONFIG += xpdf
    include($$PWD/../../XPDF/xpdf.pri)
}

!contains(XCONFIG, xsymbolswidget) {
    XCONFIG += xsymbolswidget
    include($$PWD/../../XSymbolsWidget/xsymbolswidget.pri)
}

!contains(XCONFIG, xregionswidget) {
    XCONFIG += xregionswidget
    include($$PWD/../../XRegionsWidget/xregionswidget.pri)
}

!contains(XCONFIG, xvirustotalwidget) {
    XCONFIG += xvirustotalwidget
    include($$PWD/../../XOnlineTools/xvirustotalwidget.pri)
}

!contains(XCONFIG, xextractorwidget) {
    XCONFIG += xextractorwidget
    include($$PWD/../../XExtractorWidget/xextractorwidget.pri)
}

!contains(XCONFIG, xvisualizationwidget) {
    XCONFIG += xvisualizationwidget
    include($$PWD/../../XVisualizationWidget/xvisualizationwidget.pri)
}

!contains(XCONFIG, xoptionswidget) {
    XCONFIG += xoptionswidget
    include($$PWD/../../XOptions/xoptionswidget.pri)
}

!contains(XCONFIG, xyara) {
    XCONFIG += xyara
    include($$PWD/../../XYara/xyara.pri)
}

!contains(XCONFIG, dialogwidget) {
    XCONFIG += dialogwidget
    include($$PWD/../../FormatDialogs/dialogwidget.pri)
}


DISTFILES += \
    $$PWD/xformatwidget.cmake
    

include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${CMAKE_CURRENT_LIST_DIR}/Generic)

if (NOT DEFINED ABSTRACTWIDGETS_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/abstractwidgets.cmake)
    set(XFORMATWIDGET_SOURCES ${XFORMATWIDGET_SOURCES} ${ABSTRACTWIDGETS_SOURCES})
endif()
if (NOT DEFINED XCPPFILT_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../../XCppfilt/xcppfilt.cmake)
    set(XFORMATWIDGET_SOURCES ${XFORMATWIDGET_SOURCES} ${XCPPFILT_SOURCES})
endif()
if (NOT DEFINED SEARCHSIGNATURESWIDGET_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../SearchSignatures/searchsignatureswidget.cmake)
    set(XFORMATWIDGET_SOURCES ${XFORMATWIDGET_SOURCES} ${SEARCHSIGNATURESWIDGET_SOURCES})
endif()
if (NOT DEFINED SEARCHSTRINGSWIDGET_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../SearchStrings/searchstringswidget.cmake)
    set(XFORMATWIDGET_SOURCES ${XFORMATWIDGET_SOURCES} ${SEARCHSTRINGSWIDGET_SOURCES})
endif()
if (NOT DEFINED SEARCHVALUESWIDGET_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../SearchValues/searchvalueswidget.cmake)
    set(XFORMATWIDGET_SOURCES ${XFORMATWIDGET_SOURCES} ${SEARCHVALUESWIDGET_SOURCES})
endif()
if (NOT DEFINED XLINEEDITHEX_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../../Controls/xlineedithex.cmake)
    set(XFORMATWIDGET_SOURCES ${XFORMATWIDGET_SOURCES} ${XLINEEDITHEX_SOURCES})
endif()
if (NOT DEFINED XCOMBOBOXEX_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../../Controls/xcomboboxex.cmake)
    set(XFORMATWIDGET_SOURCES ${XFORMATWIDGET_SOURCES} ${XCOMBOBOXEX_SOURCES})
endif()
if (NOT DEFINED XTABLEVIEW_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../../Controls/xtableview.cmake)
    set(XFORMATWIDGET_SOURCES ${XFORMATWIDGET_SOURCES} ${XTABLEVIEW_SOURCES})
endif()
if (NOT DEFINED XDATETIMEEDITX_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../../Controls/xdatetimeeditx.cmake)
    set(XFORMATWIDGET_SOURCES ${XFORMATWIDGET_SOURCES} ${XDATETIMEEDITX_SOURCES})
endif()
if (NOT DEFINED XHEXVIEW_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../../XHexView/xhexview.cmake)
    set(XFORMATWIDGET_SOURCES ${XFORMATWIDGET_SOURCES} ${XHEXVIEW_SOURCES})
endif()

include(${CMAKE_CURRENT_LIST_DIR}/../../XMemoryMapWidget/xmemorymapwidget.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../XEntropyWidget/xentropywidget.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../nfd_widget/nfd_widget.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../yara_widget/yara_widget.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../die_widget/die_widget.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../XHashWidget/xhashwidget.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../XDisasmView/xdisasmview.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../XDemangleWidget/xdemanglewidget.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../XFileInfo/xfileinfowidget.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../Controls/xdatetimeeditx.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../Formats/xformats.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../XDEX/xdex.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../XArchive/xarchives.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../XPDF/xpdf.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../XSymbolsWidget/xsymbolswidget.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../XOnlineTools/xvirustotalwidget.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../XExtractorWidget/xextractorwidget.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../XVisualizationWidget/xvisualizationwidget.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../XOptions/xoptionswidget.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../XYara/xyara.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../FormatDialogs/dialogwidget.cmake)

set(XFORMATWIDGET_SOURCES
    ${XFORMATWIDGET_SOURCES}
    ${ABSTRACTWIDGETS_SOURCES}
    ${XCPPFILT_SOURCES}
    ${SEARCHSIGNATURESWIDGET_SOURCES}
    ${SEARCHSTRINGSWIDGET_SOURCES}
    ${SEARCHVALUESWIDGET_SOURCES}
    ${XLINEEDITHEX_SOURCES}
    ${XCOMBOBOXEX_SOURCES}
    ${XTABLEVIEW_SOURCES}
    ${XDATETIMEEDITX_SOURCES}
    ${XHEXVIEW_SOURCES}
    ${XMEMORYMAPWIDGET_SOURCES}
    ${XENTROPYWIDGET_SOURCES}
    ${NFD_WIDGET_SOURCES}
    ${YARA_WIDGET_SOURCES}
    ${DIE_WIDGET_SOURCES}
    ${XHASHWIDGET_SOURCES}
    ${XDISASMVIEW_SOURCES}
    ${XDEMANGLEWIDGET_SOURCES}
    ${XFILEINFOWIDGET_SOURCES}
    ${XDATETIMEEDITX_SOURCES}
    ${XFORMATS_SOURCES}
    ${XDEX_SOURCES}
    ${XARCHIVES_SOURCES}
    ${XPDF_SOURCES}
    ${XSYMBOLSWIDGET_SOURCES}
    ${XVIRUSTOTALWIDGET_SOURCES}
    ${XEXTRACTORWIDGET_SOURCES}
    ${XVISUALIZATIONWIDGET_SOURCES}
    ${XOPTIONSWIDGET_SOURCES}
    ${XYARA_SOURCES}
    ${DIALOGWIDGET_SOURCES}
)

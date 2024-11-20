include_directories(${CMAKE_CURRENT_LIST_DIR})

include(${CMAKE_CURRENT_LIST_DIR}/../../XCppfilt/xcppfilt.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../SearchSignatures/searchsignatureswidget.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../SearchStrings/searchstringswidget.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../SearchValues/searchvalueswidget.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../Controls/xlineedithex.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../Controls/xcomboboxex.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../Controls/xtableview.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../Controls/xdatetimeeditx.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../XHexView/xhexview.cmake)
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

set(ABSTRACTWIDGETS_SOURCES
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
    ${CMAKE_CURRENT_LIST_DIR}/dialogprocessdata.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogprocessdata.h
    ${CMAKE_CURRENT_LIST_DIR}/dialogmodelinfo.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogmodelinfo.h
    ${CMAKE_CURRENT_LIST_DIR}/dialogmodelinfo.ui
    ${CMAKE_CURRENT_LIST_DIR}/formatwidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/formatwidget.h
    ${CMAKE_CURRENT_LIST_DIR}/formatwidget_def.h
    ${CMAKE_CURRENT_LIST_DIR}/processdata.cpp
    ${CMAKE_CURRENT_LIST_DIR}/processdata.h
    ${CMAKE_CURRENT_LIST_DIR}/toolswidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/toolswidget.h
    ${CMAKE_CURRENT_LIST_DIR}/toolswidget.ui
    ${CMAKE_CURRENT_LIST_DIR}/xmainwidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xmainwidget.h
    ${CMAKE_CURRENT_LIST_DIR}/xmainwidget.ui
    ${CMAKE_CURRENT_LIST_DIR}/ELF/elf_ehdr.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ELF/elf_ehdr.h
    ${CMAKE_CURRENT_LIST_DIR}/ELF/elf_ehdr.ui
    ${CMAKE_CURRENT_LIST_DIR}/MACH/mach_headerwidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/MACH/mach_headerwidget.h
    ${CMAKE_CURRENT_LIST_DIR}/MACH/mach_headerwidget.ui
)

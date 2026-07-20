include_directories(${CMAKE_CURRENT_LIST_DIR})

if (NOT DEFINED XFORMATS_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../../Formats/xformats.cmake)
    set(XFWIDGETS_SOURCES ${XFWIDGETS_SOURCES} ${XFORMATS_SOURCES})
endif()

if (NOT DEFINED XFCONTROLS_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../../Controls/XFControls/xfcontrols.cmake)
    set(XFWIDGETS_SOURCES ${XFWIDGETS_SOURCES} ${XFCONTROLS_SOURCES})
endif()

if (NOT DEFINED XMODEL_BINARY_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../../Controls/xmodel_binary.cmake)
    set(XFWIDGETS_SOURCES ${XFWIDGETS_SOURCES} ${XMODEL_BINARY_SOURCES})
endif()

if (NOT DEFINED XSHORTCUTS_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../../XShortcuts/xshortcuts.cmake)
    set(XFWIDGETS_SOURCES ${XFWIDGETS_SOURCES} ${XSHORTCUTS_SOURCES})
endif()

if (NOT DEFINED XDIALOGPROCESS_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../../FormatDialogs/xdialogprocess.cmake)
    set(XFWIDGETS_SOURCES ${XFWIDGETS_SOURCES} ${XDIALOGPROCESS_SOURCES})
endif()

if (NOT DEFINED XREGIONSWIDGET_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../../XRegionsWidget/xregionswidget.cmake)
    set(XFWIDGETS_SOURCES ${XFWIDGETS_SOURCES} ${XREGIONSWIDGET_SOURCES})
endif()

if (NOT DEFINED XVISUALIZATIONWIDGET_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../../XVisualizationWidget/xvisualizationwidget.cmake)
    set(XFWIDGETS_SOURCES ${XFWIDGETS_SOURCES} ${XVISUALIZATIONWIDGET_SOURCES})
endif()

if (NOT DEFINED XHEXVIEW_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../../XHexView/xhexview.cmake)
    set(XFWIDGETS_SOURCES ${XFWIDGETS_SOURCES} ${XHEXVIEW_SOURCES})
endif()

if (NOT DEFINED XDISASMVIEW_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../../XDisasmView/xdisasmview.cmake)
    set(XFWIDGETS_SOURCES ${XFWIDGETS_SOURCES} ${XDISASMVIEW_SOURCES})
endif()

if (NOT DEFINED NFD_WIDGET_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../../nfd_widget/nfd_widget.cmake)
    set(XFWIDGETS_SOURCES ${XFWIDGETS_SOURCES} ${NFD_WIDGET_SOURCES})
endif()

if (NOT DEFINED XHASHWIDGET_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../../XHashWidget/xhashwidget.cmake)
    set(XFWIDGETS_SOURCES ${XFWIDGETS_SOURCES} ${XHASHWIDGET_SOURCES})
endif()

if (NOT DEFINED SEARCHSIGNATURESWIDGET_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../SearchSignatures/searchsignatureswidget.cmake)
    set(XFWIDGETS_SOURCES ${XFWIDGETS_SOURCES} ${SEARCHSIGNATURESWIDGET_SOURCES})
endif()

if (NOT DEFINED XMEMORYMAPWIDGET_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../../XMemoryMapWidget/xmemorymapwidget.cmake)
    set(XFWIDGETS_SOURCES ${XFWIDGETS_SOURCES} ${XMEMORYMAPWIDGET_SOURCES})
endif()

if (NOT DEFINED XENTROPYWIDGET_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../../XEntropyWidget/xentropywidget.cmake)
    set(XFWIDGETS_SOURCES ${XFWIDGETS_SOURCES} ${XENTROPYWIDGET_SOURCES})
endif()

if (NOT DEFINED XEXTRACTORWIDGET_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../../XExtractorWidget/xextractorwidget.cmake)
    set(XFWIDGETS_SOURCES ${XFWIDGETS_SOURCES} ${XEXTRACTORWIDGET_SOURCES})
endif()

if (NOT DEFINED SEARCHVALUESWIDGET_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../SearchValues/searchvalueswidget.cmake)
    set(XFWIDGETS_SOURCES ${XFWIDGETS_SOURCES} ${SEARCHVALUESWIDGET_SOURCES})
endif()

set(XFWIDGETS_SOURCES
    ${XFWIDGETS_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/xfwidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xfwidget.h
    ${CMAKE_CURRENT_LIST_DIR}/xfwidget.ui
    ${CMAKE_CURRENT_LIST_DIR}/xfwidgetadvanced.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xfwidgetadvanced.h
    ${CMAKE_CURRENT_LIST_DIR}/xfwidgetadvanced.ui
    ${CMAKE_CURRENT_LIST_DIR}/xfwidget_header.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xfwidget_header.h
    ${CMAKE_CURRENT_LIST_DIR}/xfwidget_header.ui
    ${CMAKE_CURRENT_LIST_DIR}/xfwidget_table.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xfwidget_table.h
    ${CMAKE_CURRENT_LIST_DIR}/xfwidget_table.ui
    ${CMAKE_CURRENT_LIST_DIR}/xfwidget_strings.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xfwidget_strings.h
    ${CMAKE_CURRENT_LIST_DIR}/xfwidget_strings.ui
    ${CMAKE_CURRENT_LIST_DIR}/xfwidget_regions.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xfwidget_regions.h
    ${CMAKE_CURRENT_LIST_DIR}/xfwidget_regions.ui
    ${CMAKE_CURRENT_LIST_DIR}/xfwidget_visualization.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xfwidget_visualization.h
    ${CMAKE_CURRENT_LIST_DIR}/xfwidget_hex.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xfwidget_hex.h
    ${CMAKE_CURRENT_LIST_DIR}/xfwidget_disasm.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xfwidget_disasm.h
    ${CMAKE_CURRENT_LIST_DIR}/xfwidget_nfdscan.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xfwidget_nfdscan.h
    ${CMAKE_CURRENT_LIST_DIR}/xfwidget_hash.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xfwidget_hash.h
    ${CMAKE_CURRENT_LIST_DIR}/xfwidget_signatures.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xfwidget_signatures.h
    ${CMAKE_CURRENT_LIST_DIR}/xfwidget_memorymap.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xfwidget_memorymap.h
    ${CMAKE_CURRENT_LIST_DIR}/xfwidget_entropy.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xfwidget_entropy.h
    ${CMAKE_CURRENT_LIST_DIR}/xfwidget_extractor.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xfwidget_extractor.h
    ${CMAKE_CURRENT_LIST_DIR}/xfwidget_search.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xfwidget_search.h
)

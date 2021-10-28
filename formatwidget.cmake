include_directories(${CMAKE_CURRENT_LIST_DIR})

include(${CMAKE_CURRENT_LIST_DIR}/SearchSignatures/searchsignatureswidget.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/SearchStrings/searchstringswidget.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../Controls/xlineedithex.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../Controls/xcomboboxex.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../Controls/xdatetimeeditx.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../XHexView/xhexview.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../XMemoryMapWidget/xmemorymapwidget.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../XEntropyWidget/xentropywidget.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../StaticScan/heurwidget.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../XHashWidget/xhashwidget.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../XDisasmView/xdisasmview.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../XDemangleWidget/xdemanglewidget.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../Controls/xdatetimeeditx.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../Formats/xformats.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../XDEX/xdex.cmake)

set(FORMATWIDGET_SOURCES
    ${SEARCHSIGNATURESWIDGET_SOURCES}
    ${SEARCHSTRINGSWIDGET_SOURCES}
    ${XLINEEDITHEX_SOURCES}
    ${XCOMBOBOXEX_SOURCES}
    ${XDATETIMEEDITX_SOURCES}
    ${XHEXVIEW_SOURCES}
    ${XMEMORYMAPWIDGET_SOURCES}
    ${XENTROPYWIDGET_SOURCES}
    ${HEURWIDGET_SOURCES}
    ${XHASHWIDGET_SOURCES}
    ${XDISASMVIEW_SOURCES}
    ${XDEMANGLEWIDGET_SOURCES}
    ${XDATETIMEEDITX_SOURCES}
    ${XFORMATS_SOURCES}
    ${XDEX_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/dialogmodelinfo.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogmodelinfo.ui
    ${CMAKE_CURRENT_LIST_DIR}/dialogprocessdata.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogprocessdata.ui
    ${CMAKE_CURRENT_LIST_DIR}/dialogsectionheader.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogsectionheader.ui
    ${CMAKE_CURRENT_LIST_DIR}/formatwidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/invwidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/processdata.cpp
    ${CMAKE_CURRENT_LIST_DIR}/toolswidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/toolswidget.ui
    )

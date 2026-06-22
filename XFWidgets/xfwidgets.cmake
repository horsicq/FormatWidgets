include_directories(${CMAKE_CURRENT_LIST_DIR})

if (NOT DEFINED XFORMATS_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../../Formats/xformats.cmake)
    set(XFWIDGETS_SOURCES ${XFWIDGETS_SOURCES} ${XFORMATS_SOURCES})
endif()

if (NOT DEFINED XFCONTROLS_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../../Controls/XFControls/xfcontrols.cmake)
    set(XFWIDGETS_SOURCES ${XFWIDGETS_SOURCES} ${XFCONTROLS_SOURCES})
endif()

if (NOT DEFINED XSHORTCUTS_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../../XShortcuts/xshortcuts.cmake)
    set(XFWIDGETS_SOURCES ${XFWIDGETS_SOURCES} ${XSHORTCUTS_SOURCES})
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
)

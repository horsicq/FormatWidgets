include_directories(${CMAKE_CURRENT_LIST_DIR})

if (NOT DEFINED XFCONTROLS_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../../Controls/XFControls/xfcontrols.cmake)
    set(XFWIDGETS_SOURCES ${XFWIDGETS_SOURCES} ${XFCONTROLS_SOURCES})
endif()

set(XFWIDGETS_SOURCES
    ${XFWIDGETS_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/xfwidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xfwidget.h
)

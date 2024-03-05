include_directories(${CMAKE_CURRENT_LIST_DIR})

include(${CMAKE_CURRENT_LIST_DIR}/../formatwidget.cmake)

set(LEWIDGET_SOURCES
    ${FORMATWIDGET_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/lesectionheaderwidget.ui
    ${CMAKE_CURRENT_LIST_DIR}/lewidget.ui
    ${CMAKE_CURRENT_LIST_DIR}/dialogle.ui
    ${CMAKE_CURRENT_LIST_DIR}/dialogle.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogle.h
    ${CMAKE_CURRENT_LIST_DIR}/le_defs.cpp
    ${CMAKE_CURRENT_LIST_DIR}/le_defs.h
    ${CMAKE_CURRENT_LIST_DIR}/leprocessdata.cpp
    ${CMAKE_CURRENT_LIST_DIR}/leprocessdata.h
    ${CMAKE_CURRENT_LIST_DIR}/lesectionheaderwidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/lesectionheaderwidget.h
    ${CMAKE_CURRENT_LIST_DIR}/lewidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/lewidget.h
    )

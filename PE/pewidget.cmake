include_directories(${CMAKE_CURRENT_LIST_DIR})

include(${CMAKE_CURRENT_LIST_DIR}/../formatwidget.cmake)

set(PEWIDGET_SOURCES
    ${FORMATWIDGET_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/dialogpe.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogpe.h
    ${CMAKE_CURRENT_LIST_DIR}/dialogpe.ui
    ${CMAKE_CURRENT_LIST_DIR}/pe_defs.cpp
    ${CMAKE_CURRENT_LIST_DIR}/pe_defs.h
    ${CMAKE_CURRENT_LIST_DIR}/peprocessdata.cpp
    ${CMAKE_CURRENT_LIST_DIR}/peprocessdata.h
    ${CMAKE_CURRENT_LIST_DIR}/pesectionheaderwidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/pesectionheaderwidget.h
    ${CMAKE_CURRENT_LIST_DIR}/pesectionheaderwidget.ui
    ${CMAKE_CURRENT_LIST_DIR}/petoolswidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/petoolswidget.h
    ${CMAKE_CURRENT_LIST_DIR}/petoolswidget.ui
    ${CMAKE_CURRENT_LIST_DIR}/pewidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/pewidget.h
    ${CMAKE_CURRENT_LIST_DIR}/pewidget.ui
    )

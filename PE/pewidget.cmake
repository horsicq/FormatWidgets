include_directories(${CMAKE_CURRENT_LIST_DIR})

include(${CMAKE_CURRENT_LIST_DIR}/../formatwidget.cmake)

set(PEWIDGET_SOURCES
    ${FORMATWIDGET_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/pesectionheaderwidget.ui
    ${CMAKE_CURRENT_LIST_DIR}/pewidget.ui
    ${CMAKE_CURRENT_LIST_DIR}/dialogpe.ui
    ${CMAKE_CURRENT_LIST_DIR}/pesectionheaderwidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/pewidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/pe_defs.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogpe.cpp
    ${CMAKE_CURRENT_LIST_DIR}/peprocessdata.cpp
    )

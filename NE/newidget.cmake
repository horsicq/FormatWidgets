include_directories(${CMAKE_CURRENT_LIST_DIR})

include(${CMAKE_CURRENT_LIST_DIR}/../formatwidget.cmake)

set(NEWIDGET_SOURCES
    ${FORMATWIDGET_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/dialogne.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogne.h
    ${CMAKE_CURRENT_LIST_DIR}/dialogne.ui
    ${CMAKE_CURRENT_LIST_DIR}/ne_defs.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ne_defs.h
    ${CMAKE_CURRENT_LIST_DIR}/neprocessdata.cpp
    ${CMAKE_CURRENT_LIST_DIR}/neprocessdata.h
    ${CMAKE_CURRENT_LIST_DIR}/nesectionheaderwidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/nesectionheaderwidget.h
    ${CMAKE_CURRENT_LIST_DIR}/nesectionheaderwidget.ui
    ${CMAKE_CURRENT_LIST_DIR}/newidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/newidget.h
    ${CMAKE_CURRENT_LIST_DIR}/newidget.ui
    )

include_directories(${CMAKE_CURRENT_LIST_DIR})

include(${CMAKE_CURRENT_LIST_DIR}/../formatwidget.cmake)

set(MACHWIDGET_SOURCES
    ${FORMATWIDGET_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/dialogmach.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogmach.ui
    ${CMAKE_CURRENT_LIST_DIR}/mach_defs.cpp
    ${CMAKE_CURRENT_LIST_DIR}/machprocessdata.cpp
    ${CMAKE_CURRENT_LIST_DIR}/machsectionheaderwidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/machsectionheaderwidget.ui
    ${CMAKE_CURRENT_LIST_DIR}/machwidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/machwidget.ui
    )

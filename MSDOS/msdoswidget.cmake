include_directories(${CMAKE_CURRENT_LIST_DIR})

include(${CMAKE_CURRENT_LIST_DIR}/../formatwidget.cmake)

set(MSDOSWIDGET_SOURCES
    ${FORMAT_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/msdoswidget.ui
    ${CMAKE_CURRENT_LIST_DIR}/dialogmsdos.ui
    ${CMAKE_CURRENT_LIST_DIR}/msdoswidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/msdos_defs.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogmsdos.cpp
    )

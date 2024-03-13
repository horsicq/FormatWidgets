include_directories(${CMAKE_CURRENT_LIST_DIR})

include(${CMAKE_CURRENT_LIST_DIR}/../formatwidget.cmake)

set(MSDOSWIDGET_SOURCES
    ${FORMATWIDGET_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/dialogmsdos.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogmsdos.h
    ${CMAKE_CURRENT_LIST_DIR}/dialogmsdos.ui
    ${CMAKE_CURRENT_LIST_DIR}/msdos_defs.cpp
    ${CMAKE_CURRENT_LIST_DIR}/msdos_defs.h
    ${CMAKE_CURRENT_LIST_DIR}/msdoswidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/msdoswidget.h
    ${CMAKE_CURRENT_LIST_DIR}/msdoswidget.ui
    )

include_directories(${CMAKE_CURRENT_LIST_DIR})

include(${CMAKE_CURRENT_LIST_DIR}/../formatwidget.cmake)

set(MACHOFATWIDGET_SOURCES
    ${FORMATWIDGET_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/dialogmachofat.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogmachofat.ui
    ${CMAKE_CURRENT_LIST_DIR}/machofatwidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/machofatwidget.ui
    )

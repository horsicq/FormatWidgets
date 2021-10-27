include_directories(${CMAKE_CURRENT_LIST_DIR})

include(${CMAKE_CURRENT_LIST_DIR}/../formatwidget.cmake)

set(MACHOFATWIDGET_SOURCES
    ${FORMAT_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/MACHOFAT/machofatwidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/MACHOFAT/machofatwidget.ui
    )

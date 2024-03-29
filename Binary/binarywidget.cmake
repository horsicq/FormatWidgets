include_directories(${CMAKE_CURRENT_LIST_DIR})

include(${CMAKE_CURRENT_LIST_DIR}/../formatwidget.cmake)

set(BINARYWIDGET_SOURCES
    ${FORMATWIDGET_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/binary_defs.cpp
    ${CMAKE_CURRENT_LIST_DIR}/binary_defs.h
    ${CMAKE_CURRENT_LIST_DIR}/binarywidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/binarywidget.h
    ${CMAKE_CURRENT_LIST_DIR}/binarywidget.ui
    ${CMAKE_CURRENT_LIST_DIR}/dialogbinary.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogbinary.h
    ${CMAKE_CURRENT_LIST_DIR}/dialogbinary.ui
    )

include_directories(${CMAKE_CURRENT_LIST_DIR})

if (NOT DEFINED XCOMBOBOXEX_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../../Controls/xcomboboxex.cmake)
    set(XFILEEXPLORER_SOURCES ${XFILEEXPLORER_SOURCES} ${XCOMBOBOXEX_SOURCES})
endif()

set(XFILEEXPLORER_SOURCES
    ${XFILEEXPLORER_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/xfilesystemmodel.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xfilesystemmodel.h
    ${CMAKE_CURRENT_LIST_DIR}/xfileexplorer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xfileexplorer.h
    ${CMAKE_CURRENT_LIST_DIR}/xfileexplorer.ui
)

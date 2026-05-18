include_directories(${CMAKE_CURRENT_LIST_DIR})

if (NOT DEFINED XCOMBOBOXEX_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../../Controls/xcomboboxex.cmake)
    set(XFILEEXPLORER_SOURCES ${XFILEEXPLORER_SOURCES} ${XCOMBOBOXEX_SOURCES})
endif()
if (NOT DEFINED XFILEINFO_SOURCES)
    add_definitions(-DUSE_DEX)
    add_definitions(-DUSE_PDF)
    add_definitions(-DUSE_ARCHIVE)
    include(${CMAKE_CURRENT_LIST_DIR}/../../XFileInfo/xfileinfo.cmake)
    set(XFILEEXPLORER_SOURCES ${XFILEEXPLORER_SOURCES} ${XFILEINFO_SOURCES})
endif()
if (NOT DEFINED DIALOGWIDGET_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../../FormatDialogs/dialogwidget.cmake)
    set(XFILEEXPLORER_SOURCES ${XFILEEXPLORER_SOURCES} ${DIALOGWIDGET_SOURCES})
endif()
if (NOT DEFINED XDIALOGPROCESS_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../../FormatDialogs/xdialogprocess.cmake)
    set(XFILEEXPLORER_SOURCES ${XFILEEXPLORER_SOURCES} ${XDIALOGPROCESS_SOURCES})
endif()

set(XFILEEXPLORER_SOURCES
    ${XFILEEXPLORER_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/xfilesystemmodel.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xfilesystemmodel.h
    ${CMAKE_CURRENT_LIST_DIR}/xfileexplorer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xfileexplorer.h
    ${CMAKE_CURRENT_LIST_DIR}/xfileexplorer.ui
)

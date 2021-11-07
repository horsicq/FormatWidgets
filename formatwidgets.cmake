include_directories(${CMAKE_CURRENT_LIST_DIR})

include(${CMAKE_CURRENT_LIST_DIR}/Binary/binarywidget.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/DEX/dexwidget.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/ELF/elfwidget.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/LE/lewidget.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/MACH/machwidget.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/MACHOFAT/machofatwidget.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/MSDOS/msdoswidget.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/NE/newidget.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/PE/pewidget.cmake)

set(FORMATWIDGETS_SOURCES
    ${BINARYWIDGET_SOURCES}
    ${DEXWIDGET_SOURCES}
    ${ELFWIDGET_SOURCES}
    ${LEWIDGET_SOURCES}
    ${MACHWIDGET_SOURCES}
    ${MACHOFATWIDGET_SOURCES}
    ${MSDOSWIDGET_SOURCES}
    ${NEWIDGET_SOURCES}
    ${PEWIDGET_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/formatswidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/formatswidget.ui
    )


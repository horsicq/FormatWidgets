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
include(${CMAKE_CURRENT_LIST_DIR}/../XDEX/xdex.cmake)

set(ALLFORMATWIDGETS_SOURCES
    ${BINARYWIDGET_SOURCE}
    ${DEXWIDGET_SOURCE}
    ${ELFWIDGET_SOURCE}
    ${LEWIDGET_SOURCE}
    ${MACHWIDGET_SOURCE}
    ${MACHOFATWIDGET_SOURCE}
    ${MSDOSWIDGET_SOURCE}
    ${NEWIDGET_SOURCE}
    ${PEWIDGET_SOURCE}
    ${XDEX_SOURCE}
)

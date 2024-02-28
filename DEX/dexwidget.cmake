include_directories(${CMAKE_CURRENT_LIST_DIR})

include(${CMAKE_CURRENT_LIST_DIR}/../formatwidget.cmake)

set(DEXWIDGET_SOURCES
    ${FORMATWIDGET_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/dex_defs.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dex_defs.h
    ${CMAKE_CURRENT_LIST_DIR}/dexprocessdata.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dexprocessdata.h
    ${CMAKE_CURRENT_LIST_DIR}/dexsectionheaderwidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dexsectionheaderwidget.h
    ${CMAKE_CURRENT_LIST_DIR}/dexsectionheaderwidget.ui
    ${CMAKE_CURRENT_LIST_DIR}/dexwidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dexwidget.h
    ${CMAKE_CURRENT_LIST_DIR}/dexwidget.ui
    ${CMAKE_CURRENT_LIST_DIR}/dialogdex.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogdex.h
    ${CMAKE_CURRENT_LIST_DIR}/dialogdex.ui
    )

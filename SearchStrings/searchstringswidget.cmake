include_directories(${CMAKE_CURRENT_LIST_DIR})

include(${CMAKE_CURRENT_LIST_DIR}/../MultiSearch/multisearch.cmake)

set(SEARCHSTRINGSWIDGET_SOURCES
    ${MULTISEARCH_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/dialogsearchstrings.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogsearchstrings.ui
    ${CMAKE_CURRENT_LIST_DIR}/searchstringswidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/searchstringswidget.ui
    )

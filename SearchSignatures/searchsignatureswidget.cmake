include_directories(${CMAKE_CURRENT_LIST_DIR})

include(${CMAKE_CURRENT_LIST_DIR}/../MultiSearch/multisearch.cmake)

set(SEARCHSIGNATURESWIDGET_SOURCES
    ${MULTISEARCH_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/dialogsearchsignatures.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogsearchsignatures.ui
    ${CMAKE_CURRENT_LIST_DIR}/searchsignatureswidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/searchsignatureswidget.ui
    )

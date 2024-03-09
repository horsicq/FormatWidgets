include_directories(${CMAKE_CURRENT_LIST_DIR})

include(${CMAKE_CURRENT_LIST_DIR}/../MultiSearch/multisearch.cmake)

set(SEARCHVALUESWIDGET_SOURCES
    ${MULTISEARCH_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/dialogsearchvalues.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogsearchvalues.h
    ${CMAKE_CURRENT_LIST_DIR}/dialogsearchvalues.ui
    ${CMAKE_CURRENT_LIST_DIR}/searchvalueswidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/searchvalueswidget.h
    ${CMAKE_CURRENT_LIST_DIR}/searchvalueswidget.ui
    )

include_directories(${CMAKE_CURRENT_LIST_DIR})

if (NOT DEFINED MULTISEARCH_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../MultiSearch/multisearch.cmake)
    set(SEARCHVALUESWIDGET_SOURCES ${SEARCHVALUESWIDGET_SOURCES} ${MULTISEARCH_SOURCES})
endif()

set(SEARCHVALUESWIDGET_SOURCES
    ${SEARCHVALUESWIDGET_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/dialogsearchvalues.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogsearchvalues.h
    ${CMAKE_CURRENT_LIST_DIR}/dialogsearchvalues.ui
    ${CMAKE_CURRENT_LIST_DIR}/searchvalueswidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/searchvalueswidget.h
    ${CMAKE_CURRENT_LIST_DIR}/searchvalueswidget.ui
    )

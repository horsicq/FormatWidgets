include_directories(${CMAKE_CURRENT_LIST_DIR})

if (NOT DEFINED MULTISEARCH_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../MultiSearch/multisearch.cmake)
    set(SEARCHVALUESWIDGET_SOURCES ${SEARCHVALUESWIDGET_SOURCES} ${MULTISEARCH_SOURCES})
endif()
if (NOT DEFINED DIALOGEDITSTRING_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../../FormatDialogs/dialogeditstring.cmake)
    set(SEARCHVALUESWIDGET_SOURCES ${SEARCHVALUESWIDGET_SOURCES} ${DIALOGEDITSTRING_SOURCES})
endif()

include(${CMAKE_CURRENT_LIST_DIR}/../../Controls/xtableview.cmake)

set(SEARCHSTRINGSWIDGET_SOURCES
    ${SEARCHVALUESWIDGET_SOURCES}
    ${MULTISEARCH_SOURCES}
    ${DIALOGEDITSTRING_SOURCES}
    ${XTABLEVIEW_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/dialogsearchstrings.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogsearchstrings.h
    ${CMAKE_CURRENT_LIST_DIR}/dialogsearchstrings.ui
    ${CMAKE_CURRENT_LIST_DIR}/searchstringswidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/searchstringswidget.h
    ${CMAKE_CURRENT_LIST_DIR}/searchstringswidget.ui
    )

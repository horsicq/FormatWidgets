include_directories(${CMAKE_CURRENT_LIST_DIR})

if (NOT DEFINED XMODEL_BINARY_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../../Controls/xmodel_binary.cmake)
    set(XSTRUCTWIDGET_SOURCES ${XSTRUCTWIDGET_SOURCES} ${XMODEL_BINARY_SOURCES})
endif()

set(XSTRUCTWIDGET_SOURCES
    ${XSTRUCTWIDGET_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/xgenericabstractwidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xgenericabstractwidget.h
    ${CMAKE_CURRENT_LIST_DIR}/xgenericheaderwidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xgenericheaderwidget.h
    ${CMAKE_CURRENT_LIST_DIR}/xgenericheaderwidget.ui
    ${CMAKE_CURRENT_LIST_DIR}/xgenerictablewidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xgenerictablewidget.h
    ${CMAKE_CURRENT_LIST_DIR}/xgenerictablewidget.ui
    ${CMAKE_CURRENT_LIST_DIR}/xstructwidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xstructwidget.h
    ${CMAKE_CURRENT_LIST_DIR}/xstructwidget.ui
    ${CMAKE_CURRENT_LIST_DIR}/dialogxstruct.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogxstruct.h
    ${CMAKE_CURRENT_LIST_DIR}/dialogxstruct.ui
    ${CMAKE_CURRENT_LIST_DIR}/xgetdatarecordsprocess.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xgetdatarecordsprocess.h
)

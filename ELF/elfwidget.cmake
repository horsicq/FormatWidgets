include_directories(${CMAKE_CURRENT_LIST_DIR})

include(${CMAKE_CURRENT_LIST_DIR}/../formatwidget.cmake)

set(ELFWIDGET_SOURCES
    ${FORMATWIDGET_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/dialogelf.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogelf.h
    ${CMAKE_CURRENT_LIST_DIR}/dialogelf.ui
    ${CMAKE_CURRENT_LIST_DIR}/elf_defs.cpp
    ${CMAKE_CURRENT_LIST_DIR}/elf_defs.h
    ${CMAKE_CURRENT_LIST_DIR}/elfprocessdata.cpp
    ${CMAKE_CURRENT_LIST_DIR}/elfprocessdata.h
    ${CMAKE_CURRENT_LIST_DIR}/elfsectionheaderwidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/elfsectionheaderwidget.h
    ${CMAKE_CURRENT_LIST_DIR}/elfsectionheaderwidget.ui
    ${CMAKE_CURRENT_LIST_DIR}/elfwidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/elfwidget.h
    ${CMAKE_CURRENT_LIST_DIR}/elfwidget.ui
    )

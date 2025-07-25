include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${CMAKE_CURRENT_LIST_DIR}/Generic)

set(ABSTRACTWIDGETS_SOURCES
    ${ABSTRACTWIDGETS_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/Structs/xdex_defs.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Structs/xdex_defs.h
    ${CMAKE_CURRENT_LIST_DIR}/Structs/xelf_defs.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Structs/xelf_defs.h
    ${CMAKE_CURRENT_LIST_DIR}/Structs/xle_defs.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Structs/xle_defs.h
    ${CMAKE_CURRENT_LIST_DIR}/Structs/xmach_defs.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Structs/xmach_defs.h
    ${CMAKE_CURRENT_LIST_DIR}/Structs/xmsdos_defs.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Structs/xmsdos_defs.h
    ${CMAKE_CURRENT_LIST_DIR}/Structs/xne_defs.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Structs/xne_defs.h
    ${CMAKE_CURRENT_LIST_DIR}/Structs/xpe_defs.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Structs/xpe_defs.h
    ${CMAKE_CURRENT_LIST_DIR}/Structs/xarchives_defs.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Structs/xarchives_defs.h
    ${CMAKE_CURRENT_LIST_DIR}/Structs/xformats_defs.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Structs/xformats_defs.h
    ${CMAKE_CURRENT_LIST_DIR}/dialogsetgenericwidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogsetgenericwidget.h
    ${CMAKE_CURRENT_LIST_DIR}/dialogsetgenericwidget.ui
    ${CMAKE_CURRENT_LIST_DIR}/xdialogprocessdata.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xdialogprocessdata.h
    ${CMAKE_CURRENT_LIST_DIR}/xformatwidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xformatwidget.h
    ${CMAKE_CURRENT_LIST_DIR}/xformatwidget_def.h
    ${CMAKE_CURRENT_LIST_DIR}/xprocessdata.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xprocessdata.h
)

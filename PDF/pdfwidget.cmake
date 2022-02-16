include_directories(${CMAKE_CURRENT_LIST_DIR})

include(${CMAKE_CURRENT_LIST_DIR}/../formatwidget.cmake)

set(PDFWIDGET_SOURCES
    ${FORMATWIDGET_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/pdfwidget.ui
    ${CMAKE_CURRENT_LIST_DIR}/dialogpdf.ui
    ${CMAKE_CURRENT_LIST_DIR}/pdfwidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/pdf_defs.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogpdf.cpp
    )

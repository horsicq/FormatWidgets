INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/Structs/generic_defs.h \
    $$PWD/Structs/xdex_defs.h \
    $$PWD/Structs/xelf_defs.h \
    $$PWD/Structs/xle_defs.h \
    $$PWD/Structs/xmach_defs.h \
    $$PWD/Structs/xmsdos_defs.h \
    $$PWD/Structs/xne_defs.h \
    $$PWD/Structs/xpe_defs.h \
    $$PWD/Structs/xarchives_defs.h \
    $$PWD/Structs/xformats_defs.h \
    $$PWD/dialogsetgenericwidget.h \
    $$PWD/xdialogprocessdata.h \
    $$PWD/xformatwidget.h \
    $$PWD/xformatwidget_def.h \
    $$PWD/xgenericheaderwidget.h \
    $$PWD/xgenerichexwidget.h \
    $$PWD/xgenericdisasmwidget.h \
    $$PWD/xgenerictablehexwidget.h \
    $$PWD/xgenerictablewidget.h \
    $$PWD/xprocessdata.h

SOURCES += \
    $$PWD/Structs/generic_defs.cpp \
    $$PWD/Structs/xdex_defs.cpp \
    $$PWD/Structs/xelf_defs.cpp \
    $$PWD/Structs/xle_defs.cpp \
    $$PWD/Structs/xmach_defs.cpp \
    $$PWD/Structs/xmsdos_defs.cpp \
    $$PWD/Structs/xne_defs.cpp \
    $$PWD/Structs/xpe_defs.cpp \
    $$PWD/Structs/xarchives_defs.cpp \
    $$PWD/Structs/xformats_defs.cpp \
    $$PWD/dialogsetgenericwidget.cpp \
    $$PWD/xdialogprocessdata.cpp \
    $$PWD/xformatwidget.cpp \
    $$PWD/xgenericheaderwidget.cpp \
    $$PWD/xgenerichexwidget.cpp \
    $$PWD/xgenericdisasmwidget.cpp \
    $$PWD/xgenerictablehexwidget.cpp \
    $$PWD/xgenerictablewidget.cpp \
    $$PWD/xprocessdata.cpp
    
FORMS += \
    $$PWD/dialogsetgenericwidget.ui \
    $$PWD/dialogxmainwidget.ui \
    $$PWD/xgenericheaderwidget.ui \
    $$PWD/xgenerichexwidget.ui \
    $$PWD/xgenericdisasmwidget.ui \
    $$PWD/xgenerictablehexwidget.ui \
    $$PWD/xgenerictablewidget.ui \
    $$PWD/xmainwidget.ui

DISTFILES += \
    $$PWD/abstractwidgets.cmake
    

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/Specific/xdex_defs.h \
    $$PWD/Specific/xelf_defs.h \
    $$PWD/Specific/xle_defs.h \
    $$PWD/Specific/xmach_defs.h \
    $$PWD/Specific/xmsdos_defs.h \
    $$PWD/Specific/xne_defs.h \
    $$PWD/Specific/xpdf_defs.h \
    $$PWD/Specific/xpe_defs.h \
    $$PWD/dialogxmainwidget.h \
    $$PWD/dialogsetgenericwidget.h \
    $$PWD/xdialogprocessdata.h \
    $$PWD/xformatwidget.h \
    $$PWD/xformatwidget_def.h \
    $$PWD/xgenericheaderwidget.h \
    $$PWD/xgenerictablewidget.h \
    $$PWD/xmainwidget.h \
    $$PWD/xmainwidgetadvanced.h \
    $$PWD/xprocessdata.h

SOURCES += \
    $$PWD/Specific/xdex_defs.cpp \
    $$PWD/Specific/xelf_defs.cpp \
    $$PWD/Specific/xle_defs.cpp \
    $$PWD/Specific/xmach_defs.cpp \
    $$PWD/Specific/xmsdos_defs.cpp \
    $$PWD/Specific/xne_defs.cpp \
    $$PWD/Specific/xpdf_defs.cpp \
    $$PWD/Specific/xpe_defs.cpp \
    $$PWD/dialogxmainwidget.cpp \
    $$PWD/dialogsetgenericwidget.cpp \
    $$PWD/xdialogprocessdata.cpp \
    $$PWD/xformatwidget.cpp \
    $$PWD/xgenericheaderwidget.cpp \
    $$PWD/xgenerictablewidget.cpp \
    $$PWD/xmainwidget.cpp \
    $$PWD/xmainwidgetadvanced.cpp \
    $$PWD/xprocessdata.cpp
    
FORMS += \
    $$PWD/dialogsetgenericwidget.ui \
    $$PWD/dialogxmainwidget.ui \
    $$PWD/xgenericheaderwidget.ui \
    $$PWD/xgenerictablewidget.ui \
    $$PWD/xmainwidget.ui

DISTFILES += \
    $$PWD/xformatwidget.cmake
    
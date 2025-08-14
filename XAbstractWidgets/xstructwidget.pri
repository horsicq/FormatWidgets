INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/xgenericabstractwidget.h \
    $$PWD/xgenericheaderwidget.h \
    $$PWD/xgenerictablewidget.h \
    $$PWD/xgetdatarecordsprocess.h \
    $$PWD/dialogxstruct.h \
    $$PWD/xstructwidget.h

SOURCES += \
    $$PWD/xgenericabstractwidget.cpp \
    $$PWD/xgenericheaderwidget.cpp \
    $$PWD/xgenerictablewidget.cpp \
    $$PWD/xgetdatarecordsprocess.cpp \
    $$PWD/dialogxstruct.cpp \
    $$PWD/xstructwidget.cpp

FORMS += \
    $$PWD/xgenericheaderwidget.ui \
    $$PWD/xgenerictablewidget.ui \
    $$PWD/dialogxstruct.ui \
    $$PWD/xstructwidget.ui

!contains(XCONFIG, xmodel_binary) {
    XCONFIG += xmodel_binary
    include($$PWD/../../Controls/xmodel_binary.pri)
}

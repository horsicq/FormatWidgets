#ifndef XFWIDGET_DEMANGLE_H
#define XFWIDGET_DEMANGLE_H

#include "xdemanglewidget.h"
#include "xformats.h"

class XFWidget_Demangle : public XShortcutsWidget {
    Q_OBJECT

public:
    explicit XFWidget_Demangle(QWidget *pParent = nullptr);
    ~XFWidget_Demangle() override;

    void setData(const XBinary::INDATA &inData);
    void clear();
    void setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions) override;
    void setReadonly(bool bIsReadonly) override;

private:
    XDemangleWidget *m_pDemangleWidget;
};

#endif

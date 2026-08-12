#ifndef XFWIDGET_DIESCAN_H
#define XFWIDGET_DIESCAN_H

#include "diewidgetadvanced.h"
#include "xformats.h"

class XFWidget_DIEScan : public XShortcutsWidget {
    Q_OBJECT

public:
    explicit XFWidget_DIEScan(QWidget *pParent = nullptr);
    ~XFWidget_DIEScan() override;

    void setData(const XBinary::INDATA &inData);
    void clear();
    void setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions) override;
    void setReadonly(bool bIsReadonly) override;

private:
    XBinary::INDATA m_inData;
    QIODevice *m_pDevice;
    DIEWidgetAdvanced *m_pDIEWidget;
};

#endif

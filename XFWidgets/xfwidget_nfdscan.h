#ifndef XFWIDGET_NFDSCAN_H
#define XFWIDGET_NFDSCAN_H

#include "nfdwidgetadvanced.h"
#include "xformats.h"

class XFWidget_NFDScan : public XShortcutsWidget {
    Q_OBJECT

public:
    explicit XFWidget_NFDScan(QWidget *pParent = nullptr);
    ~XFWidget_NFDScan() override;

    void setData(const XBinary::INDATA &inData);
    void clear();
    void reload();
    void setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions) override;
    void setReadonly(bool bIsReadonly) override;
    NFDWidgetAdvanced *getNFDWidget();

private:
    XBinary::INDATA m_inData;
    NFDWidgetAdvanced *m_pNFDWidget;
};

#endif

#ifndef XFWIDGET_YARASCAN_H
#define XFWIDGET_YARASCAN_H

#include "yarawidgetadvanced.h"
#include "xformats.h"

class XFWidget_YaraScan : public XShortcutsWidget {
    Q_OBJECT

public:
    explicit XFWidget_YaraScan(QWidget *pParent = nullptr);
    ~XFWidget_YaraScan() override;

    void setData(const XBinary::INDATA &inData);
    void clear();
    void setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions) override;
    void setReadonly(bool bIsReadonly) override;

private:
    XBinary::INDATA m_inData;
    YARAWidgetAdvanced *m_pYaraWidget;
};

#endif

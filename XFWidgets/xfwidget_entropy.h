#ifndef XFWIDGET_ENTROPY_H
#define XFWIDGET_ENTROPY_H

#include "xentropywidget.h"
#include "xformats.h"

class XFWidget_Entropy : public XShortcutsWidget {
    Q_OBJECT

public:
    explicit XFWidget_Entropy(QWidget *pParent = nullptr);
    ~XFWidget_Entropy() override;

    void setData(const XBinary::INDATA &inData);
    void clear();
    void reload();
    void setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions) override;
    void setReadonly(bool bIsReadonly) override;
    XEntropyWidget *getEntropyWidget();

private:
    XBinary::INDATA m_inData;
    XEntropyWidget *m_pEntropyWidget;
};

#endif

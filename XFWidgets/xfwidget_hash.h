#ifndef XFWIDGET_HASH_H
#define XFWIDGET_HASH_H

#include "xformats.h"
#include "xhashwidget.h"

class XFWidget_Hash : public XShortcutsWidget {
    Q_OBJECT

public:
    explicit XFWidget_Hash(QWidget *pParent = nullptr);
    ~XFWidget_Hash() override;

    void setData(const XBinary::INDATA &inData);
    void clear();
    void reload();
    void setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions) override;
    void setReadonly(bool bIsReadonly) override;
    XHashWidget *getHashWidget();

private:
    XBinary::INDATA m_inData;
    XHashWidget *m_pHashWidget;
};

#endif

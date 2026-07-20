#ifndef XFWIDGET_SIGNATURES_H
#define XFWIDGET_SIGNATURES_H

#include "searchsignatureswidget.h"
#include "xformats.h"

class XFWidget_Signatures : public XShortcutsWidget {
    Q_OBJECT

public:
    explicit XFWidget_Signatures(QWidget *pParent = nullptr);
    ~XFWidget_Signatures() override;

    void setData(const XBinary::INDATA &inData);
    void clear();
    void reload();
    void setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions) override;
    void setReadonly(bool bIsReadonly) override;
    SearchSignaturesWidget *getSignaturesWidget();

private:
    XBinary::INDATA m_inData;
    SearchSignaturesWidget *m_pSignaturesWidget;
};

#endif

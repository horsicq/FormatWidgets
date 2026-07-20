#ifndef XFWIDGET_SEARCH_H
#define XFWIDGET_SEARCH_H

#include "searchvalueswidget.h"
#include "xformats.h"

class XFWidget_Search : public XShortcutsWidget {
    Q_OBJECT

public:
    explicit XFWidget_Search(QWidget *pParent = nullptr);
    ~XFWidget_Search() override;

    void setData(const XBinary::INDATA &inData);
    void clear();
    void reload();
    void setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions) override;
    void setReadonly(bool bIsReadonly) override;
    SearchValuesWidget *getSearchWidget();

private:
    XBinary::INDATA m_inData;
    SearchValuesWidget *m_pSearchWidget;
};

#endif

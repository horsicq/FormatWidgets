#ifndef XFWIDGET_EXTRACTOR_H
#define XFWIDGET_EXTRACTOR_H

#include "xextractorwidget.h"
#include "xformats.h"

class XFWidget_Extractor : public XShortcutsWidget {
    Q_OBJECT

public:
    explicit XFWidget_Extractor(QWidget *pParent = nullptr);
    ~XFWidget_Extractor() override;

    void setData(const XBinary::INDATA &inData);
    void clear();
    void reload();
    void setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions) override;
    void setReadonly(bool bIsReadonly) override;
    XExtractorWidget *getExtractorWidget();

private:
    XBinary::INDATA m_inData;
    XInfoDB *m_pXInfoDB;
    XExtractorWidget *m_pExtractorWidget;
};

#endif

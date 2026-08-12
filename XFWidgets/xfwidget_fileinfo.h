#ifndef XFWIDGET_FILEINFO_H
#define XFWIDGET_FILEINFO_H

#include "xfileinfowidget.h"
#include "xformats.h"

class XFWidget_FileInfo : public XShortcutsWidget {
    Q_OBJECT

public:
    explicit XFWidget_FileInfo(QWidget *pParent = nullptr);
    ~XFWidget_FileInfo() override;

    void setData(const XBinary::INDATA &inData);
    void clear();
    void setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions) override;
    void setReadonly(bool bIsReadonly) override;

private:
    XBinary::INDATA m_inData;
    QIODevice *m_pDevice;
    XFileInfoWidget *m_pFileInfoWidget;
};

#endif

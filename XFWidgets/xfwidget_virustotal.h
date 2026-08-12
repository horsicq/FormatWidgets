#ifndef XFWIDGET_VIRUSTOTAL_H
#define XFWIDGET_VIRUSTOTAL_H

#include "xvirustotalwidget.h"
#include "xformats.h"

class XFWidget_VirusTotal : public XShortcutsWidget {
    Q_OBJECT

public:
    explicit XFWidget_VirusTotal(QWidget *pParent = nullptr);
    ~XFWidget_VirusTotal() override;

    void setData(const XBinary::INDATA &inData);
    void clear();
    void setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions) override;
    void setReadonly(bool bIsReadonly) override;

private:
    XBinary::INDATA m_inData;
    QIODevice *m_pDevice;
    XVirusTotalWidget *m_pVirusTotalWidget;
};

#endif

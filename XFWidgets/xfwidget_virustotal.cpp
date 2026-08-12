#include "xfwidget_virustotal.h"

#include <QVBoxLayout>

XFWidget_VirusTotal::XFWidget_VirusTotal(QWidget *pParent) : XShortcutsWidget(pParent)
{
    m_inData = {};
    m_pDevice = nullptr;
    m_pVirusTotalWidget = nullptr;
    QVBoxLayout *pLayout = new QVBoxLayout(this);
    pLayout->setContentsMargins(0, 0, 0, 0);
}

XFWidget_VirusTotal::~XFWidget_VirusTotal()
{
    clear();
}

void XFWidget_VirusTotal::setData(const XBinary::INDATA &inData)
{
    clear();
    m_inData = inData;
    m_pDevice = XFormats::createDevice(m_inData);

    if (m_pDevice) {
        m_pVirusTotalWidget = new XVirusTotalWidget(this);
        layout()->addWidget(m_pVirusTotalWidget);
        m_pVirusTotalWidget->setGlobal(getShortcuts(), getGlobalOptions());
        m_pVirusTotalWidget->setReadonly(isReadonly());
        m_pVirusTotalWidget->setData(m_pDevice);
    }
}

void XFWidget_VirusTotal::clear()
{
    delete m_pVirusTotalWidget;
    m_pVirusTotalWidget = nullptr;

    if (m_pDevice) {
        XFormats::removeDevice(m_pDevice, m_inData);
        m_pDevice = nullptr;
    }

    m_inData = {};
}

void XFWidget_VirusTotal::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    XShortcutsWidget::setGlobal(pShortcuts, pXOptions);
    if (m_pVirusTotalWidget) m_pVirusTotalWidget->setGlobal(pShortcuts, pXOptions);
}

void XFWidget_VirusTotal::setReadonly(bool bIsReadonly)
{
    XShortcutsWidget::setReadonly(bIsReadonly);
    if (m_pVirusTotalWidget) m_pVirusTotalWidget->setReadonly(bIsReadonly);
}

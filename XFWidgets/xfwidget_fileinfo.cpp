#include "xfwidget_fileinfo.h"

#include <QVBoxLayout>

XFWidget_FileInfo::XFWidget_FileInfo(QWidget *pParent) : XShortcutsWidget(pParent)
{
    m_inData = {};
    m_pDevice = nullptr;
    m_pFileInfoWidget = nullptr;
    QVBoxLayout *pLayout = new QVBoxLayout(this);
    pLayout->setContentsMargins(0, 0, 0, 0);
}

XFWidget_FileInfo::~XFWidget_FileInfo()
{
    clear();
}

void XFWidget_FileInfo::setData(const XBinary::INDATA &inData)
{
    clear();
    m_inData = inData;
    m_pDevice = XFormats::createDevice(m_inData);

    if (m_pDevice) {
        m_pFileInfoWidget = new XFileInfoWidget(this);
        layout()->addWidget(m_pFileInfoWidget);
        m_pFileInfoWidget->setGlobal(getShortcuts(), getGlobalOptions());
        m_pFileInfoWidget->setReadonly(isReadonly());
        m_pFileInfoWidget->setData(m_pDevice, m_inData.fileType, "Info", true);
    }
}

void XFWidget_FileInfo::clear()
{
    delete m_pFileInfoWidget;
    m_pFileInfoWidget = nullptr;

    if (m_pDevice) {
        XFormats::removeDevice(m_pDevice, m_inData);
        m_pDevice = nullptr;
    }

    m_inData = {};
}

void XFWidget_FileInfo::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    XShortcutsWidget::setGlobal(pShortcuts, pXOptions);
    if (m_pFileInfoWidget) m_pFileInfoWidget->setGlobal(pShortcuts, pXOptions);
}

void XFWidget_FileInfo::setReadonly(bool bIsReadonly)
{
    XShortcutsWidget::setReadonly(bIsReadonly);
    if (m_pFileInfoWidget) m_pFileInfoWidget->setReadonly(bIsReadonly);
}

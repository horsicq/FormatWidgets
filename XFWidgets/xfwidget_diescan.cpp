#include "xfwidget_diescan.h"

#include <QVBoxLayout>

XFWidget_DIEScan::XFWidget_DIEScan(QWidget *pParent) : XShortcutsWidget(pParent)
{
    m_inData = {};
    m_pDevice = nullptr;
    m_pDIEWidget = nullptr;
    QVBoxLayout *pLayout = new QVBoxLayout(this);
    pLayout->setContentsMargins(0, 0, 0, 0);
}

XFWidget_DIEScan::~XFWidget_DIEScan()
{
    clear();
}

void XFWidget_DIEScan::setData(const XBinary::INDATA &inData)
{
    clear();
    m_inData = inData;
    m_pDevice = XFormats::createDevice(m_inData);

    if (m_pDevice) {
        m_pDIEWidget = new DIEWidgetAdvanced(this);
        layout()->addWidget(m_pDIEWidget);
        m_pDIEWidget->setGlobal(getShortcuts(), getGlobalOptions());
        m_pDIEWidget->setReadonly(isReadonly());
        m_pDIEWidget->setData(m_pDevice, true, m_inData.fileType);
    }
}

void XFWidget_DIEScan::clear()
{
    delete m_pDIEWidget;
    m_pDIEWidget = nullptr;

    if (m_pDevice) {
        XFormats::removeDevice(m_pDevice, m_inData);
        m_pDevice = nullptr;
    }

    m_inData = {};
}

void XFWidget_DIEScan::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    XShortcutsWidget::setGlobal(pShortcuts, pXOptions);
    if (m_pDIEWidget) m_pDIEWidget->setGlobal(pShortcuts, pXOptions);
}

void XFWidget_DIEScan::setReadonly(bool bIsReadonly)
{
    XShortcutsWidget::setReadonly(bIsReadonly);
    if (m_pDIEWidget) m_pDIEWidget->setReadonly(bIsReadonly);
}

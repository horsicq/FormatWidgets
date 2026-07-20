#include "xfwidget_nfdscan.h"

#include <QVBoxLayout>

XFWidget_NFDScan::XFWidget_NFDScan(QWidget *pParent) : XShortcutsWidget(pParent)
{
    m_inData = {};
    m_pNFDWidget = nullptr;
    QVBoxLayout *pLayout = new QVBoxLayout(this);
    pLayout->setContentsMargins(0, 0, 0, 0);
}

XFWidget_NFDScan::~XFWidget_NFDScan()
{
    clear();
}

void XFWidget_NFDScan::setData(const XBinary::INDATA &inData)
{
    clear();
    m_inData = inData;
    m_pNFDWidget = new NFDWidgetAdvanced(this);
    layout()->addWidget(m_pNFDWidget);
    m_pNFDWidget->setGlobal(getShortcuts(), getGlobalOptions());
    m_pNFDWidget->setReadonly(isReadonly());
    m_pNFDWidget->setData(m_inData, false);
}

void XFWidget_NFDScan::clear()
{
    delete m_pNFDWidget;
    m_pNFDWidget = nullptr;
    m_inData = {};
}

void XFWidget_NFDScan::reload()
{
    if (m_pNFDWidget) m_pNFDWidget->reloadData(false);
}

void XFWidget_NFDScan::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    XShortcutsWidget::setGlobal(pShortcuts, pXOptions);
    if (m_pNFDWidget) m_pNFDWidget->setGlobal(pShortcuts, pXOptions);
}

void XFWidget_NFDScan::setReadonly(bool bIsReadonly)
{
    XShortcutsWidget::setReadonly(bIsReadonly);
    if (m_pNFDWidget) m_pNFDWidget->setReadonly(bIsReadonly);
}

NFDWidgetAdvanced *XFWidget_NFDScan::getNFDWidget()
{
    return m_pNFDWidget;
}

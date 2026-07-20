#include "xfwidget_hash.h"

#include <QVBoxLayout>

XFWidget_Hash::XFWidget_Hash(QWidget *pParent) : XShortcutsWidget(pParent)
{
    m_inData = {};
    m_pHashWidget = nullptr;
    QVBoxLayout *pLayout = new QVBoxLayout(this);
    pLayout->setContentsMargins(0, 0, 0, 0);
}

XFWidget_Hash::~XFWidget_Hash()
{
    clear();
}

void XFWidget_Hash::setData(const XBinary::INDATA &inData)
{
    clear();
    m_inData = inData;
    m_inData.pDevice = XFormats::createDevice(inData);
    m_pHashWidget = new XHashWidget(this);
    layout()->addWidget(m_pHashWidget);
    m_pHashWidget->setGlobal(getShortcuts(), getGlobalOptions());
    m_pHashWidget->setReadonly(isReadonly());
    qint64 nSize = m_inData.pDevice ? m_inData.pDevice->size() : 0;
    m_pHashWidget->setData(m_inData.pDevice, m_inData.fileType, 0, nSize, false);
}

void XFWidget_Hash::clear()
{
    delete m_pHashWidget;
    m_pHashWidget = nullptr;
    XFormats::removeDevice(m_inData.pDevice, m_inData);
    m_inData = {};
}

void XFWidget_Hash::reload()
{
    if (m_pHashWidget) m_pHashWidget->reload();
}

void XFWidget_Hash::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    XShortcutsWidget::setGlobal(pShortcuts, pXOptions);
    if (m_pHashWidget) m_pHashWidget->setGlobal(pShortcuts, pXOptions);
}

void XFWidget_Hash::setReadonly(bool bIsReadonly)
{
    XShortcutsWidget::setReadonly(bIsReadonly);
    if (m_pHashWidget) m_pHashWidget->setReadonly(bIsReadonly);
}

XHashWidget *XFWidget_Hash::getHashWidget()
{
    return m_pHashWidget;
}

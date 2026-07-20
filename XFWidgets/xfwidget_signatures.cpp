#include "xfwidget_signatures.h"

#include <QVBoxLayout>

XFWidget_Signatures::XFWidget_Signatures(QWidget *pParent) : XShortcutsWidget(pParent)
{
    m_inData = {};
    m_pSignaturesWidget = nullptr;
    QVBoxLayout *pLayout = new QVBoxLayout(this);
    pLayout->setContentsMargins(0, 0, 0, 0);
}

XFWidget_Signatures::~XFWidget_Signatures()
{
    clear();
}

void XFWidget_Signatures::setData(const XBinary::INDATA &inData)
{
    clear();
    m_inData = inData;
    m_inData.pDevice = XFormats::createDevice(inData);
    m_pSignaturesWidget = new SearchSignaturesWidget(this);
    layout()->addWidget(m_pSignaturesWidget);
    m_pSignaturesWidget->setGlobal(getShortcuts(), getGlobalOptions());
    m_pSignaturesWidget->setReadonly(isReadonly());
    SearchSignaturesWidget::OPTIONS options = {};
    options.fileType = m_inData.fileType;
    options.bMenu_Hex = true;
    options.bMenu_Disasm = true;
    m_pSignaturesWidget->setData(m_inData.pDevice, options, false);
}

void XFWidget_Signatures::clear()
{
    delete m_pSignaturesWidget;
    m_pSignaturesWidget = nullptr;
    XFormats::removeDevice(m_inData.pDevice, m_inData);
    m_inData = {};
}

void XFWidget_Signatures::reload()
{
    if (m_pSignaturesWidget) m_pSignaturesWidget->reload();
}

void XFWidget_Signatures::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    XShortcutsWidget::setGlobal(pShortcuts, pXOptions);
    if (m_pSignaturesWidget) m_pSignaturesWidget->setGlobal(pShortcuts, pXOptions);
}

void XFWidget_Signatures::setReadonly(bool bIsReadonly)
{
    XShortcutsWidget::setReadonly(bIsReadonly);
    if (m_pSignaturesWidget) m_pSignaturesWidget->setReadonly(bIsReadonly);
}

SearchSignaturesWidget *XFWidget_Signatures::getSignaturesWidget()
{
    return m_pSignaturesWidget;
}

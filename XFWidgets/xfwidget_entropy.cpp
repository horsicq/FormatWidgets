#include "xfwidget_entropy.h"

#include <QVBoxLayout>

XFWidget_Entropy::XFWidget_Entropy(QWidget *pParent) : XShortcutsWidget(pParent)
{
    m_inData = {};
    m_pEntropyWidget = nullptr;
    QVBoxLayout *pLayout = new QVBoxLayout(this);
    pLayout->setContentsMargins(0, 0, 0, 0);
}

XFWidget_Entropy::~XFWidget_Entropy()
{
    clear();
}

void XFWidget_Entropy::setData(const XBinary::INDATA &inData)
{
    clear();
    m_inData = inData;
    m_pEntropyWidget = new XEntropyWidget(this);
    layout()->addWidget(m_pEntropyWidget);
    m_pEntropyWidget->setGlobal(getShortcuts(), getGlobalOptions());
    m_pEntropyWidget->setReadonly(isReadonly());
    m_pEntropyWidget->setData(m_inData, 0, -1, false);
}

void XFWidget_Entropy::clear()
{
    delete m_pEntropyWidget;
    m_pEntropyWidget = nullptr;
    m_inData = {};
}

void XFWidget_Entropy::reload()
{
    if (m_pEntropyWidget) m_pEntropyWidget->reload(true, true);
}

void XFWidget_Entropy::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    XShortcutsWidget::setGlobal(pShortcuts, pXOptions);
    if (m_pEntropyWidget) m_pEntropyWidget->setGlobal(pShortcuts, pXOptions);
}

void XFWidget_Entropy::setReadonly(bool bIsReadonly)
{
    XShortcutsWidget::setReadonly(bIsReadonly);
    if (m_pEntropyWidget) m_pEntropyWidget->setReadonly(bIsReadonly);
}

XEntropyWidget *XFWidget_Entropy::getEntropyWidget()
{
    return m_pEntropyWidget;
}

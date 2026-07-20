#include "xfwidget_search.h"

#include <QVBoxLayout>

XFWidget_Search::XFWidget_Search(QWidget *pParent) : XShortcutsWidget(pParent)
{
    m_inData = {};
    m_pSearchWidget = nullptr;
    QVBoxLayout *pLayout = new QVBoxLayout(this);
    pLayout->setContentsMargins(0, 0, 0, 0);
}

XFWidget_Search::~XFWidget_Search()
{
    clear();
}

void XFWidget_Search::setData(const XBinary::INDATA &inData)
{
    clear();
    m_inData = inData;
    m_inData.pDevice = XFormats::createDevice(inData);
    m_pSearchWidget = new SearchValuesWidget(this);
    layout()->addWidget(m_pSearchWidget);
    m_pSearchWidget->setGlobal(getShortcuts(), getGlobalOptions());
    m_pSearchWidget->setReadonly(isReadonly());
    SearchValuesWidget::OPTIONS options = {};
    options.fileType = m_inData.fileType;
    options.bMenu_Hex = true;
    options.bMenu_Disasm = true;
    m_pSearchWidget->setData(m_inData.pDevice, options);
}

void XFWidget_Search::clear()
{
    delete m_pSearchWidget;
    m_pSearchWidget = nullptr;
    XFormats::removeDevice(m_inData.pDevice, m_inData);
    m_inData = {};
}

void XFWidget_Search::reload()
{
    if (m_pSearchWidget) m_pSearchWidget->reloadData(false);
}

void XFWidget_Search::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    XShortcutsWidget::setGlobal(pShortcuts, pXOptions);
    if (m_pSearchWidget) m_pSearchWidget->setGlobal(pShortcuts, pXOptions);
}

void XFWidget_Search::setReadonly(bool bIsReadonly)
{
    XShortcutsWidget::setReadonly(bIsReadonly);
    if (m_pSearchWidget) m_pSearchWidget->setReadonly(bIsReadonly);
}

SearchValuesWidget *XFWidget_Search::getSearchWidget()
{
    return m_pSearchWidget;
}

#include "xfwidget_extractor.h"

#include <QVBoxLayout>

XFWidget_Extractor::XFWidget_Extractor(QWidget *pParent) : XShortcutsWidget(pParent)
{
    m_inData = {};
    m_pXInfoDB = nullptr;
    m_pExtractorWidget = nullptr;
    QVBoxLayout *pLayout = new QVBoxLayout(this);
    pLayout->setContentsMargins(0, 0, 0, 0);
}

XFWidget_Extractor::~XFWidget_Extractor()
{
    clear();
}

void XFWidget_Extractor::setData(const XBinary::INDATA &inData)
{
    clear();
    m_inData = inData;
    m_pExtractorWidget = new XExtractorWidget(this);
    layout()->addWidget(m_pExtractorWidget);
    m_pExtractorWidget->setGlobal(getShortcuts(), getGlobalOptions());
    m_pExtractorWidget->setReadonly(isReadonly());
    XExtractor::OPTIONS options = XExtractor::getDefaultOptions();
    options.fileType = m_inData.fileType;
    options.bMenu_Hex = true;
    m_pExtractorWidget->setData(m_inData, nullptr, options, false);

    m_pXInfoDB = new XInfoDB(this);
    if (m_pExtractorWidget->getDevice()) m_pXInfoDB->setData(m_pExtractorWidget->getDevice(), m_inData.fileType);
    m_pExtractorWidget->setXInfoDB(m_pXInfoDB);
}

void XFWidget_Extractor::clear()
{
    delete m_pExtractorWidget;
    m_pExtractorWidget = nullptr;
    delete m_pXInfoDB;
    m_pXInfoDB = nullptr;
    m_inData = {};
}

void XFWidget_Extractor::reload()
{
    if (m_pExtractorWidget) m_pExtractorWidget->reload();
}

void XFWidget_Extractor::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    XShortcutsWidget::setGlobal(pShortcuts, pXOptions);
    if (m_pExtractorWidget) m_pExtractorWidget->setGlobal(pShortcuts, pXOptions);
}

void XFWidget_Extractor::setReadonly(bool bIsReadonly)
{
    XShortcutsWidget::setReadonly(bIsReadonly);
    if (m_pExtractorWidget) m_pExtractorWidget->setReadonly(bIsReadonly);
}

XExtractorWidget *XFWidget_Extractor::getExtractorWidget()
{
    return m_pExtractorWidget;
}

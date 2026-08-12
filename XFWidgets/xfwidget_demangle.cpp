#include "xfwidget_demangle.h"

#include <QVBoxLayout>

XFWidget_Demangle::XFWidget_Demangle(QWidget *pParent) : XShortcutsWidget(pParent)
{
    m_pDemangleWidget = nullptr;
    QVBoxLayout *pLayout = new QVBoxLayout(this);
    pLayout->setContentsMargins(0, 0, 0, 0);
}

XFWidget_Demangle::~XFWidget_Demangle()
{
    clear();
}

void XFWidget_Demangle::setData(const XBinary::INDATA &inData)
{
    Q_UNUSED(inData)

    clear();
    m_pDemangleWidget = new XDemangleWidget(this);
    layout()->addWidget(m_pDemangleWidget);
    m_pDemangleWidget->setGlobal(getShortcuts(), getGlobalOptions());
}

void XFWidget_Demangle::clear()
{
    delete m_pDemangleWidget;
    m_pDemangleWidget = nullptr;
}

void XFWidget_Demangle::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    XShortcutsWidget::setGlobal(pShortcuts, pXOptions);
    if (m_pDemangleWidget) m_pDemangleWidget->setGlobal(pShortcuts, pXOptions);
}

void XFWidget_Demangle::setReadonly(bool bIsReadonly)
{
    XShortcutsWidget::setReadonly(bIsReadonly);
}

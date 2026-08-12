#include "xfwidget_yarascan.h"

#include <QVBoxLayout>

XFWidget_YaraScan::XFWidget_YaraScan(QWidget *pParent) : XShortcutsWidget(pParent)
{
    m_inData = {};
    m_pYaraWidget = nullptr;
    QVBoxLayout *pLayout = new QVBoxLayout(this);
    pLayout->setContentsMargins(0, 0, 0, 0);
}

XFWidget_YaraScan::~XFWidget_YaraScan()
{
    clear();
}

void XFWidget_YaraScan::setData(const XBinary::INDATA &inData)
{
    clear();
    m_inData = inData;

    QString sFileName = m_inData.sFileName;

    if (sFileName.isEmpty() && m_inData.pDevice) {
        sFileName = XBinary::getDeviceFileName(m_inData.pDevice);
    }

    m_pYaraWidget = new YARAWidgetAdvanced(this);
    layout()->addWidget(m_pYaraWidget);
    m_pYaraWidget->setGlobal(getShortcuts(), getGlobalOptions());
    m_pYaraWidget->setReadonly(isReadonly());
    m_pYaraWidget->setData(sFileName, true);
}

void XFWidget_YaraScan::clear()
{
    delete m_pYaraWidget;
    m_pYaraWidget = nullptr;
    m_inData = {};
}

void XFWidget_YaraScan::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    XShortcutsWidget::setGlobal(pShortcuts, pXOptions);
    if (m_pYaraWidget) m_pYaraWidget->setGlobal(pShortcuts, pXOptions);
}

void XFWidget_YaraScan::setReadonly(bool bIsReadonly)
{
    XShortcutsWidget::setReadonly(bIsReadonly);
    if (m_pYaraWidget) m_pYaraWidget->setReadonly(bIsReadonly);
}

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
    connect(m_pYaraWidget, SIGNAL(followLocation(quint64, qint32, qint64, qint32)), this, SIGNAL(followLocation(quint64, qint32, qint64, qint32)));
    connect(m_pYaraWidget, SIGNAL(currentLocationChanged(quint64, qint32, qint64)), this, SIGNAL(currentLocationChanged(quint64, qint32, qint64)));
    layout()->addWidget(m_pYaraWidget);
    m_pYaraWidget->setGlobal(getShortcuts(), getGlobalOptions());
    m_pYaraWidget->setReadonly(isReadonly());

    // Auto-scan only when the configured rules path exists; otherwise merely selecting the
    // node would pop a modal "YARA rules path not found" error. The user can still press
    // Scan, which reports the error on demand.
    bool bScan = false;
    XOptions *pOptions = getGlobalOptions();

    if (pOptions) {
        QString sRulesPath = pOptions->getValue(XOptions::ID_SCAN_YARA_DATABASE_PATH).toString();

        if (!sRulesPath.isEmpty()) {
            bScan = XOptions::isPathExists(XOptions::convertPathName(sRulesPath));
        }
    }

    m_pYaraWidget->setData(sFileName, bScan);
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

#include "xfwidget_memorymap.h"

#include <QVBoxLayout>

XFWidget_MemoryMap::XFWidget_MemoryMap(QWidget *pParent) : XShortcutsWidget(pParent)
{
    m_inData = {};
    m_pXInfoDB = nullptr;
    m_pMemoryMapWidget = nullptr;
    QVBoxLayout *pLayout = new QVBoxLayout(this);
    pLayout->setContentsMargins(0, 0, 0, 0);
}

XFWidget_MemoryMap::~XFWidget_MemoryMap()
{
    clear();
}

void XFWidget_MemoryMap::setData(const XBinary::INDATA &inData)
{
    clear();
    m_inData = inData;
    m_pMemoryMapWidget = new XMemoryMapWidget(this);
    connect(m_pMemoryMapWidget, SIGNAL(followLocation(quint64, qint32, qint64, qint32)), this, SIGNAL(followLocation(quint64, qint32, qint64, qint32)));
    connect(m_pMemoryMapWidget, SIGNAL(currentLocationChanged(quint64, qint32, qint64)), this, SIGNAL(currentLocationChanged(quint64, qint32, qint64)));
    connect(m_pMemoryMapWidget, SIGNAL(findValue(quint64, XBinary::ENDIAN)), this, SIGNAL(findValue(quint64, XBinary::ENDIAN)));
    layout()->addWidget(m_pMemoryMapWidget);
    m_pMemoryMapWidget->setGlobal(getShortcuts(), getGlobalOptions());
    m_pMemoryMapWidget->setReadonly(isReadonly());
    XMemoryMapWidget::OPTIONS options = {};
    options.fileType = m_inData.fileType;
    options.bIsSearchEnable = true;
    m_pMemoryMapWidget->setData(m_inData, options);

    m_pXInfoDB = new XInfoDB(this);
    if (m_pMemoryMapWidget->getDevice()) m_pXInfoDB->setData(m_pMemoryMapWidget->getDevice(), m_inData.fileType);
    m_pMemoryMapWidget->setXInfoDB(m_pXInfoDB);
}

void XFWidget_MemoryMap::clear()
{
    delete m_pMemoryMapWidget;
    m_pMemoryMapWidget = nullptr;
    delete m_pXInfoDB;
    m_pXInfoDB = nullptr;
    m_inData = {};
}

void XFWidget_MemoryMap::reload()
{
    if (m_pMemoryMapWidget) m_pMemoryMapWidget->reloadData(false);
}

void XFWidget_MemoryMap::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    XShortcutsWidget::setGlobal(pShortcuts, pXOptions);
    if (m_pMemoryMapWidget) m_pMemoryMapWidget->setGlobal(pShortcuts, pXOptions);
}

void XFWidget_MemoryMap::setReadonly(bool bIsReadonly)
{
    XShortcutsWidget::setReadonly(bIsReadonly);
    if (m_pMemoryMapWidget) m_pMemoryMapWidget->setReadonly(bIsReadonly);
}

void XFWidget_MemoryMap::setLocation(quint64 nLocation, qint32 nLocationType, qint64 nSize)
{
    if (m_pMemoryMapWidget) m_pMemoryMapWidget->setLocation(nLocation, nLocationType, nSize);
}

XMemoryMapWidget *XFWidget_MemoryMap::getMemoryMapWidget()
{
    return m_pMemoryMapWidget;
}

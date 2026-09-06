#ifndef XFWIDGET_MEMORYMAP_H
#define XFWIDGET_MEMORYMAP_H

#include "xformats.h"
#include "xmemorymapwidget.h"

class XFWidget_MemoryMap : public XShortcutsWidget {
    Q_OBJECT

public:
    explicit XFWidget_MemoryMap(QWidget *pParent = nullptr);
    ~XFWidget_MemoryMap() override;

    void setData(const XBinary::INDATA &inData);
    void clear();
    void reload();
    void setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions) override;
    void setReadonly(bool bIsReadonly) override;
    void setLocation(quint64 nLocation, qint32 nLocationType, qint64 nSize) override;
    XMemoryMapWidget *getMemoryMapWidget();

signals:
    void findValue(quint64 nValue, XBinary::ENDIAN endian);

private:
    XBinary::INDATA m_inData;
    XInfoDB *m_pXInfoDB;
    XMemoryMapWidget *m_pMemoryMapWidget;
};

#endif

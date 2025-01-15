/* Copyright (c) 2017-2024 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef XFORMATWIDGET_H
#define XFORMATWIDGET_H

#include <QHeaderView>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QTreeWidgetItem>
#include <QHBoxLayout>

#include "xformatwidget_def.h"
#include "xcomboboxex.h"
#include "xdatetimeeditx.h"
#include "xlineedithex.h"
#include "xshortcutswidget.h"

#include "Structs/generic_defs.h"
#include "Structs/generic_defs.h"
#include "Structs/xdex_defs.h"
#include "Structs/xelf_defs.h"
#include "Structs/xle_defs.h"
#include "Structs/xmach_defs.h"
#include "Structs/xmsdos_defs.h"
#include "Structs/xpdf_defs.h"
#include "Structs/xpdf_defs.h"
#include "Structs/xne_defs.h"
#include "Structs/xpe_defs.h"
#include "Structs/xsevenzip_defs.h"

class XFormatWidget : public XShortcutsWidget {
    Q_OBJECT

public:
    enum HEADER_COLUMN {
        HEADER_COLUMN_NAME = 0,
        HEADER_COLUMN_OFFSET,
        HEADER_COLUMN_SIZE,
        HEADER_COLUMN_TYPE,
        HEADER_COLUMN_VALUE,
        HEADER_COLUMN_INFO,
        HEADER_COLUMN_COMMENT
    };

    enum LIST_COLUMN {
        LIST_COLUMN_NAME = 0,
        LIST_COLUMN_VALUE
    };

    enum HEADER_DATA {
        HEADER_DATA_OFFSET = 0,
        HEADER_DATA_SIZE
    };

    enum DIRECTORY_COLUMN {
        DIRECTORY_COLUMN_NUMBER = 0,
        DIRECTORY_COLUMN_NAME,
        DIRECTORY_COLUMN_ADDRESS,
        DIRECTORY_COLUMN_SIZE
    };

    struct RECWIDGET {
        qint32 nPosition;
        qint64 nOffset;
        qint64 nSize;
        qint32 nVType;
        qint32 nSubPosition;
        XBinary::ENDIAN endian;
        XLineEditHEX *pLineEdit;
        QWidget *pWidget;
        QTableWidgetItem *pValue;
        QTableWidgetItem *pComment;
        QVariant varDelta;
    };

    XFormatWidget(QWidget *pParent = nullptr);
    XFormatWidget(QIODevice *pDevice, XFW_DEF::OPTIONS options, QWidget *pParent);
    ~XFormatWidget();

    void setXInfoDB(XInfoDB *pXInfoDB);
    XInfoDB *getXInfoDB();

    void setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions);
    virtual void adjustView();
    void setData(QIODevice *pDevice, const XFW_DEF::OPTIONS &options);
    void setData(const QString &sFileName, const XFW_DEF::OPTIONS &options);
    void setData(const XFW_DEF::OPTIONS &options);
    virtual void setCwOptions(const XFW_DEF::CWOPTIONS &cwOptions, bool bReload);
    XFW_DEF::CWOPTIONS *getCwOptions();

    void setFileType(XBinary::FT fileType);
    XBinary::FT getFileType();
    void setMode(XBinary::MODE mode);
    XBinary::MODE getMode();
    void setEndian(XBinary::ENDIAN endian);
    XBinary::ENDIAN getEndian();
    void setMemoryMap(const XBinary::_MEMORY_MAP &memoryMap);
    XBinary::_MEMORY_MAP getMemoryMap();

    QIODevice *getDevice();
    virtual void setOptions(XFW_DEF::OPTIONS options);  // TODO for all Widgets
    XFW_DEF::OPTIONS getOptions();
    static QTreeWidgetItem *createNewItem(XFW_DEF::TYPE type, XFW_DEF::WIDGETMODE widgetMode, XOptions::ICONTYPE iconType, qint64 nOffset, qint64 nSize, qint64 nCount,
                                          QVariant var1, QVariant var2, XBinary::MODE mode, XBinary::ENDIAN endian, QString sTitle, QString sInfo);
    static QString getTypeTitle(XFW_DEF::TYPE type, XBinary::MODE mode, XBinary::ENDIAN endian, QString sInfo);
    static QString getTypeTitle(const XFW_DEF::CWOPTIONS *pCwOptions);

    static QList<XFW_DEF::HEADER_RECORD> getHeaderRecords(const XFW_DEF::CWOPTIONS *pCwOptions, qint32 nLimit);
    static qint64 getStructSize(XFW_DEF::TYPE type);

    static qint32 getHeaderSize(const QList<XFW_DEF::HEADER_RECORD> *pListHeaderRecords);

    bool createHeaderTable(QTableWidget *pTableWidget, const QList<XFW_DEF::HEADER_RECORD> *pListHeaderRecords, QList<RECWIDGET> *pListRecWidget, qint64 nOffset,
                           XBinary::ENDIAN endian, QVariant var1, QVariant var2);
    void addComboBox(QTableWidget *pTableWidget, QList<RECWIDGET> *pListRecWidget, const QMap<quint64, QString> &mapData, qint32 nPosition, XComboBoxEx::CBTYPE cbtype,
                     quint64 nMask);
    void _adjustCellWidget(QList<RECWIDGET> *pListRecWidget, QTableWidget *pTableWidget, qint32 nRow, qint32 nColumn);
    // InvWidget *createInvWidget(QTableWidget *pTableWidget, qint32 nType, qint32 nData, InvWidget::TYPE widgetType);
    XDateTimeEditX *createTimeDateEdit(QTableWidget *pTableWidget, qint32 nType, qint32 nData);
    QPushButton *createPushButton(QTableWidget *pTableWidget, qint32 nType, qint32 nData, const QString &sText);

    void setValue(QVariant vValue, qint32 nPosition, qint64 nOffset, qint64 nSize);

    //    QPushButton *createHexButton(QTableWidget *pTableWidget,int nType,int
    //    nData);

    void setHeaderSelection(QTableWidget *pTableWidget);
    void setTableSelection(QTableView *pTableView);

    QColor getEnabledColor();
    QColor getDisabledColor();

    void setItemEnable(QTableWidgetItem *pItem, bool bState);
    void setLineEdit(XLineEditHEX *pLineEdit, qint32 nMaxLength, const QString &sText, qint64 nOffset);
    void dumpSection(QTableView *pTableView);
    void dumpAll(QTableView *pTableView);

    static bool _setTreeItem(QTreeWidget *pTree, QTreeWidgetItem *pItem, qint32 nID);
    static void setTreeItem(QTreeWidget *pTree, qint32 nID);
    virtual void clear();
    virtual void cleanup();
    virtual void reload();
    virtual void setReadonly(bool bState);

    QList<RECWIDGET> *getListRecWidgets();

    void reset();
    static QString getInitStringFromCwOptions(XFW_DEF::CWOPTIONS *pCwOptions);
    static QString _getInitString(XFW_DEF::TYPE _type, qint64 nDataOffset, qint64 nDataSize, qint64 nDataCount);
    static XFW_DEF::TYPE _getTypeFromInitString(const QString &sInitString);
    static qint64 _getDataOffsetFromInitString(const QString &sInitString);
    static qint64 _getDataSizeFromInitString(const QString &sInitString);
    static qint64 _getDataCountFromInitString(const QString &sInitString);

    void addInit(const QString &sString);
    bool isInitPresent(const QString &sString);

    void addPage(QTreeWidgetItem *pItem);
    void setAddPageEnabled(bool bEnable);
    QTreeWidgetItem *getPrevPage();
    QTreeWidgetItem *getNextPage();
    bool isPrevPageAvailable();
    bool isNextPageAvailable();

    void updateRecWidgets(QIODevice *pDevice, QList<RECWIDGET> *pListRecWidget);
    void _adjustRecWidget(RECWIDGET *pRecWidget, QVariant varValue);

    static QVariant _readVariant(XBinary *pBinary, qint64 nOffset, qint64 nSize, qint32 vtype, bool bIsBigEndian);
    static QStandardItem *setItemToModel(QStandardItemModel *pModel, qint32 nRow, qint32 nColumn, const QVariant &var, qint64 nSize, qint32 vtype);
    static QStandardItem *setItemToModelData(QStandardItemModel *pModel, qint32 nRow, qint32 nColumn, const QVariant &var, qint64 nSize, qint32 vtype, XFW_DEF::TYPE type,
                                             qint64 nHeaderOffset, qint64 nHeaderSize, qint64 nDataOffset, qint64 nDataSize, qint64 nDataCount);

    enum CW {
        CW_UINT8 = 0,
        CW_UINT16,
        CW_UINT32,
        CW_UINT64,
        CW_UINTMODE,
        CW_TYPE,
        CW_STRINGSHORT,
        CW_STRINGSHORT2,
        CW_STRINGMID,
        CW_STRINGLONG
    };

    static qint32 getColumnWidth(QWidget *pParent, CW cw, XBinary::MODE mode);

    void setDisasmInitAddress(XADDR nDisasmInitAddress);  // TODO remove
    XADDR getDisasmInitAddress();                         // TODO remove

    QStandardItemModel *getHeaderTableModel(QTableWidget *pTableWidget);
    void saveHeaderTable(QTableWidget *pTableWidget, const QString &sFileName);

    void saveModel(QAbstractItemModel *pModel, const QString &sFileName);

    static void _addFileType(QTreeWidgetItem *pTreeWidgetItem, QIODevice *pDevice, qint64 nOffset, qint64 nSize, XBinary::FT fileType, bool bIsImage, XADDR nImageBase);

    struct SPSTRUCT {
        QTreeWidgetItem *pTreeWidgetItem;
        QIODevice *pDevice;
        qint64 nOffset;
        qint64 nSize;
        XBinary::FT fileType;
        bool bIsImage;
        XADDR nImageBase;
        qint64 nStructOffset;
        qint64 nStructSize;
        qint32 nStructCount;
        QVariant var1;
        QVariant var2;
        XFW_DEF::TYPE type;
        XFW_DEF::WIDGETMODE widgetMode;
        XBinary::MODE mode;
        XBinary::ENDIAN endian;
        QString sTitle;
        QString sInfo;
    };

    static void _addStruct(const SPSTRUCT &spStruct);
    static XFW_DEF::TYPE load_commandIdToType(qint32 nCommandId);

    static QString widgetModeIdToString(XFW_DEF::WIDGETMODE widgetMode);
    static void adjustWidgetModeComboBox(QComboBox *pComboBox, XFW_DEF::WIDGETMODE widgetMode = XFW_DEF::WIDGETMODE_UNKNOWN);

protected:
    enum SV {
        SV_NONE = 0,
        SV_EDITED,
        SV_RELOADALL,
        SV_RELOADDATA
    };

    void adjustGenericHeader(QTableWidget *pTableWidget, const QList<XFW_DEF::HEADER_RECORD> *pListHeaderRecords);
    void adjustGenericTable(QTableView *pTableView, const QList<XFW_DEF::HEADER_RECORD> *pListHeaderRecords);
    virtual void adjustListTable(qint32 nType, QTableWidget *pTableWidget);
    virtual QString typeIdToString(qint32 nType);
    virtual void _widgetValueChanged(QVariant vValue);
    void contextMenuGenericHeaderWidget(const QPoint &pos, QTableWidget *pTableWidget, QList<RECWIDGET> *pListRecWidget, XFW_DEF::CWOPTIONS *pCwOptions);
    void contextMenuGenericTableWidget(const QPoint &pos, QTableView *pTableView, QList<RECWIDGET> *pListRecWidget, XFW_DEF::CWOPTIONS *pCwOptions);
    void tableView_doubleClicked(QTableView *pTableView, const QModelIndex &index);
    virtual void _followLocation(quint64 nLocation, qint32 nLocationType, qint64 nSize, qint32 nWidgetType);

signals:
    void closeApp();
    void showCwWidget(QString sInitString, bool nNewWindow);

public slots:
    void valueChangedSlot(QVariant varValue);
    void setEdited(qint64 nDeviceOffset, qint64 nDeviceSize);
    void allReload(qint64 nDeviceOffset, qint64 nDeviceSize);
    // void showHex(qint64 nOffset, qint64 nSize);
    void widgetValueChanged(QVariant vValue);

    void dumpRegion(qint64 nOffset, qint64 nSize, const QString &sName);
    void _reload();
    void showTableRecord();
    void followLocationSlot(quint64 nLocation, qint32 nLocationType, qint64 nSize, qint32 nWidgetType);

private slots:
    void onToolButtonClicked();

protected:
    virtual void registerShortcuts(bool bState);

private:
    QIODevice *g_pDevice;
    QString g_sFileName;
    XFW_DEF::OPTIONS g_fwOptions;
    QColor g_colEnabled;
    QColor g_colDisabled;
    XBinary::FT g_fileType;
    XBinary::MODE g_mode;
    XBinary::ENDIAN g_endian;
    XBinary::_MEMORY_MAP g_memoryMap;
    QList<QTreeWidgetItem *> g_listPages;
    qint32 g_nPageIndex;
    bool g_bAddPageEnable;
    QMap<QString, QString> g_mapInit;
    XInfoDB *g_pXInfoDB;
    XADDR g_nDisamInitAddress;
    XFW_DEF::CWOPTIONS g_cwOptions;
    QList<RECWIDGET> listRecWidget;
};

#endif  // XFORMATWIDGET_H

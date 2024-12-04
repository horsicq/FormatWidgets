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

#include "xformatwidget_def.h"
#include "xcomboboxex.h"
#include "xdatetimeeditx.h"
#include "xlineedithex.h"
#include "xshortcutswidget.h"
#include "Specific/xelf_defs.h"
// #include "Specific/pe_defs.h"
#include "Specific/xmach_defs.h"
#include "Specific/xmsdos_defs.h"
// #include "Specific/ne_defs.h"
// #include "vLE/le_defs.h"

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
        qint32 vtype;
        XBinary::ENDIAN endian;
        XLineEditHEX *pLineEdit;
        XComboBoxEx *pComboBox;
        QTableWidgetItem *pComment;
    };

    XFormatWidget(QWidget *pParent = nullptr);
    XFormatWidget(QIODevice *pDevice, XFW_DEF::OPTIONS options, quint32 nNumber, qint64 nOffset, qint32 nType, QWidget *pParent);
    ~XFormatWidget();

    void setXInfoDB(XInfoDB *pXInfoDB);
    XInfoDB *getXInfoDB();

    void setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions);
    virtual void adjustView();
    void setData(QIODevice *pDevice, XFW_DEF::OPTIONS options, quint32 nNumber, qint64 nOffset, qint32 nType);
    void setData(const QString &sFileName, XFW_DEF::OPTIONS options, quint32 nNumber, qint64 nOffset, qint32 nType);
    void setData(XFW_DEF::OPTIONS options, quint32 nNumber, qint64 nOffset, qint32 nType);
    void setCwOptions(XFW_DEF::CWOPTIONS cwOptions, bool bReload);
    XFW_DEF::CWOPTIONS *getCwOptions();

    void setFileType(XBinary::FT fileType);
    XBinary::FT getFileType();
    void setMode(XBinary::MODE mode);
    XBinary::MODE getMode();
    void setEndian(XBinary::ENDIAN endian);
    XBinary::ENDIAN getEndian();

    QIODevice *getDevice();
    virtual void setOptions(XFW_DEF::OPTIONS options);  // TODO for all Widgets
    XFW_DEF::OPTIONS getOptions();
    quint32 getNumber();
    qint64 getOffset();
    qint32 getType();
    static QTreeWidgetItem *createNewItem(XFW_DEF::TYPE type, XFW_DEF::WIDGETMODE widgetMode, const QString &sTitle, XOptions::ICONTYPE iconType, qint64 nOffset,
                                          qint64 nSize, qint64 nCount, QVariant var1, QVariant var2, XBinary::MODE mode, XBinary::ENDIAN endian);

    static QList<XFW_DEF::HEADER_RECORD> getHeaderRecords(const XFW_DEF::CWOPTIONS *pCwOptions);
    static qint32 getHeaderSize(QList<XFW_DEF::HEADER_RECORD> *pListHeaderRecords);

    bool createHeaderTable(QTableWidget *pTableWidget, const QList<XFW_DEF::HEADER_RECORD> *pListHeaderRecords, QList<RECWIDGET> *pListRecWidget, qint64 nOffset,
                           XBinary::ENDIAN endian);
    bool createListTable(qint32 nType, QTableWidget *pTableWidget, const XFW_DEF::HEADER_RECORD *pRecords, XLineEditHEX **ppLineEdits, qint32 nNumberOfRecords);
    void addComment(QTableWidget *pTableWidget, qint32 nRow, qint32 nColumn, const QString &sComment);
    void updateTableRecord(QTableWidget *pTableWidget, qint32 nRow, qint64 nOffset, qint64 nSize);
    //    bool createDirectoryTable(int type,QTableWidget *pTableWidget,const
    //    DIRECTORY_ENTRY_RECORD *pRecords,int nRecordCount);
    bool createSectionTable(qint32 nType, QTableWidget *pTableWidget, const XFW_DEF::HEADER_RECORD *pRecords, qint32 nNumberOfRecords);
    void setLineEditsReadOnly(XLineEditHEX **ppLineEdits, qint32 nCount, bool bState);
    void setComboBoxesReadOnly(XComboBoxEx **ppComboBoxes, qint32 nCount, bool bState);
    void setPushButtonReadOnly(QPushButton **ppPushButtons, qint32 nCount, bool bState);
    void setDateTimeEditReadOnly(XDateTimeEditX **ppDateTimeEdits, qint32 nCount, bool bState);
    void _deleteObjects(QObject **ppObjects, qint32 nCount);
    void _deleteSubdevices(SubDevice **ppSubdevices, qint32 nCount);

    XComboBoxEx *createComboBox(QTableWidget *pTableWidget, QMap<quint64, QString> mapData, qint32 nType, qint32 nData, XComboBoxEx::CBTYPE cbtype, quint64 nMask = 0,
                                qint32 nExtraData = -1);
    void adjustComboBox(QTableWidget *pTableWidget, QList<RECWIDGET> *pListRecWidget, const QMap<quint64, QString> &mapData, qint32 nPosition, XComboBoxEx::CBTYPE cbtype,
                        quint64 nMask);
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
    void showSectionHex(QTableView *pTableView);
    void dumpSection(QTableView *pTableView);
    void dumpAll(QTableView *pTableView);
    qint64 getTableViewItemSize(QTableView *pTableView);
    void showTableViewDemangle(QTableView *pTableView, qint32 nColumn);

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
    static QStandardItem *setItemToModelData(QStandardItemModel *pModel, qint32 nRow, qint32 nColumn, const QVariant &var, qint64 nSize, qint32 vtype, XFW_DEF::TYPE type, qint64 nHeaderOffset, qint64 nHeaderSize, qint64 nDataOffset, qint64 nDataSize, qint64 nDataCount);

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

    static void _addSpecItems(QTreeWidget *pTreeWidget, QIODevice *pDevice, XBinary::FT fileType, bool bIsImage, XADDR nImageBase);
    static XFW_DEF::TYPE MACH_commandIdToType(qint32 nCommandId);

protected:
    enum SV {
        SV_NONE = 0,
        SV_EDITED,
        SV_RELOADALL,
        SV_RELOADDATA
    };

    virtual SV _setValue(QVariant vValue, qint32 nPosition) = 0;
    void adjustGenericHeader(QTableWidget *pTableWidget, const QList<XFW_DEF::HEADER_RECORD> *pListHeaderRecords);
    void adjustGenericTable(QTableView *pTableView, const QList<XFW_DEF::HEADER_RECORD> *pListHeaderRecords);
    virtual void adjustListTable(qint32 nType, QTableWidget *pTableWidget);
    virtual QString typeIdToString(qint32 nType);
    virtual void _showInDisasmWindowAddress(XADDR nAddress);
    virtual void _showInDisasmWindowOffset(qint64 nOffset);
    virtual void _showInMemoryMapWindowOffset(qint64 nOffset);
    virtual void _showInHexWindow(qint64 nOffset, qint64 nSize);
    virtual void _findValue(quint64 nValue, XBinary::ENDIAN endian);
    virtual void _widgetValueChanged(QVariant vValue);
    void contextMenuGenericHeaderWidget(const QPoint &pos, QTableWidget *pTableWidget, QList<RECWIDGET> *pListRecWidget, XFW_DEF::CWOPTIONS *pCwOptions);
    void contextMenuGenericTableWidget(const QPoint &pos, QTableView *pTableView, QList<RECWIDGET> *pListRecWidget, XFW_DEF::CWOPTIONS *pCwOptions);
    void tableView_doubleClicked(QTableView *pTableView, const QModelIndex &index);

signals:
    void closeApp();
    void showCwWidget(QString sInitString, bool nNewWindow);

public slots:
    void valueChangedSlot(QVariant varValue);
    void setEdited(qint64 nDeviceOffset, qint64 nDeviceSize);
    void allReload(qint64 nDeviceOffset, qint64 nDeviceSize);
    // void showHex(qint64 nOffset, qint64 nSize);
    void showInDisasmWindowAddress(XADDR nAddress);
    void showInDisasmWindowOffset(qint64 nOffset);
    void showInMemoryMapWindowOffset(qint64 nOffset);
    void showInHexWindow(qint64 nOffset, qint64 nSize);
    void showInHexWindow(qint64 nOffset);
    void widgetValueChanged(QVariant vValue);
    void findValue(quint64 nValue, XBinary::ENDIAN endian);
    void dumpRegion(qint64 nOffset, qint64 nSize, const QString &sName);
    void showDemangle(const QString &sString);
    void _reload();
    void showTableRecord();

protected:
    virtual void registerShortcuts(bool bState);

private:
    QIODevice *g_pDevice;
    QString g_sFileName;
    XFW_DEF::OPTIONS g_fwOptions;
    quint32 g_nNumber;
    qint64 g_nOffset;
    quint32 g_nType;
    QColor g_colEnabled;
    QColor g_colDisabled;
    XBinary::FT g_fileType;
    XBinary::MODE g_mode;
    XBinary::ENDIAN g_endian;
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

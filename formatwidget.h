/* Copyright (c) 2017-2022 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef FORMATWIDGET_H
#define FORMATWIDGET_H

#include <QHeaderView>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QTreeWidgetItem>
#include "dialogdemangle.h"
#include "dialogentropy.h"
#include "dialoghexview.h"
#include "dialogmodelinfo.h"
#include "dialogmultidisasm.h"
#include "dialogprocessdata.h"
#include "formatwidget_def.h"
#include "invwidget.h"
#include "toolswidget.h"
#include "xcomboboxex.h"
#include "xdatetimeeditx.h"
#include "xhexviewwidget.h"
#include "xlineedithex.h"
#include "xmultidisasmwidget.h"
#include "xshortcutsdialog.h"
#include "xoptions.h"
#include "xmemorymapwidget.h"

class FormatWidget : public XShortcutsWidget
{
    Q_OBJECT

public:
    enum HEADER_COLUMN
    {
        HEADER_COLUMN_NAME=0,
        HEADER_COLUMN_OFFSET,
        HEADER_COLUMN_TYPE,
        HEADER_COLUMN_VALUE,
        HEADER_COLUMN_INFO,
        HEADER_COLUMN_COMMENT
    };

    enum LIST_COLUMN
    {
        LIST_COLUMN_NAME=0,
        LIST_COLUMN_VALUE
    };

    enum HEADER_DATA
    {
        HEADER_DATA_OFFSET=0,
        HEADER_DATA_SIZE
    };

    enum DIRECTORY_COLUMN
    {
        DIRECTORY_COLUMN_NUMBER=0,
        DIRECTORY_COLUMN_NAME,
        DIRECTORY_COLUMN_ADDRESS,
        DIRECTORY_COLUMN_SIZE
    };

    FormatWidget(QWidget *pParent=nullptr);
    FormatWidget(QIODevice *pDevice,FW_DEF::OPTIONS options,quint32 nNumber,qint64 nOffset,qint32 nType,QWidget *pParent);
    ~FormatWidget();

    void setXInfoDB(XInfoDB *pXInfoDB);
    XInfoDB *getXInfoDB();

    void setGlobal(XShortcuts *pShortcuts,XOptions *pXOptions);
    void adjustView();

    void setData(QIODevice *pDevice,FW_DEF::OPTIONS options,quint32 nNumber,qint64 nOffset,qint32 nType);
    void setData(QString sFileName,FW_DEF::OPTIONS options,quint32 nNumber,qint64 nOffset,qint32 nType);
    void setData(FW_DEF::OPTIONS options,quint32 nNumber,qint64 nOffset,qint32 nType);
    void setBackupDevice(QIODevice *pDevice);
    QIODevice *getBackupDevice();

    void setFileType(XBinary::FT fileType);
    XBinary::FT getFileType();
    QIODevice *getDevice();
    virtual void setOptions(FW_DEF::OPTIONS options); // TODO for all Widgets
    FW_DEF::OPTIONS getOptions();
    quint32 getNumber();
    qint64 getOffset();
    qint32 getType();
    bool isReadonly();
    QTreeWidgetItem *createNewItem(int nType,QString sTitle,qint64 nOffset=0,qint64 nSize=0,qint64 nExtraOffset=0,qint64 nExtraSize=0);
    bool createHeaderTable(int nType,QTableWidget *pTableWidget,const FW_DEF::HEADER_RECORD *pRecords,XLineEditHEX **ppLineEdits,int nNumberOfRecords,int nPosition=0,qint64 nOffset=0);
    bool createListTable(int nType,QTableWidget *pTableWidget,const FW_DEF::HEADER_RECORD *pRecords,XLineEditHEX **ppLineEdits,int nNumberOfRecords);
    void addComment(QTableWidget *pTableWidget,int nRow,int nColumn,QString sComment);
    void updateTableRecord(QTableWidget *pTableWidget,int nRow,qint64 nOffset,qint64 nSize);

//    bool createDirectoryTable(int type,QTableWidget *pTableWidget,const DIRECTORY_ENTRY_RECORD *pRecords,int nRecordCount);
    bool createSectionTable(int nType,QTableWidget *pTableWidget,const FW_DEF::HEADER_RECORD *pRecords,int nNumberOfRecords);
    void setLineEditsReadOnly(XLineEditHEX **ppLineEdits,int nCount,bool bState);
    void setComboBoxesReadOnly(XComboBoxEx **ppComboBoxes,int nCount,bool bState);
    void setPushButtonReadOnly(QPushButton **ppPushButtons,int nCount,bool bState);
    void setDateTimeEditReadOnly(XDateTimeEditX **ppDateTimeEdits,int nCount,bool bState);

    void _deleteObjects(QObject **ppObjects,qint32 nCount);
    void _deleteSubdevices(SubDevice **ppSubdevices,qint32 nCount);

    void _blockSignals(QObject **ppObjects,int nCount,bool bState);
    XComboBoxEx *createComboBox(QTableWidget *pTableWidget,QMap<quint64,QString> mapData,int nType,int nData,XComboBoxEx::CBTYPE cbtype,quint64 nMask=0,int nExtraData=-1);
    InvWidget *createInvWidget(QTableWidget *pTableWidget,int nType,int nData,InvWidget::TYPE widgetType);
    XDateTimeEditX *createTimeDateEdit(QTableWidget *pTableWidget,int nType,int nData,XDateTimeEditX::DT_TYPE dtType);
    QPushButton *createPushButton(QTableWidget *pTableWidget,int nType,int nData,QString sText);

    void setValue(QVariant vValue,int nStype,int nNdata,int nVtype,int nPosition,qint64 nOffset);

    bool isEdited();

//    QPushButton *createHexButton(QTableWidget *pTableWidget,int nType,int nData);

    bool loadHexSubdevice(qint64 nOffset,qint64 nSize,XADDR nAddress,SubDevice **ppSubDevice,ToolsWidget *pToolsWidget,bool bOffset=false,bool bDisasm=true);
    bool loadHexSubdeviceByTableView(int nRow,int nType,ToolsWidget *pToolsWidget,QTableView *pTableView,SubDevice **ppSubDevice);
    bool setHexSubdeviceByTableView(int nRow,int nType,ToolsWidget *pToolsWidget,QTableView *pTableView);

    void setHeaderTableSelection(ToolsWidget *pToolWidget,QTableWidget *pTableWidget);

    QColor getEnabledColor();
    QColor getDisabledColor();

    void setItemEnable(QTableWidgetItem *pItem,bool bState);
    void setLineEdit(XLineEditHEX *pLineEdit,qint32 nMaxLength,QString sText,qint64 nOffset);
    void ajustTableView(ProcessData *pProcessData,QStandardItemModel **ppModel,QTableView *pTableView,QSortFilterProxyModel *pProxyModel=nullptr,bool bStretchLastSection=true);
    void ajustTreeView(ProcessData *pProcessData,QStandardItemModel **ppModel,QTreeView *pTreeView);
    void ajustDialogModel(ProcessData *pProcessData,QStandardItemModel **ppModel,QString sTitle);
    void showSectionHex(QTableView *pTableView);
    void showSectionEntropy(QTableView *pTableView);
    void showSectionDisasm(QTableView *pTableView);
    void dumpSection(QTableView *pTableView);
    qint64 getTableViewItemSize(QTableView *pTableView);

    void showTableViewDemangle(QTableView *pTableView,int nColumn);

    static bool _setTreeItem(QTreeWidget *pTree,QTreeWidgetItem *pItem,int nID);
    static void setTreeItem(QTreeWidget *pTree,int nID);
    virtual void clear()=0;
    virtual void cleanup()=0;
    virtual void reload()=0;

    void reset();
    QString getInitString(QTreeWidgetItem *pItem);
    void addInit(QString sString);
    bool isInitPresent(QString sString);

    void addPage(QTreeWidgetItem *pItem);
    void setAddPageEnabled(bool bEnable);
    QTreeWidgetItem *getPrevPage();
    QTreeWidgetItem *getNextPage();
    bool isPrevPageAvailable();
    bool isNextPageAvailable();

    void initWidget();
    void resetWidget();

    void initSearchStringsWidget(SearchStringsWidget *pWidget);
    void initSearchSignaturesWidget(SearchSignaturesWidget *pWidget);
    void initHexViewWidget(XHexViewWidget *pWidget);
    void initHexView(XHexView *pWidget);
    void initToolsWidget(ToolsWidget *pWidget);

    enum CW
    {
        CW_UINT8=0,
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

    static qint32 getColumnWidth(QWidget *pParent,CW cw,XBinary::MODE mode);

protected:
    enum SV
    {
        SV_NONE=0,
        SV_EDITED,
        SV_RELOAD,
        SV_RELOADDATA
    };

    virtual SV _setValue(QVariant vValue,int nStype,int nNdata,int nVtype,int nPosition,qint64 nOffset)=0;
    virtual void setReadonly(bool bState)=0;
    virtual void blockSignals(bool bState)=0;
    virtual void reloadData()=0;
    virtual void adjustHeaderTable(int nType,QTableWidget *pTableWidget);
    virtual void adjustListTable(int nType,QTableWidget *pTableWidget);
    virtual QString typeIdToString(int nType);
    virtual void _showInDisasmWindowAddress(XADDR nAddress);
    virtual void _showInDisasmWindowOffset(qint64 nOffset);
    virtual void _showInMemoryMapWindowOffset(qint64 nOffset);
    virtual void _showInHexWindow(qint64 nOffset,qint64 nSize);

signals:
    void changed();
    void closeApp();

public slots:
    void hexValueChanged(quint64 nValue);
    void textValueChanged(QString sText);
    void setEdited();
    void showHex(qint64 nOffset,qint64 nSize);
    void showInDisasmWindowAddress(XADDR nAddress);
    void showInDisasmWindowOffset(qint64 nOffset);
    void showInMemoryMapWindowOffset(qint64 nOffset);
    void showInHexWindow(qint64 nOffset,qint64 nSize);
    void showEntropy(qint64 nOffset,qint64 nSize);
    void dumpRegion(qint64 nOffset,qint64 nSize,QString sName);
    void showDemangle(QString sString);
    bool saveBackup();

protected:
    virtual void registerShortcuts(bool bState);

private:
    QIODevice *g_pDevice;
    QString g_sFileName;
    QIODevice *g_pBackupDevice;
    FW_DEF::OPTIONS g_fwOptions;
    quint32 g_nNumber;
    qint64 g_nOffset;
    quint32 g_nType;
    bool g_bIsReadonly;
    QColor g_colEnabled;
    QColor g_colDisabled;
    XBinary::FT g_fileType;
    QList<QTreeWidgetItem *> g_listPages;
    qint32 g_nPageIndex;
    bool g_bAddPageEnable;
    QMap<QString,QString> g_mapInit;
    XInfoDB *g_pXInfoDB;
};

#endif // FORMATWIDGET_H

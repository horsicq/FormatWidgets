// copyright (c) 2017-2020 hors<horsicq@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#ifndef FORMATWIDGET_H
#define FORMATWIDGET_H

#include <QFile>
#include <QWidget>
#include <QDialog>
#include <QTreeWidgetItem>
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QPushButton>
#include "xlineedithex.h"
#include "xcomboboxex.h"
#include "xdatetimeeditx.h"
#include "qhexview.h"
#include "invwidget.h"
#include "dialoghex.h"
#include "dialogentropy.h"
#include "toolswidget.h"
#include "formatwidget_def.h"
#include "dialogprocessdata.h"
#ifdef USE_DISASM
#include "xdisasmwidget.h"
#endif

class FormatWidget : public QWidget
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
    FormatWidget(QIODevice *pDevice, FW_DEF::OPTIONS *pOptions, quint32 nNumber, qint64 nOffset, qint32 nType, QWidget *pParent);
    ~FormatWidget();
    virtual void clear()=0;
    virtual void reset()=0;
    virtual void cleanup()=0;
    void setData(QIODevice *pDevice, FW_DEF::OPTIONS *pOptions, quint32 nNumber, qint64 nOffset, qint32 nType);
    virtual void reload()=0;
    QIODevice *getDevice();
    FW_DEF::OPTIONS *getOptions();
    quint32 getNumber();
    qint64 getOffset();
    qint32 getType();
    bool isReadonly();
    QTreeWidgetItem *createNewItem(int nType, QString sTitle, qint64 nOffset=0, qint64 nSize=0, qint64 nExtraOffset=0, qint64 nExtraSize=0);
    bool createHeaderTable(int nType,QTableWidget *pTableWidget, const FW_DEF::HEADER_RECORD *pRecords, XLineEditHEX **ppLineEdits, int nNumberOfRecords,int nPosition=0,qint64 nOffset=0);
    bool createListTable(int nType,QTableWidget *pTableWidget, const FW_DEF::HEADER_RECORD *pRecords, XLineEditHEX **ppLineEdits, int nNumberOfRecords);
    void addComment(QTableWidget *pTableWidget,int nRow,int nColumn,QString sComment);
    void updateTableRecord(QTableWidget *pTableWidget,int nRow,qint64 nOffset,qint64 nSize);

//    bool createDirectoryTable(int type,QTableWidget *pTableWidget, const DIRECTORY_ENTRY_RECORD *pRecords,int nRecordCount);
    bool createSectionTable(int nType,QTableWidget *pTableWidget, const FW_DEF::HEADER_RECORD *pRecords,int nNumberOfRecords);
    void setLineEditsReadOnly(XLineEditHEX **ppLineEdits,int nCount,bool bState);
    void setComboBoxesReadOnly(XComboBoxEx **ppComboBoxes,int nCount,bool bState);
    void setPushButtonReadOnly(QPushButton **ppPushButtons,int nCount,bool bState);
    void setDateTimeEditReadOnly(XDateTimeEditX **ppDateTimeEdits,int nCount,bool bState);

    void _deleteObjects(QObject **ppObjects,int nCount);

    void _blockSignals(QObject **ppObjects,int nCount,bool bState);
    XComboBoxEx *createComboBox(QTableWidget *pTableWidget, QMap<quint64,QString> mapData, int nType, int nData, XComboBoxEx::CBTYPE cbtype,quint64 nMask=0);
    InvWidget *createInvWidget(QTableWidget *pTableWidget, int nType, int nData,InvWidget::TYPE widgetType);
    XDateTimeEditX *createTimeDateEdit(QTableWidget *pTableWidget,int nType, int nData,XDateTimeEditX::DT_TYPE dtType);
    QPushButton *createPushButton(QTableWidget *pTableWidget,int nType, int nData,QString sText);

    void setValue(QVariant vValue, int nStype, int nNdata, int nVtype, int nPosition, qint64 nOffset);
    virtual bool _setValue(QVariant vValue,int nStype,int nNdata,int nVtype,int nPosition,qint64 nOffset)=0;
    virtual void setReadonly(bool bState)=0;
    virtual void blockSignals(bool bState)=0;
    virtual void adjustHeaderTable(int nType,QTableWidget *pTableWidget);
    virtual void adjustListTable(int nType,QTableWidget *pTableWidget);
    virtual QString typeIdToString(int nType);
    bool isEdited();

//    QPushButton *createHexButton(QTableWidget *pTableWidget,int nType, int nData);

    bool loadHexSubdevice(qint64 nOffset, qint64 nSize, qint64 nAddress, SubDevice **ppSubDevice, ToolsWidget *pToolsWidget);
    bool loadHexSubdeviceByTableView(int nRow, int nType, ToolsWidget *pToolsWidget, QTableView *pTableView, SubDevice **ppSubDevice);

    void setHeaderTableSelection(ToolsWidget *pToolWidget,QTableWidget *pTableWidget);

    QColor getEnabledColor();
    QColor getDisabledColor();

    void setItemEnable(QTableWidgetItem *pItem,bool bState);
    void setLineEdit(XLineEditHEX *pLineEdit,qint32 nMaxLength,QString sText,qint64 nOffset);
    void ajustTableView(ProcessData *pProcessData, QStandardItemModel **ppModel, QTableView *pTableView);
    void ajustTreeView(ProcessData *pProcessData, QStandardItemModel **ppModel, QTreeView *pTreeView);
    void showSectionHex(QTableView *pTableView);
    void showSectionEntropy(QTableView *pTableView);
    qint64 getTableViewItemSize(QTableView *pTableView,int nRow);

    static bool _setTreeItem(QTreeWidget *pTree,QTreeWidgetItem *pItem,int nID);
    static void setTreeItem(QTreeWidget *pTree,int nID);

signals:
    void editState(bool bState);

public slots:
    void hexValueChanged(quint64 nValue);
    void textValueChanged(QString sText);
    void setEdited(bool bState);
    void showHex(qint64 nOffset,qint64 nSize);
    void showEntropy(qint64 nOffset,qint64 nSize);

private slots:
    virtual void reloadData()=0;

private:
    bool saveBackup();

private:
    QIODevice *pDevice;
    FW_DEF::OPTIONS fwOptions;
    quint32 g_nNumber;
    qint64 __nOffset;
    quint32 __nType;
    bool bIsReadonly;
    bool bIsEdited;

    QColor colEnabled;
    QColor colDisabled;
};

#endif // FORMATWIDGET_H

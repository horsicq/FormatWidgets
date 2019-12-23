// copyright (c) 2017-2019 hors<horsicq@gmail.com>
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
#include "toolswidget.h"
#include "formatwidget_def.h"

class FormatWidget : public QWidget
{
    Q_OBJECT

public:
    enum VAL_TYPE
    {
        VAL_TYPE_UNKNOWN=0,
        VAL_TYPE_DATA,
        VAL_TYPE_FLAGS,
        VAL_TYPE_RELADDRESS,
        VAL_TYPE_ADDRESS,
        VAL_TYPE_OFFSET,
        VAL_TYPE_SIZE,
        VAL_TYPE_TEXT,
        VAL_TYPE_LABEL,
        VAL_TYPE_UNIXTIME // TODO
    };

    struct HEADER_RECORD
    {
        int nData;
        const char *pszName;
        int nOffset;
        int nSize;
        const char *pszType;
        VAL_TYPE vtype;
    };
//    struct DIRECTORY_ENTRY_RECORD
//    {
//        int nData;
//        const char *pszName;
//        int nSize[2];
//        const char *pszType[2];
//        VAL_TYPE vtype[2];
//    };

    enum HEADER_COLUMN
    {
        HEADER_COLUMN_NAME=0,
        HEADER_COLUMN_TYPE,
        HEADER_COLUMN_VALUE,
        HEADER_COLUMN_INFO
    };

    enum DIRECTORY_COLUMN
    {
        DIRECTORY_COLUMN_NUMBER=0,
        DIRECTORY_COLUMN_NAME,
        DIRECTORY_COLUMN_ADDRESS,
        DIRECTORY_COLUMN_SIZE
    };

    enum SECTION_DATA
    {
        SECTION_DATA_OFFSET=0,
        SECTION_DATA_SIZE,
        SECTION_DATA_ADDRESS,
//        SECTION_DATA_RELADDRESS,
        SECTION_DATA_VSIZE
    };

    FormatWidget(QWidget *parent=nullptr);
    FormatWidget(QIODevice *pDevice, FW_DEF::OPTIONS *pOptions, quint32 nNumber, QWidget *parent);
    ~FormatWidget();
    virtual void clear()=0;
    void setData(QIODevice *pDevice, FW_DEF::OPTIONS *pOptions, quint32 nNumber);
    virtual void reload()=0;
    QIODevice *getDevice();
    FW_DEF::OPTIONS *getOptions();
    quint32 getNumber();
    bool isReadonly();
    QTreeWidgetItem *createNewItem(int nUserData, QString sTitle);
    bool createHeaderTable(int type,QTableWidget *pTableWidget, const HEADER_RECORD *pRecords, XLineEditHEX **ppLineEdits, int nRecordCount,int nPosition=0);

//    bool createDirectoryTable(int type,QTableWidget *pTableWidget, const DIRECTORY_ENTRY_RECORD *pRecords,int nRecordCount);
    bool createSectionTable(int type,QTableWidget *pTableWidget, const HEADER_RECORD *pRecords,int nRecordCount);
    void setLineEditsReadOnly(XLineEditHEX **ppLineEdits,int nCount,bool bState);
    void setComboBoxesReadOnly(XComboBoxEx **ppComboBoxes,int nCount,bool bState);
    void setPushButtonReadOnly(QPushButton **ppPushButtons,int nCount,bool bState);
    void setDateTimeEditReadOnly(XDateTimeEditX **ppDateTimeEdits,int nCount,bool bState);

    void _blockSignals(QObject **ppObjects,int nCount,bool bState);
    XComboBoxEx *createComboBox(QTableWidget *pTableWidget, QMap<quint64,QString> mapData, int type, int nData, XComboBoxEx::CBTYPE cbtype,quint64 nMask=0);
    InvWidget *createInvWidget(QTableWidget *pTableWidget, int type, int nData,InvWidget::TYPE widgetType);
    XDateTimeEditX *createTimeDateEdit(QTableWidget *pTableWidget,int type, int nData,XDateTimeEditX::DT_TYPE dtType);
    QPushButton *createPushButton(QTableWidget *pTableWidget,int type, int nData,QString sText);
    int getSymbolWidth();
    void setValue(QVariant vValue, int nStype, int nNdata, int nVtype, int nPosition);
    virtual bool _setValue(QVariant vValue,int nStype,int nNdata,int nVtype,int nPosition)=0;
    virtual void setReadonly(bool bState)=0;
    virtual void blockSignals(bool bState)=0;
    virtual void adjustHeaderTable(int type,QTableWidget *pTableWidget);
    bool isEdited();

    QPushButton *createHexButton(QTableWidget *pTableWidget,int type, int nData);

    void loadHexSubdevice(qint64 nOffset, qint64 nSize, qint64 nAddress, SubDevice **ppSubDevice, ToolsWidget *pToolsWidget);

    void resizeToolsWidget(QWidget *pParent,ToolsWidget *pToolWidget);

signals:
    void editState(bool bState);

public slots:
    void hexValueChanged(quint64 nValue);
    void textValueChanged(QString sText);
    void setEdited(bool bState);

private slots:
    virtual void reloadData()=0;
    void showHex(qint64 nOffset,qint64 nSize);

private:
    bool saveBackup();

private:
    QIODevice *pDevice;
    FW_DEF::OPTIONS options;
    quint32 nNumber;
    bool bIsReadonly;
    bool bIsEdited;
};

#endif // FORMATWIDGET_H

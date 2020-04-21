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
#include "formatwidget.h"

FormatWidget::FormatWidget(QWidget *parent):
    QWidget(parent)
{
    bIsReadonly=false;
    fwOptions={};
    bIsEdited=false;

    colDisabled=QWidget::palette().color(QPalette::Window);
    colEnabled=QWidget::palette().color(QPalette::BrightText);
}

FormatWidget::FormatWidget(QIODevice *pDevice, FW_DEF::OPTIONS *pOptions, quint32 nNumber, qint64 nOffset, qint32 nType, QWidget *parent):
    QWidget(parent)
{
    fwOptions={};
    bIsEdited=false;
    setData(pDevice,pOptions,nNumber,nOffset,nType);
}

FormatWidget::~FormatWidget()
{

}

void FormatWidget::setData(QIODevice *pDevice, FW_DEF::OPTIONS *pOptions, quint32 nNumber, qint64 nOffset, qint32 nType)
{
    this->pDevice=pDevice;
    this->__nNumber=nNumber;
    this->__nOffset=nOffset;
    this->__nType=nType;

    if(pOptions)
    {
        fwOptions=*pOptions;
    }

    bIsReadonly=!(pDevice->isWritable());
}

QIODevice *FormatWidget::getDevice()
{
    return this->pDevice;
}

FW_DEF::OPTIONS *FormatWidget::getOptions()
{
    return &fwOptions;
}

quint32 FormatWidget::getNumber()
{
    return __nNumber;
}

qint64 FormatWidget::getOffset()
{
    return __nOffset;
}

qint32 FormatWidget::getType()
{
    return __nType;
}

bool FormatWidget::isReadonly()
{
    return bIsReadonly;
}

QTreeWidgetItem *FormatWidget::createNewItem(int nType, QString sTitle, qint64 nOffset, qint64 nSize, qint64 nExtraOffset, qint64 nExtraSize)
{
    QTreeWidgetItem *result=new QTreeWidgetItem;
    result->setText(0,sTitle);
    result->setData(0,Qt::UserRole+FW_DEF::SECTION_DATA_TYPE,nType);
    result->setData(0,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET,nOffset);
    result->setData(0,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE,nSize);
    result->setData(0,Qt::UserRole+FW_DEF::SECTION_DATA_EXTRAOFFSET,nExtraOffset);
    result->setData(0,Qt::UserRole+FW_DEF::SECTION_DATA_EXTRASIZE,nExtraSize);

    return result;
}

void FormatWidget::setValue(QVariant vValue, int nStype, int nNdata, int nVtype, int nPosition, qint64 nOffset)
{
    if(saveBackup())
    {
        if(_setValue(vValue,nStype,nNdata,nVtype,nPosition,nOffset))
        {
            setEdited(true);
        }
    }
    else
    {
        QMessageBox::critical(this,tr("Error"),tr("Cannot save file")+QString(": %1").arg(fwOptions.sBackupFileName));
    }
}

void FormatWidget::adjustHeaderTable(int type, QTableWidget *pTableWidget)
{
    Q_UNUSED(type)
    Q_UNUSED(pTableWidget)
}

void FormatWidget::adjustListTable(int type, QTableWidget *pTableWidget)
{
    Q_UNUSED(type)
    Q_UNUSED(pTableWidget)
}

bool FormatWidget::isEdited()
{
    return bIsEdited;
}

bool FormatWidget::loadHexSubdevice(qint64 nOffset, qint64 nSize, qint64 nAddress,SubDevice **ppSubDevice,ToolsWidget *pToolsWidget)
{
    if(*ppSubDevice)
    {
        (*ppSubDevice)->close();
        delete (*ppSubDevice);
    }

    (*ppSubDevice)=new SubDevice(getDevice(),nOffset,nSize,this);

    (*ppSubDevice)->open(getDevice()->openMode());

    FW_DEF::OPTIONS hexOptions=*getOptions();
    hexOptions.nImageBase=nAddress;

    pToolsWidget->setData((*ppSubDevice),&hexOptions);
    pToolsWidget->setEdited(isEdited());
    connect(pToolsWidget,SIGNAL(editState(bool)),this,SLOT(setEdited(bool)));

    return true;
}

void FormatWidget::setHeaderTableSelection(ToolsWidget *pToolWidget, QTableWidget *pTableWidget)
{
    qint32 nCurrentRow=pTableWidget->currentRow();

    if(nCurrentRow!=-1)
    {
        qint32 nOffset=pTableWidget->item(nCurrentRow,0)->data(Qt::UserRole+HEADER_DATA_OFFSET).toInt();
        qint64 nSize=pTableWidget->item(nCurrentRow,0)->data(Qt::UserRole+HEADER_DATA_SIZE).toInt();

        if(nOffset!=-1)
        {
            qint64 nAddress=pToolWidget->getBaseAddress()+nOffset;

            pToolWidget->setSelection(nAddress,nSize);
        }
    }
}

QColor FormatWidget::getEnabledColor()
{
    return colEnabled;
}

QColor FormatWidget::getDisabledColor()
{
    return colDisabled;
}

void FormatWidget::setItemEnable(QTableWidgetItem *pItem, bool bState)
{
    pItem->setBackgroundColor(bState?(colEnabled):(colDisabled));
}

void FormatWidget::setLineEdit(XLineEditHEX *pLineEdit, qint32 nMaxLength, QString sText, qint64 nOffset)
{
    pLineEdit->setMaxLength(nMaxLength);
    pLineEdit->setText(sText);
    pLineEdit->setProperty("OFFSET",nOffset);
}

void FormatWidget::ajustTableView(ProcessData *pProcessData, QStandardItemModel **ppModel, QTableView *pTableView)
{
    QAbstractItemModel *pOldModel=pTableView->model();

    DialogProcessData dialogProcessData(this,pProcessData);

    dialogProcessData.exec();

    pTableView->setModel(*ppModel);

    pProcessData->ajustTableView(this,pTableView);

    delete pOldModel; // TODO Thread
}

void FormatWidget::showSectionHex(QTableView *pTableView)
{
    int nRow=pTableView->currentIndex().row();

    if(nRow!=-1)
    {
        QModelIndex index=pTableView->selectionModel()->selectedIndexes().at(0);

        qint64 nOffset=pTableView->model()->data(index,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET).toLongLong();
        qint64 nSize=pTableView->model()->data(index,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE).toLongLong();

        showHex(nOffset,nSize);

        reloadData();

        pTableView->setCurrentIndex(index);
    }
}

qint64 FormatWidget::getTableViewItemSize(QTableView *pTableView, int nRow)
{
    qint64 nResult=0;

    if(nRow!=-1)
    {
        QModelIndex index=pTableView->selectionModel()->selectedIndexes().at(0);
        nResult=pTableView->model()->data(index,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE).toLongLong();
    }

    return nResult;
}

//void FormatWidget::resizeToolsWidget(QWidget *pParent, ToolsWidget *pToolWidget)
//{
//    qint32 nHeight=pParent->height();
//    qint32 _nMaxHeight=pToolWidget->maximumHeight();
//    pToolWidget->setMaximumHeight(nHeight/4);
//    pToolWidget->setMaximumHeight(_nMaxHeight);
//}

void FormatWidget::hexValueChanged(quint64 nValue)
{
    XLineEditHEX *lineEdit=qobject_cast<XLineEditHEX *>(sender());

    int nStype=lineEdit->property("STYPE").toInt();
    int nNdata=lineEdit->property("NDATA").toInt();
    int nVtype=lineEdit->property("VTYPE").toInt();
    int nPosition=lineEdit->property("POSITION").toInt();
    qint64 nOffset=lineEdit->property("OFFSET").toLongLong();

    setValue(nValue,nStype,nNdata,nVtype,nPosition,nOffset);
}

void FormatWidget::textValueChanged(QString sText)
{
    XLineEditHEX *lineEdit=qobject_cast<XLineEditHEX *>(sender());

    int nStype=lineEdit->property("STYPE").toInt();
    int nNdata=lineEdit->property("NDATA").toInt();
    int nVtype=lineEdit->property("VTYPE").toInt();
    int nPosition=lineEdit->property("POSITION").toInt();
    qint64 nOffset=lineEdit->property("OFFSET").toLongLong();

    setValue(sText,nStype,nNdata,nVtype,nPosition,nOffset);
}

void FormatWidget::setEdited(bool bState)
{
    bIsEdited=bState;

    reset();

    emit editState(bState);
}

void FormatWidget::showHex(qint64 nOffset, qint64 nSize)
{
    // mb TODO StartAddress
    QHexView::OPTIONS hexOptions={};

    XBinary binary(pDevice,true,fwOptions.nImageBase);

    hexOptions.memoryMap=binary.getMemoryMap();
    hexOptions.sBackupFileName=fwOptions.sBackupFileName;
    hexOptions.nStartAddress=nOffset;
    hexOptions.nStartSelectionAddress=nOffset;
    hexOptions.nSizeOfSelection=nSize;

    DialogHex dialogHex(this,pDevice,&hexOptions);

    dialogHex.exec();

    // TODO Connect edited !!!

    reloadData();
}

bool FormatWidget::saveBackup()
{
    bool bResult=true;
    // TODO Check
    if(!bIsEdited)
    {
        // Save backup
        if(fwOptions.sBackupFileName!="")
        {
            if(!QFile::exists(fwOptions.sBackupFileName))
            {
                if(pDevice->metaObject()->className()==QString("QFile"))
                {
                    QString sFileName=((QFile *)pDevice)->fileName();

                    bResult=QFile::copy(sFileName,fwOptions.sBackupFileName);
                }
            }
        }
    }

    return bResult;
}

bool FormatWidget::createHeaderTable(int type, QTableWidget *pTableWidget, const FW_DEF::HEADER_RECORD *pRecords, XLineEditHEX **ppLineEdits, int nRecordCount, int nPosition, qint64 nOffset)
{
    pTableWidget->setColumnCount(6);
    pTableWidget->setRowCount(nRecordCount);

    QStringList slHeader;
    slHeader.append(tr("Name"));
    slHeader.append(tr("Offset"));
    slHeader.append(tr("Type"));
    slHeader.append(tr("Value"));
    slHeader.append(tr(""));
    slHeader.append(tr(""));

    pTableWidget->setHorizontalHeaderLabels(slHeader);
    pTableWidget->horizontalHeader()->setVisible(true);

    for(int i=0; i<nRecordCount; i++)
    {
        QTableWidgetItem *newItemName=new QTableWidgetItem;
        newItemName->setText(pRecords[i].pszName);
        newItemName->setData(Qt::UserRole+HEADER_DATA_OFFSET,pRecords[i].nOffset);
        newItemName->setData(Qt::UserRole+HEADER_DATA_SIZE,pRecords[i].nSize);
        pTableWidget->setItem(i,HEADER_COLUMN_NAME,newItemName);

        QTableWidgetItem *newItemOffset=new QTableWidgetItem;

        if(pRecords[i].nOffset!=-1)
        {
            newItemOffset->setText(XBinary::valueToHex((quint16)pRecords[i].nOffset));
        }

        pTableWidget->setItem(i,HEADER_COLUMN_OFFSET,newItemOffset);

        QTableWidgetItem *newItemType=new QTableWidgetItem;
        newItemType->setText(pRecords[i].pszType);
        pTableWidget->setItem(i,HEADER_COLUMN_TYPE,newItemType);

        ppLineEdits[i]=new XLineEditHEX(this);
        ppLineEdits[i]->setProperty("STYPE",type);
        ppLineEdits[i]->setProperty("NDATA",pRecords[i].nData);
        ppLineEdits[i]->setProperty("POSITION",nPosition);
        ppLineEdits[i]->setProperty("OFFSET",nOffset);

        if(pRecords[i].vtype==FW_DEF::VAL_TYPE_TEXT)
        {
            connect(ppLineEdits[i],SIGNAL(textChanged(QString)),this,SLOT(textValueChanged(QString)));
        }
        else
        {
            connect(ppLineEdits[i],SIGNAL(valueChanged(quint64)),this,SLOT(hexValueChanged(quint64)));
        }

        pTableWidget->setCellWidget(i,HEADER_COLUMN_VALUE,ppLineEdits[i]);

        if(pRecords[i].nSize==0)
        {
            ppLineEdits[i]->setEnabled(false);
        }

        pTableWidget->setItem(i,HEADER_COLUMN_COMMENT,new QTableWidgetItem);
    }

    pTableWidget->horizontalHeader()->setSectionResizeMode(HEADER_COLUMN_COMMENT,QHeaderView::Stretch);

    adjustHeaderTable(type,pTableWidget);

    return true;
}

bool FormatWidget::createListTable(int type, QTableWidget *pTableWidget, const FW_DEF::HEADER_RECORD *pRecords, XLineEditHEX **ppLineEdits, int nRecordCount)
{
    pTableWidget->setColumnCount(2);
    pTableWidget->setRowCount(nRecordCount);

    QStringList slHeader;
    slHeader.append(tr("Name"));
    slHeader.append(tr("Value"));

    pTableWidget->setHorizontalHeaderLabels(slHeader);
    pTableWidget->horizontalHeader()->setVisible(true);

    for(int i=0; i<nRecordCount; i++)
    {
        QTableWidgetItem *newItemName=new QTableWidgetItem;
        newItemName->setText(pRecords[i].pszName);
        pTableWidget->setItem(i,LIST_COLUMN_NAME,newItemName);

        ppLineEdits[i]=new XLineEditHEX(this);

        ppLineEdits[i]->setProperty("STYPE",type);
        ppLineEdits[i]->setProperty("NDATA",pRecords[i].nData);

        if(pRecords[i].vtype==FW_DEF::VAL_TYPE_TEXT)
        {
            ppLineEdits[i]->setAlignment(Qt::AlignLeft);
            connect(ppLineEdits[i],SIGNAL(textChanged(QString)),this,SLOT(textValueChanged(QString)));
        }
        else
        {
            connect(ppLineEdits[i],SIGNAL(valueChanged(quint64)),this,SLOT(hexValueChanged(quint64)));
        }

        pTableWidget->setCellWidget(i,LIST_COLUMN_VALUE,ppLineEdits[i]);
    }

    pTableWidget->horizontalHeader()->setSectionResizeMode(LIST_COLUMN_VALUE,QHeaderView::Stretch);

    adjustListTable(type,pTableWidget);

    return true;
}

void FormatWidget::addComment(QTableWidget *pTableWidget, int nRow, int nColumn, QString sComment)
{
    pTableWidget->item(nRow,nColumn)->setText(sComment);
}

//bool FormatWidget::createDirectoryTable(int type, QTableWidget *pTableWidget, const DIRECTORY_ENTRY_RECORD *pRecords, int nRecordCount)
//{
//    Q_UNUSED(type)

//    pTableWidget->setColumnCount(4);
//    pTableWidget->setRowCount(nRecordCount);

//    int nSymbolWidth=getSymbolWidth();

//    pTableWidget->setColumnWidth(0,nSymbolWidth*3);
//    pTableWidget->setColumnWidth(1,nSymbolWidth*12);
//    pTableWidget->setColumnWidth(2,nSymbolWidth*8);
//    pTableWidget->setColumnWidth(3,nSymbolWidth*8);

//    QStringList slHeader;
//    slHeader.append(tr(""));
//    slHeader.append(tr("Name"));
//    slHeader.append(tr("Address"));
//    slHeader.append(tr("Size"));

//    pTableWidget->setHorizontalHeaderLabels(slHeader);
//    pTableWidget->horizontalHeader()->setVisible(true);

//    for(int i=0; i<nRecordCount; i++)
//    {
//        QTableWidgetItem *newItemNumber=new QTableWidgetItem;
//        newItemNumber->setText(QString("%1").arg(i));
//        pTableWidget->setItem(i,DIRECTORY_COLUMN_NUMBER,newItemNumber);

//        QTableWidgetItem *newItemName=new QTableWidgetItem;
//        newItemName->setText(pRecords[i].pszName);
//        pTableWidget->setItem(i,DIRECTORY_COLUMN_NAME,newItemName);

////        ppLineEdits1[i]=new XLineEditHEX(this);
////        ppLineEdits1[i]->setProperty("STYPE",type);
////        ppLineEdits1[i]->setProperty("NDATA",pRecords[i].nData);
////        ppLineEdits1[i]->setProperty("VTYPE",pRecords[i].vtype[0]);

////        connect(ppLineEdits1[i],SIGNAL(valueChanged(quint64)),this,SLOT(hexValueChanged(quint64)));

////        pTableWidget->setCellWidget(i,DIRECTORY_COLUMN_ADDRESS,ppLineEdits1[i]);

////        ppLineEdits2[i]=new XLineEditHEX(this);
////        ppLineEdits2[i]->setProperty("STYPE",type);
////        ppLineEdits2[i]->setProperty("NDATA",pRecords[i].nData);
////        ppLineEdits2[i]->setProperty("VTYPE",pRecords[i].vtype[1]);

////        connect(ppLineEdits2[i],SIGNAL(valueChanged(quint64)),this,SLOT(hexValueChanged(quint64)));

////        pTableWidget->setCellWidget(i,DIRECTORY_COLUMN_SIZE,ppLineEdits2[i]);
//    }

//    return true;
//}

bool FormatWidget::createSectionTable(int type, QTableWidget *pTableWidget, const FW_DEF::HEADER_RECORD *pRecords, int nRecordCount)
{
    Q_UNUSED(type)

    QStringList slHeader;

    pTableWidget->setRowCount(0);

    for(int i=0; i<nRecordCount; i++)
    {
        slHeader.append(pRecords[i].pszName);
    }

    pTableWidget->setColumnCount(nRecordCount);
    pTableWidget->setHorizontalHeaderLabels(slHeader);
    pTableWidget->horizontalHeader()->setVisible(true);

    return true;
}

void FormatWidget::setLineEditsReadOnly(XLineEditHEX **ppLineEdits, int nCount, bool bState)
{
    for(int i=0; i<nCount; i++)
    {
        if(ppLineEdits[i])
        {
            ppLineEdits[i]->setReadOnly(bState);
        }
    }
}

void FormatWidget::setComboBoxesReadOnly(XComboBoxEx **ppComboBoxes, int nCount, bool bState)
{
    for(int i=0; i<nCount; i++)
    {
        if(ppComboBoxes[i])
        {
            ppComboBoxes[i]->setReadOnly(bState);
        }
    }
}

void FormatWidget::setPushButtonReadOnly(QPushButton **ppPushButtons, int nCount, bool bState)
{
    for(int i=0; i<nCount; i++)
    {
        if(ppPushButtons[i])
        {
            ppPushButtons[i]->setEnabled(!bState);
        }
    }
}

void FormatWidget::setDateTimeEditReadOnly(XDateTimeEditX **ppDateTimeEdits, int nCount, bool bState)
{
    for(int i=0; i<nCount; i++)
    {
        if(ppDateTimeEdits[i])
        {
            ppDateTimeEdits[i]->setReadOnly(bState);
        }
    }
}

void FormatWidget::_deleteObjects(QObject **ppObjects, int nCount)
{
    nCount=nCount/(sizeof (QObject *));

    for(int i=0; i<nCount; i++)
    {
        if(ppObjects[i])
        {
            delete (ppObjects[i]);
            ppObjects[i]=0;
        }
    }
}

void FormatWidget::_blockSignals(QObject **ppObjects, int nCount, bool bState)
{
    for(int i=0; i<nCount; i++)
    {
        if(ppObjects[i])
        {
            ppObjects[i]->blockSignals(bState);
        }
    }
}

XComboBoxEx *FormatWidget::createComboBox(QTableWidget *pTableWidget, QMap<quint64, QString> mapData, int type, int nData, XComboBoxEx::CBTYPE cbtype, quint64 nMask)
{
    XComboBoxEx *result=new XComboBoxEx(this);
    result->setData(mapData,cbtype,nMask);

    result->setProperty("STYPE",type);
    result->setProperty("NDATA",nData);

    connect(result,SIGNAL(valueChanged(quint64)),this,SLOT(widgetValueChanged(quint64)));

    pTableWidget->setCellWidget(nData,HEADER_COLUMN_INFO,result);

    return result;
}

InvWidget *FormatWidget::createInvWidget(QTableWidget *pTableWidget, int type, int nData, InvWidget::TYPE widgetType)
{
    InvWidget *result=new InvWidget(this,widgetType);

    result->setProperty("STYPE",type);
    result->setProperty("NDATA",nData);

    connect(result,SIGNAL(showHex(qint64,qint64)),this,SLOT(showHex(qint64,qint64)));

    pTableWidget->setCellWidget(nData,HEADER_COLUMN_INFO,result);

    return result;
}

XDateTimeEditX *FormatWidget::createTimeDateEdit(QTableWidget *pTableWidget, int type, int nData, XDateTimeEditX::DT_TYPE dtType)
{
    XDateTimeEditX *result=new XDateTimeEditX(this);
    result->setType(dtType);

    result->setProperty("STYPE",type);
    result->setProperty("NDATA",nData);

    connect(result,SIGNAL(valueChanged(quint64)),this,SLOT(widgetValueChanged(quint64)));

    pTableWidget->setCellWidget(nData,HEADER_COLUMN_INFO,result);

    return result;
}

QPushButton *FormatWidget::createPushButton(QTableWidget *pTableWidget, int type, int nData, QString sText)
{
    QPushButton *result=new QPushButton(this);

    result->setText(sText);
    result->setProperty("STYPE",type);
    result->setProperty("NDATA",nData);

    connect(result,SIGNAL(clicked()),this,SLOT(widgetAction()));

    pTableWidget->setCellWidget(nData,HEADER_COLUMN_INFO,result);

    return result;
}

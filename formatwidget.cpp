// copyright (c) 2017-2021 hors<horsicq@gmail.com>
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

FormatWidget::FormatWidget(QWidget *pParent):
    XShortcutsWidget(pParent)
{
    g_pDevice=nullptr;
    g_pBackupDevice=nullptr;
    g_bIsReadonly=false;
    g_fwOptions={};
    g_bAddPageEnable=true;
    g_nPageIndex=0;

    g_colDisabled=QWidget::palette().color(QPalette::Window);
    g_colEnabled=QWidget::palette().color(QPalette::BrightText);
}

FormatWidget::FormatWidget(QIODevice *pDevice, FW_DEF::OPTIONS options, quint32 nNumber, qint64 nOffset, qint32 nType, QWidget *pParent):
    FormatWidget(pParent)
{
    FormatWidget::setData(pDevice,options,nNumber,nOffset,nType);
}

FormatWidget::~FormatWidget()
{

}

void FormatWidget::setData(QIODevice *pDevice, FW_DEF::OPTIONS options, quint32 nNumber, qint64 nOffset, qint32 nType)
{
    g_pDevice=pDevice;
    g_nNumber=nNumber;
    g_nOffset=nOffset;
    g_nType=nType;

    g_bIsReadonly=!(pDevice->isWritable());

    g_listPages.clear();
    g_nPageIndex=0;
    g_bAddPageEnable=true;

    setOptions(options);
}

void FormatWidget::setBackupDevice(QIODevice *pDevice)
{
    g_pBackupDevice=pDevice;
}

void FormatWidget::setFileType(XBinary::FT fileType)
{
    g_fileType=fileType;
}

XBinary::FT FormatWidget::getFileType()
{
    return g_fileType;
}

QIODevice *FormatWidget::getDevice()
{
    return this->g_pDevice;
}

void FormatWidget::setOptions(FW_DEF::OPTIONS options)
{
    g_fwOptions=options;
}

FW_DEF::OPTIONS FormatWidget::getOptions()
{
    return g_fwOptions;
}

quint32 FormatWidget::getNumber()
{
    return g_nNumber;
}

qint64 FormatWidget::getOffset()
{
    return g_nOffset;
}

qint32 FormatWidget::getType()
{
    return g_nType;
}

bool FormatWidget::isReadonly()
{
    return g_bIsReadonly;
}

QTreeWidgetItem *FormatWidget::createNewItem(int nType, QString sTitle, qint64 nOffset, qint64 nSize, qint64 nExtraOffset, qint64 nExtraSize)
{
    QTreeWidgetItem *pResult=new QTreeWidgetItem;

    pResult->setText(0,sTitle);
    pResult->setData(0,Qt::UserRole+FW_DEF::SECTION_DATA_TYPE,nType);
    pResult->setData(0,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET,nOffset);
    pResult->setData(0,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE,nSize);
    pResult->setData(0,Qt::UserRole+FW_DEF::SECTION_DATA_EXTRAOFFSET,nExtraOffset);
    pResult->setData(0,Qt::UserRole+FW_DEF::SECTION_DATA_EXTRASIZE,nExtraSize);

    return pResult;
}

void FormatWidget::setValue(QVariant vValue, int nStype, int nNdata, int nVtype, int nPosition, qint64 nOffset)
{
    if(saveBackup())
    {
        SV sv=_setValue(vValue,nStype,nNdata,nVtype,nPosition,nOffset);
        if(sv==SV_EDITED)
        {
            setEdited(true);
        }
        else if(sv==SV_RELOAD)
        {
            setEdited(true);
            reload();
            reloadData();
        }
        else if(sv==SV_RELOADDATA)
        {
            setEdited(true);
            reloadData();
        }
    }
    else
    {
        QMessageBox::critical(this,tr("Error"),tr("Cannot save file")+QString(": %1").arg(XBinary::getBackupName(getDevice())));
    }
}

void FormatWidget::adjustHeaderTable(int nType, QTableWidget *pTableWidget)
{
    Q_UNUSED(nType)
    Q_UNUSED(pTableWidget)
}

void FormatWidget::adjustListTable(int nType, QTableWidget *pTableWidget)
{
    Q_UNUSED(nType)
    Q_UNUSED(pTableWidget)
}

QString FormatWidget::typeIdToString(int nType)
{
    Q_UNUSED(nType)

    return "";
}

bool FormatWidget::isEdited()
{
    bool bResult=false;

    if(g_pBackupDevice)
    {
        bResult=XBinary::isBackupPresent(g_pBackupDevice);
    }
    else
    {
        bResult=XBinary::isBackupPresent(getDevice());
    }

    return bResult;
}

bool FormatWidget::loadHexSubdevice(qint64 nOffset, qint64 nSize, qint64 nAddress,SubDevice **ppSubDevice,ToolsWidget *pToolsWidget)
{
    if(*ppSubDevice)
    {
        (*ppSubDevice)->close();
        delete (*ppSubDevice);

        (*ppSubDevice)=0;
    }

    if((nOffset==-1)||(nAddress==-1)||(nSize==0))
    {
        nOffset=0;
        nAddress=0;
        nSize=0;
    }

    (*ppSubDevice)=new SubDevice(getDevice(),nOffset,nSize,this);

    (*ppSubDevice)->open(getDevice()->openMode());

    FW_DEF::OPTIONS hexOptions=getOptions();
    hexOptions.nImageBase=nAddress;

    pToolsWidget->setData((*ppSubDevice),hexOptions);
    pToolsWidget->setEdited(isEdited());

    return true;
}

bool FormatWidget::loadHexSubdeviceByTableView(int nRow, int nType, ToolsWidget *pToolsWidget, QTableView *pTableView, SubDevice **ppSubDevice)
{
    Q_UNUSED(nType)

    bool bResult=false;

    if(nRow!=-1)
    {
        QModelIndex index=pTableView->model()->index(nRow,0);

        qint64 nOffset=pTableView->model()->data(index,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET).toLongLong();
        qint64 nSize=pTableView->model()->data(index,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE).toLongLong();
        qint64 nAddress=pTableView->model()->data(index,Qt::UserRole+FW_DEF::SECTION_DATA_ADDRESS).toLongLong();

        bResult=loadHexSubdevice(nOffset,nSize,nAddress,ppSubDevice,pToolsWidget);
    }

    return bResult;
}

bool FormatWidget::setHexSubdeviceByTableView(int nRow, int nType, ToolsWidget *pToolsWidget, QTableView *pTableView)
{
    Q_UNUSED(nType)

    bool bResult=false;

    if(nRow!=-1)
    {
        QModelIndex index=pTableView->model()->index(nRow,0);

        qint64 nOffset=pTableView->model()->data(index,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET).toLongLong();
        qint64 nSize=pTableView->model()->data(index,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE).toLongLong();

        pToolsWidget->setSelection(nOffset,nSize);
        bResult=true;
    }

    return bResult;
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
//            qint64 nAddress=pToolWidget->getStartAddress()+nOffset;

            pToolWidget->setSelection(nOffset,nSize);
        }
    }
}

QColor FormatWidget::getEnabledColor()
{
    return g_colEnabled;
}

QColor FormatWidget::getDisabledColor()
{
    return g_colDisabled;
}

void FormatWidget::setItemEnable(QTableWidgetItem *pItem, bool bState)
{
    if(!bState)
    {
        pItem->setBackground(g_colDisabled);
    }
}

void FormatWidget::setLineEdit(XLineEditHEX *pLineEdit, qint32 nMaxLength, QString sText, qint64 nOffset)
{
    pLineEdit->setMaxLength(nMaxLength);
    pLineEdit->setText(sText);
    pLineEdit->setProperty("OFFSET",nOffset);
}

void FormatWidget::ajustTableView(ProcessData *pProcessData, QStandardItemModel **ppModel, QTableView *pTableView, QSortFilterProxyModel *pProxyModel, bool bStretchLastSection)
{
    QAbstractItemModel *pOldModel=0;

    if(pProxyModel)
    {
        pOldModel=pProxyModel->sourceModel();
        pProxyModel->setSourceModel(0);
    }
    else
    {
        pOldModel=pTableView->model();
    }

    DialogProcessData dialogProcessData(this,pProcessData);

    dialogProcessData.exec();

    bool bSort=pTableView->isSortingEnabled();

    if(bSort)
    {
        pTableView->setSortingEnabled(false);
    }

    if(pProxyModel)
    {
        pProxyModel->setSourceModel(*ppModel);
        pTableView->setModel(pProxyModel);
    }
    else
    {
        pTableView->setModel(*ppModel);
    }

    pProcessData->ajustTableView(this,pTableView);

    if(bSort)
    {
        pTableView->setSortingEnabled(true);
        pTableView->sortByColumn(0,Qt::AscendingOrder);
    }

    pTableView->horizontalHeader()->setStretchLastSection(bStretchLastSection);

    delete pOldModel; // TODO Thread
}

void FormatWidget::ajustTreeView(ProcessData *pProcessData, QStandardItemModel **ppModel, QTreeView *pTreeView)
{
    QAbstractItemModel *pOldModel=pTreeView->model();

    DialogProcessData dialogProcessData(this,pProcessData);

    dialogProcessData.exec();

    pTreeView->setModel(*ppModel);

    pProcessData->ajustTreeView(this,pTreeView);

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

        showInHexWindow(nOffset,nSize);

        reloadData();

        pTableView->setCurrentIndex(pTableView->model()->index(nRow,0));
    }
}

void FormatWidget::showSectionEntropy(QTableView *pTableView)
{
    int nRow=pTableView->currentIndex().row();

    if(nRow!=-1)
    {
        QModelIndex index=pTableView->selectionModel()->selectedIndexes().at(0);

        qint64 nOffset=pTableView->model()->data(index,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET).toLongLong();
        qint64 nSize=pTableView->model()->data(index,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE).toLongLong();

        showEntropy(nOffset,nSize);
    }
}

void FormatWidget::showSectionDisasm(QTableView *pTableView)
{
    int nRow=pTableView->currentIndex().row();

    if(nRow!=-1)
    {
        QModelIndex index=pTableView->selectionModel()->selectedIndexes().at(0);

        qint64 nAddress=pTableView->model()->data(index,Qt::UserRole+FW_DEF::SECTION_DATA_ADDRESS).toLongLong();

        showInDisasmWindowAddress(nAddress);
    }
}

qint64 FormatWidget::getTableViewItemSize(QTableView *pTableView)
{
    qint64 nResult=0;

    int nRow=pTableView->currentIndex().row();

    if(nRow!=-1)
    {
        QModelIndex index=pTableView->selectionModel()->selectedIndexes().at(0);
        nResult=pTableView->model()->data(index,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE).toLongLong();
    }

    return nResult;
}

void FormatWidget::showTableViewDemangle(QTableView *pTableView, int nColumn)
{
    int nRow=pTableView->currentIndex().row();

    if(nRow!=-1)
    {
        QModelIndex index=pTableView->selectionModel()->selectedIndexes().at(nColumn);
        QString sString=pTableView->model()->data(index).toString();

        showDemangle(sString);
    }
}

bool FormatWidget::_setTreeItem(QTreeWidget *pTree, QTreeWidgetItem *pItem, int nID)
{
    bool bResult=false;

    if(pItem->data(0,Qt::UserRole).toInt()==nID)
    {
        pTree->setCurrentItem(pItem);

        bResult=true;
    }
    else
    {
        int nNumberOfChildren=pItem->childCount();

        for(int i=0;i<nNumberOfChildren;i++)
        {
            if(_setTreeItem(pTree,pItem->child(i),nID))
            {
                bResult=true;
                break;
            }
        }
    }

    return bResult;
}

void FormatWidget::setTreeItem(QTreeWidget *pTree, int nID)
{
    int nNumberOfItems=pTree->topLevelItemCount();

    for(int i=0;i<nNumberOfItems;i++)
    {
        if(_setTreeItem(pTree,pTree->topLevelItem(i),nID))
        {
            break;
        }
    }
}

void FormatWidget::reset()
{
    g_stInit.clear();
}

QString FormatWidget::getInitString(QTreeWidgetItem *pItem)
{
    QString sResult;

    int nType=pItem->data(0,Qt::UserRole+FW_DEF::SECTION_DATA_TYPE).toInt();
    qint64 nDataOffset=pItem->data(0,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET).toLongLong();
    qint64 nDataSize=pItem->data(0,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE).toLongLong();

    sResult=QString("%1-%2-%3").arg(nType).arg(nDataOffset).arg(nDataSize);

    return sResult;
}

void FormatWidget::addInit(QString sString)
{
    g_stInit.insert(sString);
}

bool FormatWidget::isInitPresent(QString sString)
{
    return g_stInit.contains(sString);
}

void FormatWidget::addPage(QTreeWidgetItem *pItem)
{
    if(g_bAddPageEnable)
    {
        qint32 nNumberOfPages=g_listPages.count();

        for(int i=nNumberOfPages-1;i>g_nPageIndex;i--)
        {
            g_listPages.removeAt(i);
        }

        g_listPages.append(pItem);
        g_nPageIndex=g_listPages.count()-1;
    }
}

void FormatWidget::setAddPageEnabled(bool bEnable)
{
    g_bAddPageEnable=bEnable;
}

QTreeWidgetItem *FormatWidget::getPrevPage()
{
    QTreeWidgetItem *pResult=0;

    if(isPrevPageAvailable())
    {
        g_nPageIndex--;
        pResult=g_listPages.at(g_nPageIndex);
    }

    return pResult;
}

QTreeWidgetItem *FormatWidget::getNextPage()
{
    QTreeWidgetItem *pResult=0;

    if(isNextPageAvailable())
    {
        g_nPageIndex++;
        pResult=g_listPages.at(g_nPageIndex);
    }

    return pResult;
}

bool FormatWidget::isPrevPageAvailable()
{
    return g_nPageIndex>0;
}

bool FormatWidget::isNextPageAvailable()
{
    return g_nPageIndex<(g_listPages.count()-1);
}

void FormatWidget::initSearchStringsWidget(SearchStringsWidget *pWidget)
{
    connect(pWidget,SIGNAL(showHex(qint64,qint64)),this,SLOT(showInHexWindow(qint64,qint64)));
    connect(pWidget,SIGNAL(showDemangle(QString)),this,SLOT(showDemangle(QString)));
}

void FormatWidget::initSearchSignaturesWidget(SearchSignaturesWidget *pWidget)
{
    connect(pWidget,SIGNAL(showHex(qint64,qint64)),this,SLOT(showInHexWindow(qint64,qint64)));
}

void FormatWidget::initHexViewWidget(XHexViewWidget *pWidget)
{
    connect(pWidget,SIGNAL(editState(bool)),this,SLOT(setEdited(bool)));
    connect(pWidget,SIGNAL(showOffsetDisasm(qint64)),this,SLOT(showInDisasmWindowOffset(qint64)));
    connect(pWidget,SIGNAL(showOffsetMemoryMap(qint64)),this,SLOT(showInMemoryMapWindowOffset(qint64)));
}

void FormatWidget::initToolsWidget(ToolsWidget *pWidget)
{
    connect(pWidget,SIGNAL(editState(bool)),this,SLOT(setEdited(bool)));
    connect(pWidget,SIGNAL(showOffsetHex(qint64,qint64)),this,SLOT(showInHexWindow(qint64,qint64)));
    connect(pWidget,SIGNAL(showOffsetDisasm(qint64)),this,SLOT(showInDisasmWindowOffset(qint64)));
    connect(pWidget,SIGNAL(showOffsetMemoryMap(qint64)),this,SLOT(showInMemoryMapWindowOffset(qint64)));
    connect(pWidget,SIGNAL(showDemangle(QString)),this,SLOT(showDemangle(QString)));
}

qint32 FormatWidget::getColumnWidth(QWidget *pParent, FormatWidget::CW cw, XBinary::MODE mode)
{
    qint32 nResult=0;

    int nSymbolWidth=XLineEditHEX::getSymbolWidth(pParent);

    if(cw==CW_UINTMODE)
    {
        switch(mode)
        {
            case XBinary::MODE_16:      cw=CW_UINT16;       break;
            case XBinary::MODE_32:      cw=CW_UINT32;       break;
            case XBinary::MODE_64:      cw=CW_UINT64;       break;
            default:                    cw=CW_UINT32;
        }
    }

    switch(cw)
    {
        case CW_UINT8:          nResult=2*nSymbolWidth;         break;
        case CW_UINT16:         nResult=4*nSymbolWidth;         break;
        case CW_UINT32:         nResult=8*nSymbolWidth;         break;
        case CW_UINT64:         nResult=14*nSymbolWidth;        break;
        case CW_TYPE:           nResult=8*nSymbolWidth;         break;
        case CW_STRINGSHORT:    nResult=10*nSymbolWidth;        break;
        case CW_STRINGMID:      nResult=25*nSymbolWidth;        break;
        case CW_STRINGLONG:     nResult=50*nSymbolWidth;        break;
        default:                nResult=8*nSymbolWidth;         break;
    }

    return nResult;
}

void FormatWidget::_showInDisasmWindowAddress(qint64 nAddress)
{
    qDebug("TODO _showInDisasmWindowAddress");
}

void FormatWidget::_showInDisasmWindowOffset(qint64 nOffset)
{
    qDebug("TODO _showInDisasmWindowOffset");
}

void FormatWidget::_showInMemoryMapWindowOffset(qint64 nOffset)
{
    qDebug("TODO _showInMemoryMapWindowOffset");
}

void FormatWidget::_showInHexWindow(qint64 nOffset, qint64 nSize)
{
    qDebug("TODO _showInHexWindow");
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
    XLineEditHEX *pLineEdit=qobject_cast<XLineEditHEX *>(sender());

    int nStype=pLineEdit->property("STYPE").toInt();
    int nNdata=pLineEdit->property("NDATA").toInt();
    int nVtype=pLineEdit->property("VTYPE").toInt();
    int nPosition=pLineEdit->property("POSITION").toInt();
    qint64 nOffset=pLineEdit->property("OFFSET").toLongLong();

    setValue(nValue,nStype,nNdata,nVtype,nPosition,nOffset);
}

void FormatWidget::textValueChanged(QString sText)
{
    XLineEditHEX *pLineEdit=qobject_cast<XLineEditHEX *>(sender());

    int nStype=pLineEdit->property("STYPE").toInt();
    int nNdata=pLineEdit->property("NDATA").toInt();
    int nVtype=pLineEdit->property("VTYPE").toInt();
    int nPosition=pLineEdit->property("POSITION").toInt();
    qint64 nOffset=pLineEdit->property("OFFSET").toLongLong();

    setValue(sText,nStype,nNdata,nVtype,nPosition,nOffset);
}

void FormatWidget::setEdited(bool bState)
{
    reset();

    emit editState(bState);
}

void FormatWidget::showHex(qint64 nOffset, qint64 nSize)
{
    // mb TODO StartAddress
    XHexView::OPTIONS hexOptions={};

    hexOptions.nStartAddress=0;
    hexOptions.nStartSelectionOffset=nOffset;
    hexOptions.nSizeOfSelection=nSize;

    DialogHexView dialogHexView(this,g_pDevice,hexOptions);
    dialogHexView.setShortcuts(getShortcuts());

    connect(&dialogHexView,SIGNAL(editState(bool)),this,SLOT(setEdited(bool)));

    dialogHexView.exec();

    reloadData();
}

void FormatWidget::showInDisasmWindowAddress(qint64 nAddress)
{
    _showInDisasmWindowAddress(nAddress);
}

void FormatWidget::showInDisasmWindowOffset(qint64 nOffset)
{
    _showInDisasmWindowOffset(nOffset);
}

void FormatWidget::showInMemoryMapWindowOffset(qint64 nOffset)
{
     _showInMemoryMapWindowOffset(nOffset);
}

void FormatWidget::showInHexWindow(qint64 nOffset, qint64 nSize)
{
    _showInHexWindow(nOffset,nSize);
}

void FormatWidget::showEntropy(qint64 nOffset, qint64 nSize)
{
    DialogEntropy dialogEntropy(this,g_pDevice,nOffset,nSize);

    dialogEntropy.exec();
}

void FormatWidget::showDemangle(QString sString)
{
    DialogDemangle dialogDemangle(this,sString);

    dialogDemangle.exec();
}

bool FormatWidget::saveBackup()
{
    bool bResult=true;
    // TODO Check
    if(!isEdited())
    {
        // Save backup
        if(g_pBackupDevice)
        {
            bResult=XBinary::saveBackup(g_pBackupDevice);
        }
        else
        {
            bResult=XBinary::saveBackup(getDevice());
        }
    }

    return bResult;
}

void FormatWidget::registerShortcuts(bool bState)
{
    Q_UNUSED(bState)
}

bool FormatWidget::createHeaderTable(int nType, QTableWidget *pTableWidget, const FW_DEF::HEADER_RECORD *pRecords, XLineEditHEX **ppLineEdits, int nNumberOfRecords, int nPosition, qint64 nOffset)
{
    pTableWidget->setColumnCount(6);
    pTableWidget->setRowCount(nNumberOfRecords);

    QStringList slHeader;
    slHeader.append(tr("Name"));
    slHeader.append(tr("Offset"));
    slHeader.append(tr("Type"));
    slHeader.append(tr("Value"));
    slHeader.append(tr(""));
    slHeader.append(tr(""));

    pTableWidget->setHorizontalHeaderLabels(slHeader);
    pTableWidget->horizontalHeader()->setVisible(true);

    for(int i=0; i<nNumberOfRecords; i++)
    {
        QTableWidgetItem *pItemName=new QTableWidgetItem;
        pItemName->setText(pRecords[i].sName);
        pItemName->setData(Qt::UserRole+HEADER_DATA_OFFSET,pRecords[i].nOffset);
        pItemName->setData(Qt::UserRole+HEADER_DATA_SIZE,pRecords[i].nSize);
        pTableWidget->setItem(i,HEADER_COLUMN_NAME,pItemName);

        QTableWidgetItem *pItemOffset=new QTableWidgetItem;

        if(pRecords[i].nOffset!=-1)
        {
            pItemOffset->setText(XBinary::valueToHex((quint16)pRecords[i].nOffset));
        }

        pTableWidget->setItem(i,HEADER_COLUMN_OFFSET,pItemOffset);

        QTableWidgetItem *pItemType=new QTableWidgetItem;
        pItemType->setText(pRecords[i].sType);
        pTableWidget->setItem(i,HEADER_COLUMN_TYPE,pItemType);

        ppLineEdits[i]=new XLineEditHEX(this);
        ppLineEdits[i]->setProperty("STYPE",nType);
        ppLineEdits[i]->setProperty("NDATA",pRecords[i].nData);
        ppLineEdits[i]->setProperty("POSITION",nPosition);
        ppLineEdits[i]->setProperty("OFFSET",nOffset);

        if((pRecords[i].vtype==FW_DEF::VAL_TYPE_TEXT)||(pRecords[i].vtype==FW_DEF::VAL_TYPE_UUID))
        {
            connect(ppLineEdits[i],SIGNAL(valueChanged(QString)),this,SLOT(textValueChanged(QString)));

            if(pRecords[i].nSize!=-1)
            {
                ppLineEdits[i]->setMaxLength(pRecords[i].nSize);
            }
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

    adjustHeaderTable(nType,pTableWidget);

    return true;
}

bool FormatWidget::createListTable(int nType, QTableWidget *pTableWidget, const FW_DEF::HEADER_RECORD *pRecords, XLineEditHEX **ppLineEdits, int nNumberOfRecords)
{
    pTableWidget->setColumnCount(2);
    pTableWidget->setRowCount(nNumberOfRecords);

    QStringList slHeader;
    slHeader.append(tr("Name"));
    slHeader.append(tr("Value"));

    pTableWidget->setHorizontalHeaderLabels(slHeader);
    pTableWidget->horizontalHeader()->setVisible(true);

    for(int i=0; i<nNumberOfRecords; i++)
    {
        QTableWidgetItem *newItemName=new QTableWidgetItem;
        newItemName->setText(pRecords[i].sName);
        pTableWidget->setItem(i,LIST_COLUMN_NAME,newItemName);

        ppLineEdits[i]=new XLineEditHEX(this);

        ppLineEdits[i]->setProperty("STYPE",nType);
        ppLineEdits[i]->setProperty("NDATA",pRecords[i].nData);

        if(pRecords[i].vtype==FW_DEF::VAL_TYPE_TEXT)
        {
            ppLineEdits[i]->setAlignment(Qt::AlignLeft);
        }

        if(pRecords[i].nOffset!=-1)
        {
            if((pRecords[i].vtype==FW_DEF::VAL_TYPE_TEXT)||(pRecords[i].vtype==FW_DEF::VAL_TYPE_UUID))
            {
                connect(ppLineEdits[i],SIGNAL(valueChanged(QString)),this,SLOT(textValueChanged(QString)));
            }
            else
            {
                connect(ppLineEdits[i],SIGNAL(valueChanged(quint64)),this,SLOT(hexValueChanged(quint64)));
            }
        }
        else
        {
            ppLineEdits[i]->setReadOnly(true);
        }

        pTableWidget->setCellWidget(i,LIST_COLUMN_VALUE,ppLineEdits[i]);
    }

    pTableWidget->horizontalHeader()->setSectionResizeMode(LIST_COLUMN_VALUE,QHeaderView::Stretch);

    adjustListTable(nType,pTableWidget);

    return true;
}

void FormatWidget::addComment(QTableWidget *pTableWidget, int nRow, int nColumn, QString sComment)
{
    pTableWidget->item(nRow,nColumn)->setText(sComment);
}

void FormatWidget::updateTableRecord(QTableWidget *pTableWidget, int nRow, qint64 nOffset, qint64 nSize)
{
    pTableWidget->item(nRow,HEADER_COLUMN_NAME)->setData(Qt::UserRole+HEADER_DATA_OFFSET,nOffset);
    pTableWidget->item(nRow,HEADER_COLUMN_NAME)->setData(Qt::UserRole+HEADER_DATA_SIZE,nSize);
    pTableWidget->item(nRow,HEADER_COLUMN_OFFSET)->setText(XBinary::valueToHex((quint16)nOffset));
    pTableWidget->cellWidget(nRow,HEADER_COLUMN_VALUE)->setProperty("OFFSET",nOffset);
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

bool FormatWidget::createSectionTable(int nType, QTableWidget *pTableWidget, const FW_DEF::HEADER_RECORD *pRecords, int nNumberOfRecords)
{
    Q_UNUSED(nType)

    QStringList slHeader;

    pTableWidget->setRowCount(0);

    for(int i=0; i<nNumberOfRecords; i++)
    {
        slHeader.append(pRecords[i].sName);
    }

    pTableWidget->setColumnCount(nNumberOfRecords);
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

XComboBoxEx *FormatWidget::createComboBox(QTableWidget *pTableWidget, QMap<quint64, QString> mapData, int nType, int nData, XComboBoxEx::CBTYPE cbtype, quint64 nMask)
{
    XComboBoxEx *result=new XComboBoxEx(this);
    result->setData(mapData,cbtype,nMask);

    result->setProperty("STYPE",nType);
    result->setProperty("NDATA",nData);

    connect(result,SIGNAL(valueChanged(quint64)),this,SLOT(widgetValueChanged(quint64)));

    pTableWidget->setCellWidget(nData,HEADER_COLUMN_INFO,result);

    return result;
}

InvWidget *FormatWidget::createInvWidget(QTableWidget *pTableWidget, int nType, int nData, InvWidget::TYPE widgetType)
{
    InvWidget *pResult=new InvWidget(this,widgetType);

    pResult->setProperty("STYPE",nType);
    pResult->setProperty("NDATA",nData);

    connect(pResult,SIGNAL(showHex(qint64,qint64)),this,SLOT(showInHexWindow(qint64,qint64)));
    connect(pResult,SIGNAL(showDisasm(qint64)),this,SLOT(showInDisasmWindowAddress(qint64)));

    pTableWidget->setCellWidget(nData,HEADER_COLUMN_INFO,pResult);

    return pResult;
}

XDateTimeEditX *FormatWidget::createTimeDateEdit(QTableWidget *pTableWidget, int nType, int nData, XDateTimeEditX::DT_TYPE dtType)
{
    XDateTimeEditX *result=new XDateTimeEditX(this);
    result->setType(dtType);

    result->setProperty("STYPE",nType);
    result->setProperty("NDATA",nData);

    connect(result,SIGNAL(valueChanged(quint64)),this,SLOT(widgetValueChanged(quint64)));

    pTableWidget->setCellWidget(nData,HEADER_COLUMN_INFO,result);

    return result;
}

QPushButton *FormatWidget::createPushButton(QTableWidget *pTableWidget, int nType, int nData, QString sText)
{
    QPushButton *result=new QPushButton(this);

    result->setText(sText);
    result->setProperty("STYPE",nType);
    result->setProperty("NDATA",nData);

    connect(result,SIGNAL(clicked()),this,SLOT(widgetAction()));

    pTableWidget->setCellWidget(nData,HEADER_COLUMN_INFO,result);

    return result;
}

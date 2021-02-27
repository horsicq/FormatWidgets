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
#include "msdoswidget.h"
#include "ui_msdoswidget.h"

MSDOSWidget::MSDOSWidget(QWidget *pParent) :
    FormatWidget(pParent),
    ui(new Ui::MSDOSWidget)
{
    ui->setupUi(this);

    connect(ui->widgetStrings,SIGNAL(showHex(qint64,qint64)),this,SLOT(showInHexWindow(qint64,qint64)));
}

MSDOSWidget::MSDOSWidget(QIODevice *pDevice, FW_DEF::OPTIONS options, QWidget *pParent) :
    MSDOSWidget(pParent)
{
    MSDOSWidget::setData(pDevice,options,0,0,0);
    MSDOSWidget::reload();
}

MSDOSWidget::~MSDOSWidget()
{
    delete ui;
}

void MSDOSWidget::setShortcuts(XShortcuts *pShortcuts)
{
    ui->widgetHex->setShortcuts(pShortcuts);
    ui->widgetDisasm->setShortcuts(pShortcuts);
    ui->widgetStrings->setShortcuts(pShortcuts);
    ui->widgetEntropy->setShortcuts(pShortcuts);
    ui->widgetHeuristicScan->setShortcuts(pShortcuts);
    ui->widgetMemoryMap->setShortcuts(pShortcuts);
    ui->widgetHex_DOS_HEADER->setShortcuts(pShortcuts);
    ui->widgetHex_OVERLAY->setShortcuts(pShortcuts);

    FormatWidget::setShortcuts(pShortcuts);
}

void MSDOSWidget::clear()
{
    MSDOSWidget::reset();

    memset(g_lineEdit_DOS_HEADER,0,sizeof g_lineEdit_DOS_HEADER);
    memset(g_comboBox,0,sizeof g_comboBox);
    memset(g_subDevice,0,sizeof g_subDevice);

    ui->checkBoxReadonly->setChecked(true);

    ui->treeWidgetNavi->clear();
}

void MSDOSWidget::cleanup()
{

}

void MSDOSWidget::reset()
{
    g_stInit.clear();
}

void MSDOSWidget::reload()
{
    MSDOSWidget::clear();

    ui->checkBoxReadonly->setEnabled(!isReadonly());

    XMSDOS msdos(getDevice(),getOptions().bIsImage,getOptions().nImageBase);

    if(msdos.isValid())
    {
        setFileType(msdos.getFileType());

        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMSDOS::TYPE_HEX,tr("Hex")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMSDOS::TYPE_DISASM,tr("Disasm")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMSDOS::TYPE_STRINGS,tr("Strings")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMSDOS::TYPE_MEMORYMAP,tr("Memory map")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMSDOS::TYPE_ENTROPY,tr("Entropy")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMSDOS::TYPE_HEURISTICSCAN,tr("Heuristic scan")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMSDOS::TYPE_DOS_HEADER,"DOS_HEADER"));

        if(msdos.isOverlayPresent())
        {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMSDOS::TYPE_OVERLAY,tr("Overlay")));
        }

        ui->treeWidgetNavi->expandAll();

        setTreeItem(ui->treeWidgetNavi,getOptions().nStartType);
    }
}

bool MSDOSWidget::_setValue(QVariant vValue, int nStype, int nNdata, int nVtype, int nPosition, qint64 nOffset)
{
    Q_UNUSED(nVtype)
    Q_UNUSED(nPosition)
    Q_UNUSED(nOffset)

    bool bResult=false;

    blockSignals(true);

    quint64 nValue=vValue.toULongLong();

    if(getDevice()->isWritable())
    {
        XMSDOS msdos(getDevice(),getOptions().bIsImage,getOptions().nImageBase);

        if(msdos.isValid())
        {
            switch(nStype)
            {
                case SMSDOS::TYPE_DOS_HEADER:
                    switch(nNdata)
                    {
                        case N_DOS_HEADER::e_magic:     g_comboBox[CB_DOS_HEADER_e_magic]->setValue(nValue);      break;
                    }
                    break;
            }

            switch(nStype)
            {
                case SMSDOS::TYPE_DOS_HEADER:
                    switch(nNdata)
                    {
                        case N_DOS_HEADER::e_magic:         msdos.set_e_magic((quint16)nValue);     break;
                        case N_DOS_HEADER::e_cblp:          msdos.set_e_cblp((quint16)nValue);      break;
                        case N_DOS_HEADER::e_cp:            msdos.set_e_cp((quint16)nValue);        break;
                        case N_DOS_HEADER::e_crlc:          msdos.set_e_crlc((quint16)nValue);      break;
                        case N_DOS_HEADER::e_cparhdr:       msdos.set_e_cparhdr((quint16)nValue);   break;
                        case N_DOS_HEADER::e_minalloc:      msdos.set_e_minalloc((quint16)nValue);  break;
                        case N_DOS_HEADER::e_maxalloc:      msdos.set_e_maxalloc((quint16)nValue);  break;
                        case N_DOS_HEADER::e_ss:            msdos.set_e_ss((quint16)nValue);        break;
                        case N_DOS_HEADER::e_sp:            msdos.set_e_sp((quint16)nValue);        break;
                        case N_DOS_HEADER::e_csum:          msdos.set_e_csum((quint16)nValue);      break;
                        case N_DOS_HEADER::e_ip:            msdos.set_e_ip((quint16)nValue);        break;
                        case N_DOS_HEADER::e_cs:            msdos.set_e_cs((quint16)nValue);        break;
                        case N_DOS_HEADER::e_lfarlc:        msdos.set_e_lfarlc((quint16)nValue);    break;
                        case N_DOS_HEADER::e_ovno:          msdos.set_e_ovno((quint16)nValue);      break;
                    }

                    ui->widgetHex_DOS_HEADER->reload();

                    break;
            }

            bResult=true;
        }
    }

    blockSignals(false);

    return bResult;
}

void MSDOSWidget::setReadonly(bool bState)
{
    setLineEditsReadOnly(g_lineEdit_DOS_HEADER,N_DOS_HEADER::__data_size,bState);

    setComboBoxesReadOnly(g_comboBox,__CB_size,bState);

    ui->widgetHex->setReadonly(bState);
}

void MSDOSWidget::blockSignals(bool bState)
{
    _blockSignals((QObject **)g_lineEdit_DOS_HEADER,N_DOS_HEADER::__data_size,bState);

    _blockSignals((QObject **)g_comboBox,__CB_size,bState);
}

void MSDOSWidget::adjustHeaderTable(int nType, QTableWidget *pTableWidget)
{
    Q_UNUSED(nType);

    int nSymbolWidth=XLineEditHEX::getSymbolWidth(this);

    pTableWidget->setColumnWidth(HEADER_COLUMN_NAME,nSymbolWidth*12);
    pTableWidget->setColumnWidth(HEADER_COLUMN_OFFSET,nSymbolWidth*4);
    pTableWidget->setColumnWidth(HEADER_COLUMN_TYPE,nSymbolWidth*6);
    pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE,nSymbolWidth*8);
    pTableWidget->setColumnWidth(HEADER_COLUMN_INFO,nSymbolWidth*16);
}

void MSDOSWidget::reloadData()
{
    int nType=ui->treeWidgetNavi->currentItem()->data(0,Qt::UserRole+FW_DEF::SECTION_DATA_TYPE).toInt();
    qint64 nDataOffset=ui->treeWidgetNavi->currentItem()->data(0,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET).toLongLong();
    qint64 nDataSize=ui->treeWidgetNavi->currentItem()->data(0,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE).toLongLong();

    QString sInit=QString("%1-%2-%3").arg(nType).arg(nDataOffset).arg(nDataSize);

    if((g_nLastType==nType)&&(sInit!=g_sLastInit))
    {
        g_stInit.remove(sInit);
    }

    g_nLastType=nType;
    g_sLastInit=sInit;

    ui->stackedWidgetInfo->setCurrentIndex(nType);

    XMSDOS msdos(getDevice(),getOptions().bIsImage,getOptions().nImageBase);

    if(msdos.isValid())
    {
        if(nType==SMSDOS::TYPE_HEX)
        {
            if(!g_stInit.contains(sInit))
            {
                XHexView::OPTIONS options={};
                options.bMenu_Disasm=true;
                options.bMenu_MemoryMap=true;
                options.sSignaturesPath=getOptions().sSearchSignaturesPath;
                ui->widgetHex->setData(getDevice(),options);
//                ui->widgetHex->setBackupFileName(getOptions().sBackupFileName);
                ui->widgetHex->enableReadOnly(false);
                connect(ui->widgetHex,SIGNAL(editState(bool)),this,SLOT(setEdited(bool)));
            }
        }
        else if(nType==SMSDOS::TYPE_DISASM)
        {
            if(!g_stInit.contains(sInit))
            {
                XMultiDisasmWidget::OPTIONS options={};
                options.fileType=msdos.getFileType();
                options.nInitAddress=msdos.getEntryPointAddress();
                options.sSignaturesPath=getOptions().sSearchSignaturesPath;

                ui->widgetDisasm->setData(getDevice(),options);
            }
        }
        else if(nType==SMSDOS::TYPE_STRINGS)
        {
            if(!g_stInit.contains(sInit))
            {
                SearchStringsWidget::OPTIONS stringsOptions={};
                stringsOptions.bMenu_Hex=true;
                stringsOptions.bAnsi=true;
                stringsOptions.bUnicode=true;

                ui->widgetStrings->setData(getDevice(),stringsOptions,true);
            }
        }
        else if(nType==SMSDOS::TYPE_MEMORYMAP)
        {
            if(!g_stInit.contains(sInit))
            {
                ui->widgetMemoryMap->setData(getDevice(),msdos.getFileType(),getOptions().sSearchSignaturesPath);
            }
        }
        else if(nType==SMSDOS::TYPE_ENTROPY)
        {
            if(!g_stInit.contains(sInit))
            {
                ui->widgetEntropy->setData(getDevice(),0,getDevice()->size(),msdos.getFileType(),true,this);
            }
        }
        else if(nType==SMSDOS::TYPE_HEURISTICSCAN)
        {
            if(!g_stInit.contains(sInit))
            {
                ui->widgetHeuristicScan->setData(getDevice(),true,msdos.getFileType());
            }
        }
        else if(nType==SMSDOS::TYPE_DOS_HEADER)
        {
            if(!g_stInit.contains(sInit))
            {
                createHeaderTable(SMSDOS::TYPE_DOS_HEADER,ui->tableWidget_DOS_HEADER,N_DOS_HEADER::records,g_lineEdit_DOS_HEADER,N_DOS_HEADER::__data_size,0);
                g_comboBox[CB_DOS_HEADER_e_magic]=createComboBox(ui->tableWidget_DOS_HEADER,XMSDOS::getImageMagicsS(),SMSDOS::TYPE_DOS_HEADER,N_DOS_HEADER::e_magic,XComboBoxEx::CBTYPE_NORMAL);

                blockSignals(true);

                XMSDOS_DEF::IMAGE_DOS_HEADEREX msdosheaderex=msdos.getDosHeaderEx();

                g_lineEdit_DOS_HEADER[N_DOS_HEADER::e_magic]->setValue(msdosheaderex.e_magic);
                g_lineEdit_DOS_HEADER[N_DOS_HEADER::e_cblp]->setValue(msdosheaderex.e_cblp);
                g_lineEdit_DOS_HEADER[N_DOS_HEADER::e_cp]->setValue(msdosheaderex.e_cp);
                g_lineEdit_DOS_HEADER[N_DOS_HEADER::e_crlc]->setValue(msdosheaderex.e_crlc);
                g_lineEdit_DOS_HEADER[N_DOS_HEADER::e_cparhdr]->setValue(msdosheaderex.e_cparhdr);
                g_lineEdit_DOS_HEADER[N_DOS_HEADER::e_minalloc]->setValue(msdosheaderex.e_minalloc);
                g_lineEdit_DOS_HEADER[N_DOS_HEADER::e_maxalloc]->setValue(msdosheaderex.e_maxalloc);
                g_lineEdit_DOS_HEADER[N_DOS_HEADER::e_ss]->setValue(msdosheaderex.e_ss);
                g_lineEdit_DOS_HEADER[N_DOS_HEADER::e_sp]->setValue(msdosheaderex.e_sp);
                g_lineEdit_DOS_HEADER[N_DOS_HEADER::e_csum]->setValue(msdosheaderex.e_csum);
                g_lineEdit_DOS_HEADER[N_DOS_HEADER::e_ip]->setValue(msdosheaderex.e_ip);
                g_lineEdit_DOS_HEADER[N_DOS_HEADER::e_cs]->setValue(msdosheaderex.e_cs);
                g_lineEdit_DOS_HEADER[N_DOS_HEADER::e_lfarlc]->setValue(msdosheaderex.e_lfarlc);
                g_lineEdit_DOS_HEADER[N_DOS_HEADER::e_ovno]->setValue(msdosheaderex.e_ovno);

                g_comboBox[CB_DOS_HEADER_e_magic]->setValue(msdosheaderex.e_magic);

                qint64 nOffset=msdos.getDosHeaderExOffset(); // Ex!
                qint64 nSize=msdos.getDosHeaderExSize();

                loadHexSubdevice(nOffset,nSize,nOffset,&g_subDevice[SMSDOS::TYPE_DOS_HEADER],ui->widgetHex_DOS_HEADER);

                blockSignals(false);
            }
        }
        else if(nType==SMSDOS::TYPE_OVERLAY)
        {
            if(!g_stInit.contains(sInit))
            {
                qint64 nOverLayOffset=msdos.getOverlayOffset();
                qint64 nOverlaySize=msdos.getOverlaySize();

                loadHexSubdevice(nOverLayOffset,nOverlaySize,nOverLayOffset,&g_subDevice[SMSDOS::TYPE_OVERLAY],ui->widgetHex_OVERLAY);
            }
        }

        setReadonly(ui->checkBoxReadonly->isChecked());
    }

    g_stInit.insert(sInit);
}

void MSDOSWidget::widgetValueChanged(quint64 nValue)
{
    QWidget *pWidget=qobject_cast<QWidget *>(sender());
    int nStype=pWidget->property("STYPE").toInt();
    int nNdata=pWidget->property("NDATA").toInt();

    switch(nStype)
    {
        case SMSDOS::TYPE_DOS_HEADER:
            switch(nNdata)
            {
                case N_DOS_HEADER::e_magic:
                    g_lineEdit_DOS_HEADER[N_DOS_HEADER::e_magic]->setValue((quint16)nValue);
                    break;
            }

            break;
    }
}

void MSDOSWidget::on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *pCurrent, QTreeWidgetItem *pPrevious)
{
    Q_UNUSED(pPrevious)

    if(pCurrent)
    {
        reloadData();
        addPage(pCurrent);
        ui->toolButtonPrev->setEnabled(isPrevPageAvailable());
        ui->toolButtonNext->setEnabled(isNextPageAvailable());
    }
}

void MSDOSWidget::on_checkBoxReadonly_toggled(bool bChecked)
{
    setReadonly(bChecked);
}

void MSDOSWidget::on_pushButtonReload_clicked()
{
    ui->pushButtonReload->setEnabled(false);
    reload();

    QTimer::singleShot(1000,this,SLOT(enableButton()));
}

void MSDOSWidget::enableButton()
{
    ui->pushButtonReload->setEnabled(true);
}

void MSDOSWidget::on_tableWidget_DOS_HEADER_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow);
    Q_UNUSED(nCurrentColumn);
    Q_UNUSED(nPreviousRow);
    Q_UNUSED(nPreviousColumn);

    setHeaderTableSelection(ui->widgetHex_DOS_HEADER,ui->tableWidget_DOS_HEADER);
}

void MSDOSWidget::on_toolButtonPrev_clicked()
{
    setAddPageEnabled(false);
    ui->treeWidgetNavi->setCurrentItem(getPrevPage());
    setAddPageEnabled(true);
}

void MSDOSWidget::on_toolButtonNext_clicked()
{
    setAddPageEnabled(false);
    ui->treeWidgetNavi->setCurrentItem(getNextPage());
    setAddPageEnabled(true);
}

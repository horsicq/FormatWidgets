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
#include "msdoswidget.h"
#include "ui_msdoswidget.h"

MSDOSWidget::MSDOSWidget(QWidget *pParent) :
    FormatWidget(pParent),
    ui(new Ui::MSDOSWidget)
{
    ui->setupUi(this);
}

MSDOSWidget::MSDOSWidget(QIODevice *pDevice, FW_DEF::OPTIONS *pOptions, QWidget *pParent) :
    FormatWidget(pDevice,pOptions,0,0,0,pParent),
    ui(new Ui::MSDOSWidget)
{
    ui->setupUi(this);

    setData(pDevice,pOptions);
    reload();
}

MSDOSWidget::~MSDOSWidget()
{
    delete ui;
}

void MSDOSWidget::clear()
{
    reset();

    memset(lineEdit_DOS_HEADER,0,sizeof lineEdit_DOS_HEADER);
    memset(comboBox,0,sizeof comboBox);
    memset(subDevice,0,sizeof subDevice);

    ui->checkBoxReadonly->setChecked(true);

    ui->treeWidgetNavi->clear();
}

void MSDOSWidget::cleanup()
{

}

void MSDOSWidget::reset()
{
    stInit.clear();
}

void MSDOSWidget::setData(QIODevice *pDevice, FW_DEF::OPTIONS *pOptions)
{
    FormatWidget::setData(pDevice,pOptions,0,0,0);
}

void MSDOSWidget::reload()
{
    clear();

    ui->checkBoxReadonly->setEnabled(!isReadonly());

    XMSDOS msdos(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

    if(msdos.isValid())
    {
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMSDOS::TYPE_HEX,QString("Hex")));
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

        setTreeItem(ui->treeWidgetNavi,getOptions()->nStartType);
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
        XMSDOS msdos(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

        if(msdos.isValid())
        {
            switch(nStype)
            {
                case SMSDOS::TYPE_DOS_HEADER:
                    switch(nNdata)
                    {
                        case N_DOS_HEADER::e_magic:     comboBox[CB_DOS_HEADER_e_magic]->setValue(nValue);      break;
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
    setLineEditsReadOnly(lineEdit_DOS_HEADER,N_DOS_HEADER::__data_size,bState);

    setComboBoxesReadOnly(comboBox,__CB_size,bState);

    ui->widgetHex->setReadonly(bState);
}

void MSDOSWidget::blockSignals(bool bState)
{
    _blockSignals((QObject **)lineEdit_DOS_HEADER,N_DOS_HEADER::__data_size,bState);

    _blockSignals((QObject **)comboBox,__CB_size,bState);
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

    if((nLastType==nType)&&(sInit!=sLastInit))
    {
        stInit.remove(sInit);
    }

    nLastType=nType;
    sLastInit=sInit;

    ui->stackedWidgetInfo->setCurrentIndex(nType);

    XMSDOS msdos(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

    if(msdos.isValid())
    {
        if(nType==SMSDOS::TYPE_HEX)
        {
            if(!stInit.contains(sInit))
            {
                ui->widgetHex->setData(getDevice());
                ui->widgetHex->setBackupFileName(getOptions()->sBackupFileName);
                ui->widgetHex->enableReadOnly(false);
                connect(ui->widgetHex,SIGNAL(editState(bool)),this,SLOT(setEdited(bool)));
            }
        }
        else if(nType==SMSDOS::TYPE_STRINGS)
        {
            if(!stInit.contains(sInit))
            {
                ui->widgetStrings->setData(getDevice(),0,true);
            }
        }
        else if(nType==SMSDOS::TYPE_MEMORYMAP)
        {
            if(!stInit.contains(sInit))
            {
                ui->widgetMemoryMap->setData(getDevice());
            }
        }
        else if(nType==SMSDOS::TYPE_ENTROPY)
        {
            if(!stInit.contains(sInit))
            {
                ui->widgetEntropy->setData(getDevice(),0,getDevice()->size(),true);
            }
        }
        else if(nType==SMSDOS::TYPE_HEURISTICSCAN)
        {
            if(!stInit.contains(sInit))
            {
                ui->widgetHeuristicScan->setData(getDevice(),true,XBinary::FT_MSDOS);
            }
        }
        else if(nType==SMSDOS::TYPE_DOS_HEADER)
        {
            if(!stInit.contains(sInit))
            {
                createHeaderTable(SMSDOS::TYPE_DOS_HEADER,ui->tableWidget_DOS_HEADER,N_DOS_HEADER::records,lineEdit_DOS_HEADER,N_DOS_HEADER::__data_size,0);
                comboBox[CB_DOS_HEADER_e_magic]=createComboBox(ui->tableWidget_DOS_HEADER,XMSDOS::getImageMagicsS(),SMSDOS::TYPE_DOS_HEADER,N_DOS_HEADER::e_magic,XComboBoxEx::CBTYPE_NORMAL);

                blockSignals(true);

                XMSDOS_DEF::IMAGE_DOS_HEADEREX msdosheaderex=msdos.getDosHeaderEx();

                lineEdit_DOS_HEADER[N_DOS_HEADER::e_magic]->setValue(msdosheaderex.e_magic);
                lineEdit_DOS_HEADER[N_DOS_HEADER::e_cblp]->setValue(msdosheaderex.e_cblp);
                lineEdit_DOS_HEADER[N_DOS_HEADER::e_cp]->setValue(msdosheaderex.e_cp);
                lineEdit_DOS_HEADER[N_DOS_HEADER::e_crlc]->setValue(msdosheaderex.e_crlc);
                lineEdit_DOS_HEADER[N_DOS_HEADER::e_cparhdr]->setValue(msdosheaderex.e_cparhdr);
                lineEdit_DOS_HEADER[N_DOS_HEADER::e_minalloc]->setValue(msdosheaderex.e_minalloc);
                lineEdit_DOS_HEADER[N_DOS_HEADER::e_maxalloc]->setValue(msdosheaderex.e_maxalloc);
                lineEdit_DOS_HEADER[N_DOS_HEADER::e_ss]->setValue(msdosheaderex.e_ss);
                lineEdit_DOS_HEADER[N_DOS_HEADER::e_sp]->setValue(msdosheaderex.e_sp);
                lineEdit_DOS_HEADER[N_DOS_HEADER::e_csum]->setValue(msdosheaderex.e_csum);
                lineEdit_DOS_HEADER[N_DOS_HEADER::e_ip]->setValue(msdosheaderex.e_ip);
                lineEdit_DOS_HEADER[N_DOS_HEADER::e_cs]->setValue(msdosheaderex.e_cs);
                lineEdit_DOS_HEADER[N_DOS_HEADER::e_lfarlc]->setValue(msdosheaderex.e_lfarlc);
                lineEdit_DOS_HEADER[N_DOS_HEADER::e_ovno]->setValue(msdosheaderex.e_ovno);

                comboBox[CB_DOS_HEADER_e_magic]->setValue(msdosheaderex.e_magic);

                qint64 nOffset=msdos.getDosHeaderExOffset(); // Ex!
                qint64 nSize=msdos.getDosHeaderExSize();

                loadHexSubdevice(nOffset,nSize,nOffset,&subDevice[SMSDOS::TYPE_DOS_HEADER],ui->widgetHex_DOS_HEADER);

                blockSignals(false);
            }
        }
        else if(nType==SMSDOS::TYPE_OVERLAY)
        {
            if(!stInit.contains(sInit))
            {
                qint64 nOverLayOffset=msdos.getOverlayOffset();
                qint64 nOverlaySize=msdos.getOverlaySize();

                loadHexSubdevice(nOverLayOffset,nOverlaySize,nOverLayOffset,&subDevice[SMSDOS::TYPE_OVERLAY],ui->widgetHex_OVERLAY);
            }
        }

        setReadonly(ui->checkBoxReadonly->isChecked());
    }

    stInit.insert(sInit);
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
                    lineEdit_DOS_HEADER[N_DOS_HEADER::e_magic]->setValue((quint16)nValue);
                    break;
            }

            break;
    }
}

void MSDOSWidget::on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(previous)

    if(current)
    {
        reloadData();
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

void MSDOSWidget::on_tableWidget_DOS_HEADER_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(currentRow);
    Q_UNUSED(currentColumn);
    Q_UNUSED(previousRow);
    Q_UNUSED(previousColumn);

    setHeaderTableSelection(ui->widgetHex_DOS_HEADER,ui->tableWidget_DOS_HEADER);
}

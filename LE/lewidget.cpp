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
#include "lewidget.h"
#include "ui_lewidget.h"

LEWidget::LEWidget(QWidget *parent) :
    FormatWidget(parent),
    ui(new Ui::LEWidget)
{
    ui->setupUi(this);
}

LEWidget::LEWidget(QIODevice *pDevice, FW_DEF::OPTIONS *pOptions, QWidget *parent) :
    FormatWidget(pDevice,pOptions,0,0,0,parent),
    ui(new Ui::LEWidget)
{
    ui->setupUi(this);

    setData(pDevice,pOptions);
    reload();
}

LEWidget::~LEWidget()
{
    delete ui;
}

void LEWidget::clear()
{
    reset();

    memset(lineEdit_DOS_HEADER,0,sizeof lineEdit_DOS_HEADER);
    memset(lineEdit_VXD_HEADER,0,sizeof lineEdit_VXD_HEADER);
    memset(comboBox,0,sizeof comboBox);
    memset(invWidget,0,sizeof invWidget);
    memset(subDevice,0,sizeof subDevice);

    ui->checkBoxReadonly->setChecked(true);

    ui->treeWidgetNavi->clear();
}

void LEWidget::cleanup()
{

}

void LEWidget::reset()
{
    stInit.clear();
}

void LEWidget::setData(QIODevice *pDevice, FW_DEF::OPTIONS *pOptions)
{
    FormatWidget::setData(pDevice,pOptions,0,0,0);
}

void LEWidget::reload()
{
    clear();

    ui->checkBoxReadonly->setEnabled(!isReadonly());

    XLE le(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

    if(le.isValid())
    {
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SLE::TYPE_HEX,QString("Hex")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SLE::TYPE_STRINGS,tr("Strings")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SLE::TYPE_MEMORYMAP,tr("Memory map")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SLE::TYPE_ENTROPY,tr("Entropy")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SLE::TYPE_HEURISTICSCAN,tr("Heuristic scan")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SLE::TYPE_DOS_HEADER,"DOS_HEADER"));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SLE::TYPE_VXD_HEADER,"VXD_HEADER"));

        if(le.isOverlayPresent())
        {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SLE::TYPE_OVERLAY,tr("Overlay")));
        }

        ui->treeWidgetNavi->expandAll();

        ui->treeWidgetNavi->setCurrentItem(ui->treeWidgetNavi->topLevelItem(SLE::TYPE_DOS_HEADER));
    }
}

bool LEWidget::_setValue(QVariant vValue, int nStype, int nNdata, int nVtype, int nPosition, qint64 nOffset)
{
    Q_UNUSED(nVtype)
    Q_UNUSED(nPosition)
    Q_UNUSED(nOffset)

    bool bResult=false;

    blockSignals(true);

    quint64 nValue=vValue.toULongLong();

    if(getDevice()->isWritable())
    {
        XLE le(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

        if(le.isValid())
        {
            switch(nStype)
            {
                case SLE::TYPE_DOS_HEADER:
                    switch(nNdata)
                    {
                        case N_LE_DOS_HEADER::e_magic:     comboBox[CB_DOS_HEADER_e_magic]->setValue(nValue);                              break;
                        case N_LE_DOS_HEADER::e_lfanew:    invWidget[INV_DOS_HEADER_e_lfanew]->setOffsetAndSize(&le,(quint32)nValue,0);    break;
                    }
                    break;
                case SLE::TYPE_VXD_HEADER:
                    switch(nNdata)
                    {
                        case N_VXD_HEADER::e32_magic:   comboBox[CB_VXD_HEADER_e32_magic]->setValue(nValue);    break;
                        case N_VXD_HEADER::e32_cpu:     comboBox[CB_VXD_HEADER_e32_cpu]->setValue(nValue);      break;
                        case N_VXD_HEADER::e32_os:      comboBox[CB_VXD_HEADER_e32_os]->setValue(nValue);       break;
                    }
                    break;
            }

            switch(nStype)
            {
            case SLE::TYPE_DOS_HEADER:
                switch(nNdata)
                {
                    case N_LE_DOS_HEADER::e_magic:         le.set_e_magic((quint16)nValue);        break;
                    case N_LE_DOS_HEADER::e_cblp:          le.set_e_cblp((quint16)nValue);         break;
                    case N_LE_DOS_HEADER::e_cp:            le.set_e_cp((quint16)nValue);           break;
                    case N_LE_DOS_HEADER::e_crlc:          le.set_e_crlc((quint16)nValue);         break;
                    case N_LE_DOS_HEADER::e_cparhdr:       le.set_e_cparhdr((quint16)nValue);      break;
                    case N_LE_DOS_HEADER::e_minalloc:      le.set_e_minalloc((quint16)nValue);     break;
                    case N_LE_DOS_HEADER::e_maxalloc:      le.set_e_maxalloc((quint16)nValue);     break;
                    case N_LE_DOS_HEADER::e_ss:            le.set_e_ss((quint16)nValue);           break;
                    case N_LE_DOS_HEADER::e_sp:            le.set_e_sp((quint16)nValue);           break;
                    case N_LE_DOS_HEADER::e_csum:          le.set_e_csum((quint16)nValue);         break;
                    case N_LE_DOS_HEADER::e_ip:            le.set_e_ip((quint16)nValue);           break;
                    case N_LE_DOS_HEADER::e_cs:            le.set_e_cs((quint16)nValue);           break;
                    case N_LE_DOS_HEADER::e_lfarlc:        le.set_e_lfarlc((quint16)nValue);       break;
                    case N_LE_DOS_HEADER::e_ovno:          le.set_e_ovno((quint16)nValue);         break;
                    case N_LE_DOS_HEADER::e_res_0:         le.set_e_res(0,(quint16)nValue);        break;
                    case N_LE_DOS_HEADER::e_res_1:         le.set_e_res(1,(quint16)nValue);        break;
                    case N_LE_DOS_HEADER::e_res_2:         le.set_e_res(2,(quint16)nValue);        break;
                    case N_LE_DOS_HEADER::e_res_3:         le.set_e_res(3,(quint16)nValue);        break;
                    case N_LE_DOS_HEADER::e_oemid:         le.set_e_oemid((quint16)nValue);        break;
                    case N_LE_DOS_HEADER::e_oeminfo:       le.set_e_oeminfo((quint16)nValue);      break;
                    case N_LE_DOS_HEADER::e_res2_0:        le.set_e_res2(0,(quint16)nValue);       break;
                    case N_LE_DOS_HEADER::e_res2_1:        le.set_e_res2(1,(quint16)nValue);       break;
                    case N_LE_DOS_HEADER::e_res2_2:        le.set_e_res2(2,(quint16)nValue);       break;
                    case N_LE_DOS_HEADER::e_res2_3:        le.set_e_res2(3,(quint16)nValue);       break;
                    case N_LE_DOS_HEADER::e_res2_4:        le.set_e_res2(4,(quint16)nValue);       break;
                    case N_LE_DOS_HEADER::e_res2_5:        le.set_e_res2(5,(quint16)nValue);       break;
                    case N_LE_DOS_HEADER::e_res2_6:        le.set_e_res2(6,(quint16)nValue);       break;
                    case N_LE_DOS_HEADER::e_res2_7:        le.set_e_res2(7,(quint16)nValue);       break;
                    case N_LE_DOS_HEADER::e_res2_8:        le.set_e_res2(8,(quint16)nValue);       break;
                    case N_LE_DOS_HEADER::e_res2_9:        le.set_e_res2(9,(quint16)nValue);       break;
                    case N_LE_DOS_HEADER::e_lfanew:        le.set_e_lfanew((quint32)nValue);       break;
                }

                ui->widgetHex_DOS_HEADER->reload();

                break;

            case SLE::TYPE_VXD_HEADER:
                switch(nNdata)
                {
                    case N_VXD_HEADER::e32_magic:       le.setImageVxdHeader_magic((quint16)nValue);        break;
                    case N_VXD_HEADER::e32_border:      le.setImageVxdHeader_border((quint8)nValue);        break;
                    case N_VXD_HEADER::e32_worder:      le.setImageVxdHeader_worder((quint8)nValue);        break;
                    case N_VXD_HEADER::e32_level:       le.setImageVxdHeader_level((quint32)nValue);        break;
                    case N_VXD_HEADER::e32_cpu:         le.setImageVxdHeader_cpu((quint16)nValue);          break;
                    case N_VXD_HEADER::e32_os:          le.setImageVxdHeader_os((quint16)nValue);           break;
                    case N_VXD_HEADER::e32_ver:         le.setImageVxdHeader_ver((quint32)nValue);          break;
                    case N_VXD_HEADER::e32_mflags:      le.setImageVxdHeader_mflags((quint32)nValue);       break;
                    case N_VXD_HEADER::e32_mpages:      le.setImageVxdHeader_mpages((quint32)nValue);       break;
                    case N_VXD_HEADER::e32_startobj:    le.setImageVxdHeader_startobj((quint32)nValue);     break;
                    case N_VXD_HEADER::e32_eip:         le.setImageVxdHeader_eip((quint32)nValue);          break;
                    case N_VXD_HEADER::e32_stackobj:    le.setImageVxdHeader_stackobj((quint32)nValue);     break;
                    case N_VXD_HEADER::e32_esp:         le.setImageVxdHeader_esp((quint32)nValue);          break;
                    case N_VXD_HEADER::e32_pagesize:    le.setImageVxdHeader_pagesize((quint32)nValue);     break;
                    case N_VXD_HEADER::e32_lastpagesize:le.setImageVxdHeader_lastpagesize((quint32)nValue); break;
                    case N_VXD_HEADER::e32_fixupsize:   le.setImageVxdHeader_fixupsize((quint32)nValue);    break;
                    case N_VXD_HEADER::e32_fixupsum:    le.setImageVxdHeader_fixupsum((quint32)nValue);     break;
                    case N_VXD_HEADER::e32_ldrsize:     le.setImageVxdHeader_ldrsize((quint32)nValue);      break;
                    case N_VXD_HEADER::e32_ldrsum:      le.setImageVxdHeader_ldrsum((quint32)nValue);       break;
                    case N_VXD_HEADER::e32_objtab:      le.setImageVxdHeader_objtab((quint32)nValue);       break;
                    case N_VXD_HEADER::e32_objcnt:      le.setImageVxdHeader_objcnt((quint32)nValue);       break;
                    case N_VXD_HEADER::e32_objmap:      le.setImageVxdHeader_objmap((quint32)nValue);       break;
                    case N_VXD_HEADER::e32_itermap:     le.setImageVxdHeader_itermap((quint32)nValue);      break;
                    case N_VXD_HEADER::e32_rsrctab:     le.setImageVxdHeader_rsrctab((quint32)nValue);      break;
                    case N_VXD_HEADER::e32_rsrccnt:     le.setImageVxdHeader_rsrccnt((quint32)nValue);      break;
                    case N_VXD_HEADER::e32_restab:      le.setImageVxdHeader_restab((quint32)nValue);       break;
                    case N_VXD_HEADER::e32_enttab:      le.setImageVxdHeader_enttab((quint32)nValue);       break;
                    case N_VXD_HEADER::e32_dirtab:      le.setImageVxdHeader_dirtab((quint32)nValue);       break;
                    case N_VXD_HEADER::e32_dircnt:      le.setImageVxdHeader_dircnt((quint32)nValue);       break;
                    case N_VXD_HEADER::e32_fpagetab:    le.setImageVxdHeader_fpagetab((quint32)nValue);     break;
                    case N_VXD_HEADER::e32_frectab:     le.setImageVxdHeader_frectab((quint32)nValue);      break;
                    case N_VXD_HEADER::e32_impmod:      le.setImageVxdHeader_impmod((quint32)nValue);       break;
                    case N_VXD_HEADER::e32_impmodcnt:   le.setImageVxdHeader_impmodcnt((quint32)nValue);    break;
                    case N_VXD_HEADER::e32_impproc:     le.setImageVxdHeader_impproc((quint32)nValue);      break;
                    case N_VXD_HEADER::e32_pagesum:     le.setImageVxdHeader_pagesum((quint32)nValue);      break;
                    case N_VXD_HEADER::e32_datapage:    le.setImageVxdHeader_datapage((quint32)nValue);     break;
                    case N_VXD_HEADER::e32_preload:     le.setImageVxdHeader_preload((quint32)nValue);      break;
                    case N_VXD_HEADER::e32_nrestab:     le.setImageVxdHeader_nrestab((quint32)nValue);      break;
                    case N_VXD_HEADER::e32_cbnrestab:   le.setImageVxdHeader_cbnrestab((quint32)nValue);    break;
                    case N_VXD_HEADER::e32_nressum:     le.setImageVxdHeader_nressum((quint32)nValue);      break;
                    case N_VXD_HEADER::e32_autodata:    le.setImageVxdHeader_autodata((quint32)nValue);     break;
                    case N_VXD_HEADER::e32_debuginfo:   le.setImageVxdHeader_debuginfo((quint32)nValue);    break;
                    case N_VXD_HEADER::e32_debuglen:    le.setImageVxdHeader_debuglen((quint32)nValue);     break;
                    case N_VXD_HEADER::e32_instpreload: le.setImageVxdHeader_instpreload((quint32)nValue);  break;
                    case N_VXD_HEADER::e32_instdemand:  le.setImageVxdHeader_instdemand((quint32)nValue);    break;
                    case N_VXD_HEADER::e32_heapsize:    le.setImageVxdHeader_heapsize((quint32)nValue);     break;
                }

                ui->widgetHex_VXD_HEADER->reload();

                break;
            }

            bResult=true;
        }
    }

    blockSignals(false);

    return bResult;
}

void LEWidget::setReadonly(bool bState)
{
    setLineEditsReadOnly(lineEdit_DOS_HEADER,N_LE_DOS_HEADER::__data_size,bState);
    setLineEditsReadOnly(lineEdit_VXD_HEADER,N_VXD_HEADER::__data_size,bState);

    setComboBoxesReadOnly(comboBox,__CB_size,bState);

    ui->widgetHex->setReadonly(bState);
}

void LEWidget::blockSignals(bool bState)
{
    _blockSignals((QObject **)lineEdit_DOS_HEADER,N_LE_DOS_HEADER::__data_size,bState);
    _blockSignals((QObject **)lineEdit_VXD_HEADER,N_VXD_HEADER::__data_size,bState);

    _blockSignals((QObject **)comboBox,__CB_size,bState);
}

void LEWidget::adjustHeaderTable(int type, QTableWidget *pTableWidget)
{
    Q_UNUSED(type)

    int nSymbolWidth=XLineEditHEX::getSymbolWidth(this);

    pTableWidget->horizontalHeader()->setSectionResizeMode(HEADER_COLUMN_NAME,QHeaderView::ResizeToContents);
    pTableWidget->horizontalHeader()->setSectionResizeMode(HEADER_COLUMN_OFFSET,QHeaderView::ResizeToContents);
    pTableWidget->horizontalHeader()->setSectionResizeMode(HEADER_COLUMN_TYPE,QHeaderView::ResizeToContents);
    pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE,nSymbolWidth*12);
    pTableWidget->setColumnWidth(HEADER_COLUMN_INFO,nSymbolWidth*15);
}

void LEWidget::reloadData()
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

    XLE le(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

    if(le.isValid())
    {
        if(nType==SLE::TYPE_HEX)
        {
            if(!stInit.contains(sInit))
            {
                ui->widgetHex->setData(getDevice());
                ui->widgetHex->setBackupFileName(getOptions()->sBackupFileName);
                ui->widgetHex->enableReadOnly(false);
                connect(ui->widgetHex,SIGNAL(editState(bool)),this,SLOT(setEdited(bool)));

                ui->widgetHex->reload();
            }
        }
        else if(nType==SLE::TYPE_STRINGS)
        {
            if(!stInit.contains(sInit))
            {
                ui->widgetStrings->setData(getDevice(),0,true);
            }
        }
        else if(nType==SLE::TYPE_MEMORYMAP)
        {
            if(!stInit.contains(sInit))
            {
                ui->widgetMemoryMap->setData(getDevice());
            }
        }
        else if(nType==SLE::TYPE_ENTROPY)
        {
            if(!stInit.contains(sInit))
            {
                ui->widgetEntropy->setData(getDevice(),0,getDevice()->size(),true);
            }
        }
        else if(nType==SLE::TYPE_HEURISTICSCAN)
        {
            if(!stInit.contains(sInit))
            {
                ui->widgetHeuristicScan->setData(getDevice(),true);
            }
        }
        else if(nType==SLE::TYPE_DOS_HEADER)
        {
            if(!stInit.contains(sInit))
            {
                createHeaderTable(SLE::TYPE_DOS_HEADER,ui->tableWidget_DOS_HEADER,N_LE_DOS_HEADER::records,lineEdit_DOS_HEADER,N_LE_DOS_HEADER::__data_size,0);
                comboBox[CB_DOS_HEADER_e_magic]=createComboBox(ui->tableWidget_DOS_HEADER,XMSDOS::getImageMagicsS(),SLE::TYPE_DOS_HEADER,N_LE_DOS_HEADER::e_magic,XComboBoxEx::CBTYPE_NORMAL);
                invWidget[INV_DOS_HEADER_e_lfanew]=createInvWidget(ui->tableWidget_DOS_HEADER,SLE::TYPE_DOS_HEADER,N_LE_DOS_HEADER::e_lfanew,InvWidget::TYPE_HEX);
                blockSignals(true);

                XMSDOS_DEF::IMAGE_DOS_HEADEREX msdosheaderex=le.getDosHeaderEx();

                lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_magic]->setValue(msdosheaderex.e_magic);
                lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_cblp]->setValue(msdosheaderex.e_cblp);
                lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_cp]->setValue(msdosheaderex.e_cp);
                lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_crlc]->setValue(msdosheaderex.e_crlc);
                lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_cparhdr]->setValue(msdosheaderex.e_cparhdr);
                lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_minalloc]->setValue(msdosheaderex.e_minalloc);
                lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_maxalloc]->setValue(msdosheaderex.e_maxalloc);
                lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_ss]->setValue(msdosheaderex.e_ss);
                lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_sp]->setValue(msdosheaderex.e_sp);
                lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_csum]->setValue(msdosheaderex.e_csum);
                lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_ip]->setValue(msdosheaderex.e_ip);
                lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_cs]->setValue(msdosheaderex.e_cs);
                lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_lfarlc]->setValue(msdosheaderex.e_lfarlc);
                lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_ovno]->setValue(msdosheaderex.e_ovno);
                lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_res_0]->setValue(msdosheaderex.e_res[0]);
                lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_res_1]->setValue(msdosheaderex.e_res[1]);
                lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_res_2]->setValue(msdosheaderex.e_res[2]);
                lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_res_3]->setValue(msdosheaderex.e_res[3]);
                lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_oemid]->setValue(msdosheaderex.e_oemid);
                lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_oeminfo]->setValue(msdosheaderex.e_oeminfo);
                lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_res2_0]->setValue(msdosheaderex.e_res2[0]);
                lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_res2_1]->setValue(msdosheaderex.e_res2[1]);
                lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_res2_2]->setValue(msdosheaderex.e_res2[2]);
                lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_res2_3]->setValue(msdosheaderex.e_res2[3]);
                lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_res2_4]->setValue(msdosheaderex.e_res2[4]);
                lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_res2_5]->setValue(msdosheaderex.e_res2[5]);
                lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_res2_6]->setValue(msdosheaderex.e_res2[6]);
                lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_res2_7]->setValue(msdosheaderex.e_res2[7]);
                lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_res2_8]->setValue(msdosheaderex.e_res2[8]);
                lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_res2_9]->setValue(msdosheaderex.e_res2[9]);
                lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_lfanew]->setValue(msdosheaderex.e_lfanew);

                comboBox[CB_DOS_HEADER_e_magic]->setValue(msdosheaderex.e_magic);

                invWidget[INV_DOS_HEADER_e_lfanew]->setOffsetAndSize(&le,msdosheaderex.e_lfanew,0);

                qint64 nOffset=le.getDosHeaderExOffset();
                qint64 nSize=le.getDosHeaderExSize();

                loadHexSubdevice(nOffset,nSize,nOffset,&subDevice[SLE::TYPE_DOS_HEADER],ui->widgetHex_DOS_HEADER);

                blockSignals(false);
            }
        }
        else if(nType==SLE::TYPE_VXD_HEADER)
        {
            if(!stInit.contains(sInit))
            {
                createHeaderTable(SLE::TYPE_VXD_HEADER,ui->tableWidget_VXD_HEADER,N_VXD_HEADER::records,lineEdit_VXD_HEADER,N_VXD_HEADER::__data_size,0);
                comboBox[CB_VXD_HEADER_e32_magic]=createComboBox(ui->tableWidget_VXD_HEADER,XLE::getImageLEMagicsS(),SLE::TYPE_VXD_HEADER,N_VXD_HEADER::e32_magic,XComboBoxEx::CBTYPE_NORMAL);
                comboBox[CB_VXD_HEADER_e32_cpu]=createComboBox(ui->tableWidget_VXD_HEADER,XLE::getImageLECpusS(),SLE::TYPE_VXD_HEADER,N_VXD_HEADER::e32_cpu,XComboBoxEx::CBTYPE_NORMAL);
                comboBox[CB_VXD_HEADER_e32_os]=createComboBox(ui->tableWidget_VXD_HEADER,XLE::getImageLEOssS(),SLE::TYPE_VXD_HEADER,N_VXD_HEADER::e32_os,XComboBoxEx::CBTYPE_NORMAL);

                blockSignals(true);

                XLE_DEF::IMAGE_VXD_HEADER vxdheader=le.getImageVxdHeader();

                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_magic]->setValue(vxdheader.e32_magic);
                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_border]->setValue(vxdheader.e32_border);
                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_worder]->setValue(vxdheader.e32_worder);
                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_level]->setValue(vxdheader.e32_level);
                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_cpu]->setValue(vxdheader.e32_cpu);
                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_os]->setValue(vxdheader.e32_os);
                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_ver]->setValue(vxdheader.e32_ver);
                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_mflags]->setValue(vxdheader.e32_mflags);
                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_mpages]->setValue(vxdheader.e32_mpages);
                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_startobj]->setValue(vxdheader.e32_startobj);
                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_eip]->setValue(vxdheader.e32_eip);
                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_stackobj]->setValue(vxdheader.e32_stackobj);
                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_esp]->setValue(vxdheader.e32_esp);
                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_pagesize]->setValue(vxdheader.e32_pagesize);
                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_lastpagesize]->setValue(vxdheader.e32_lastpagesize);
                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_fixupsize]->setValue(vxdheader.e32_fixupsize);
                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_fixupsum]->setValue(vxdheader.e32_fixupsum);
                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_ldrsize]->setValue(vxdheader.e32_ldrsize);
                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_ldrsum]->setValue(vxdheader.e32_ldrsum);
                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_objtab]->setValue(vxdheader.e32_objtab);
                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_objcnt]->setValue(vxdheader.e32_objcnt);
                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_objmap]->setValue(vxdheader.e32_objmap);
                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_itermap]->setValue(vxdheader.e32_itermap);
                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_rsrctab]->setValue(vxdheader.e32_rsrctab);
                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_rsrccnt]->setValue(vxdheader.e32_rsrccnt);
                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_restab]->setValue(vxdheader.e32_restab);
                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_enttab]->setValue(vxdheader.e32_enttab);
                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_dirtab]->setValue(vxdheader.e32_dirtab);
                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_dircnt]->setValue(vxdheader.e32_dircnt);
                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_fpagetab]->setValue(vxdheader.e32_fpagetab);
                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_frectab]->setValue(vxdheader.e32_frectab);
                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_impmod]->setValue(vxdheader.e32_impmod);
                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_impmodcnt]->setValue(vxdheader.e32_impmodcnt);
                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_impproc]->setValue(vxdheader.e32_impproc);
                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_pagesum]->setValue(vxdheader.e32_pagesum);
                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_datapage]->setValue(vxdheader.e32_datapage);
                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_preload]->setValue(vxdheader.e32_preload);
                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_nrestab]->setValue(vxdheader.e32_nrestab);
                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_cbnrestab]->setValue(vxdheader.e32_cbnrestab);
                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_nressum]->setValue(vxdheader.e32_nressum);
                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_autodata]->setValue(vxdheader.e32_autodata);
                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_debuginfo]->setValue(vxdheader.e32_debuginfo);
                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_debuglen]->setValue(vxdheader.e32_debuglen);
                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_instpreload]->setValue(vxdheader.e32_instpreload);
                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_instdemand]->setValue(vxdheader.e32_instdemand);
                lineEdit_VXD_HEADER[N_VXD_HEADER::e32_heapsize]->setValue(vxdheader.e32_heapsize);

                comboBox[CB_VXD_HEADER_e32_magic]->setValue(vxdheader.e32_magic);
                comboBox[CB_VXD_HEADER_e32_cpu]->setValue(vxdheader.e32_cpu);
                comboBox[CB_VXD_HEADER_e32_os]->setValue(vxdheader.e32_os);

                qint64 nOffset=le.getImageVxdHeaderOffset();
                qint64 nSize=le.getImageVxdHeaderSize();

                loadHexSubdevice(nOffset,nSize,nOffset,&subDevice[SLE::TYPE_VXD_HEADER],ui->widgetHex_VXD_HEADER);

                blockSignals(false);
            }
        }
        else if(nType==SLE::TYPE_OVERLAY)
        {
            qint64 nOverLayOffset=le.getOverlayOffset();
            qint64 nOverlaySize=le.getOverlaySize();

            loadHexSubdevice(nOverLayOffset,nOverlaySize,nOverLayOffset,&subDevice[SLE::TYPE_OVERLAY],ui->widgetHex_OVERLAY);
        }

        setReadonly(ui->checkBoxReadonly->isChecked());
    }

    stInit.insert(sInit);
}

void LEWidget::widgetValueChanged(quint64 nValue)
{
    QWidget *pWidget=qobject_cast<QWidget *>(sender());
    int nStype=pWidget->property("STYPE").toInt();
    int nNdata=pWidget->property("NDATA").toInt();

    switch(nStype)
    {
        case SLE::TYPE_DOS_HEADER:
            switch(nNdata)
            {
                case N_LE_DOS_HEADER::e_magic:     lineEdit_DOS_HEADER[N_LE_DOS_HEADER::e_magic]->setValue((quint16)nValue);      break;
            }
            break;
        case SLE::TYPE_VXD_HEADER:
            switch(nNdata)
            {
                case N_VXD_HEADER::e32_magic:   lineEdit_VXD_HEADER[N_VXD_HEADER::e32_magic]->setValue((quint16)nValue);    break;
                case N_VXD_HEADER::e32_cpu:     lineEdit_VXD_HEADER[N_VXD_HEADER::e32_cpu]->setValue((quint16)nValue);      break;
                case N_VXD_HEADER::e32_os:      lineEdit_VXD_HEADER[N_VXD_HEADER::e32_os]->setValue((quint16)nValue);       break;
            }
            break;
    }
}

void LEWidget::on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(previous)

    if(current)
    {
        reloadData();
    }
}

void LEWidget::on_checkBoxReadonly_toggled(bool checked)
{
    setReadonly(checked);
}

void LEWidget::on_pushButtonReload_clicked()
{
    ui->pushButtonReload->setEnabled(false);
    reload();

    QTimer::singleShot(1000,this,SLOT(enableButton()));
}

void LEWidget::enableButton()
{
    ui->pushButtonReload->setEnabled(true);
}

void LEWidget::on_tableWidget_DOS_HEADER_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(currentRow)
    Q_UNUSED(currentColumn)
    Q_UNUSED(previousRow)
    Q_UNUSED(previousColumn)

    setHeaderTableSelection(ui->widgetHex_DOS_HEADER,ui->tableWidget_DOS_HEADER);
}

void LEWidget::on_tableWidget_VXD_HEADER_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(currentRow)
    Q_UNUSED(currentColumn)
    Q_UNUSED(previousRow)
    Q_UNUSED(previousColumn)

    setHeaderTableSelection(ui->widgetHex_VXD_HEADER,ui->tableWidget_VXD_HEADER);
}

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
#include "newidget.h"
#include "ui_newidget.h"

NEWidget::NEWidget(QWidget *parent) :
    FormatWidget(parent),
    ui(new Ui::NEWidget)
{
    ui->setupUi(this);
}

NEWidget::NEWidget(QIODevice *pDevice, FW_DEF::OPTIONS *pOptions, QWidget *parent) :
    FormatWidget(pDevice,pOptions,0,parent),
    ui(new Ui::NEWidget)
{
    ui->setupUi(this);

    setData(pDevice,pOptions);
    reload();
}

NEWidget::~NEWidget()
{
    delete ui;
}

void NEWidget::clear()
{
    memset(bInit,0,sizeof bInit);
    memset(lineEdit_DOS_HEADER,0,sizeof lineEdit_DOS_HEADER);
    memset(comboBox,0,sizeof comboBox);
    memset(subDevice,0,sizeof subDevice);

    ui->checkBoxReadonly->setChecked(true);

    ui->treeWidgetNavi->clear();
}

void NEWidget::setData(QIODevice *pDevice, FW_DEF::OPTIONS *pOptions)
{
    FormatWidget::setData(pDevice,pOptions,0);
}

void NEWidget::reload()
{
    clear();

    ui->checkBoxReadonly->setEnabled(!isReadonly());

    XNE ne(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

    if(ne.isValid())
    {
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SNE::TYPE_TOOLS,tr("Tools")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SNE::TYPE_DOS_HEADER,"DOS_HEADER"));

        if(ne.isOverlayPresent())
        {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SNE::TYPE_OVERLAY,tr("Overlay")));
        }

        ui->treeWidgetNavi->expandAll();

        ui->treeWidgetNavi->setCurrentItem(ui->treeWidgetNavi->topLevelItem(1));
    }
}

bool NEWidget::_setValue(QVariant vValue, int nStype, int nNdata, int nVtype, int nPosition)
{
    Q_UNUSED(nVtype)
    Q_UNUSED(nPosition)

    bool bResult=false;

    blockSignals(true);

    quint64 nValue=vValue.toULongLong();

    if(getDevice()->isWritable())
    {
        XNE ne(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

        if(ne.isValid())
        {
            switch(nStype)
            {
                case SNE::TYPE_DOS_HEADER:
                    switch(nNdata)
                    {
                    case N_DOS_HEADER::e_magic:
                        comboBox[CB_DOS_HEADER_e_magic]->setValue(nValue);
                        ne.set_e_magic((quint16)nValue);
                        break;

                    case N_DOS_HEADER::e_cblp:
                        ne.set_e_cblp((quint16)nValue);
                        break;

                    case N_DOS_HEADER::e_cp:
                        ne.set_e_cp((quint16)nValue);
                        break;

                    case N_DOS_HEADER::e_crlc:
                        ne.set_e_crlc((quint16)nValue);
                        break;

                    case N_DOS_HEADER::e_cparhdr:
                        ne.set_e_cparhdr((quint16)nValue);
                        break;

                    case N_DOS_HEADER::e_minalloc:
                        ne.set_e_minalloc((quint16)nValue);
                        break;

                    case N_DOS_HEADER::e_maxalloc:
                        ne.set_e_maxalloc((quint16)nValue);
                        break;

                    case N_DOS_HEADER::e_ss:
                        ne.set_e_ss((quint16)nValue);
                        break;

                    case N_DOS_HEADER::e_sp:
                        ne.set_e_sp((quint16)nValue);
                        break;

                    case N_DOS_HEADER::e_csum:
                        ne.set_e_csum((quint16)nValue);
                        break;

                    case N_DOS_HEADER::e_ip:
                        ne.set_e_ip((quint16)nValue);
                        break;

                    case N_DOS_HEADER::e_cs:
                        ne.set_e_cs((quint16)nValue);
                        break;

                    case N_DOS_HEADER::e_lfarlc:
                        ne.set_e_lfarlc((quint16)nValue);
                        break;

                    case N_DOS_HEADER::e_ovno:
                        ne.set_e_ovno((quint16)nValue);
                        break;

                    case N_DOS_HEADER::e_res_0:
                        ne.set_e_res(0,(quint16)nValue);
                        break;

                    case N_DOS_HEADER::e_res_1:
                        ne.set_e_res(1,(quint16)nValue);
                        break;

                    case N_DOS_HEADER::e_res_2:
                        ne.set_e_res(2,(quint16)nValue);
                        break;

                    case N_DOS_HEADER::e_res_3:
                        ne.set_e_res(3,(quint16)nValue);
                        break;

                    case N_DOS_HEADER::e_oemid:
                        ne.set_e_oemid((quint16)nValue);
                        break;

                    case N_DOS_HEADER::e_oeminfo:
                        ne.set_e_oeminfo((quint16)nValue);
                        break;

                    case N_DOS_HEADER::e_res2_0:
                        ne.set_e_res2(0,(quint16)nValue);
                        break;

                    case N_DOS_HEADER::e_res2_1:
                        ne.set_e_res2(1,(quint16)nValue);
                        break;

                    case N_DOS_HEADER::e_res2_2:
                        ne.set_e_res2(2,(quint16)nValue);
                        break;

                    case N_DOS_HEADER::e_res2_3:
                        ne.set_e_res2(3,(quint16)nValue);
                        break;

                    case N_DOS_HEADER::e_res2_4:
                        ne.set_e_res2(4,(quint16)nValue);
                        break;

                    case N_DOS_HEADER::e_res2_5:
                        ne.set_e_res2(5,(quint16)nValue);
                        break;

                    case N_DOS_HEADER::e_res2_6:
                        ne.set_e_res2(6,(quint16)nValue);
                        break;

                    case N_DOS_HEADER::e_res2_7:
                        ne.set_e_res2(7,(quint16)nValue);
                        break;

                    case N_DOS_HEADER::e_res2_8:
                        ne.set_e_res2(8,(quint16)nValue);
                        break;

                    case N_DOS_HEADER::e_res2_9:
                        ne.set_e_res2(9,(quint16)nValue);
                        break;

                    case N_DOS_HEADER::e_lfanew:
                        ne.set_e_lfanew((quint32)nValue);
                        break;
                    }

                    break;
            }

            bResult=true;
        }
    }

    blockSignals(false);

    return bResult;
}

void NEWidget::setReadonly(bool bState)
{
    setLineEditsReadOnly(lineEdit_DOS_HEADER,N_DOS_HEADER::__data_size,bState);

    setComboBoxesReadOnly(comboBox,__CB_size,bState);

    ui->widgetHex->setReadonly(bState);
}

void NEWidget::blockSignals(bool bState)
{
    _blockSignals((QObject **)lineEdit_DOS_HEADER,N_DOS_HEADER::__data_size,bState);

    _blockSignals((QObject **)comboBox,__CB_size,bState);
}

void NEWidget::adjustHeaderTable(int type, QTableWidget *pTableWidget)
{
    int nSymbolWidth=getSymbolWidth();

    switch(type)
    {
        case SNE::TYPE_DOS_HEADER:
            pTableWidget->setColumnWidth(0,nSymbolWidth*10);
            pTableWidget->setColumnWidth(1,nSymbolWidth*13);
            pTableWidget->setColumnWidth(2,nSymbolWidth*13);
            pTableWidget->setColumnWidth(3,nSymbolWidth*13);
            break;
    }
}

void NEWidget::reloadData()
{
    int nData=ui->treeWidgetNavi->currentItem()->data(0,Qt::UserRole).toInt();
    ui->stackedWidgetInfo->setCurrentIndex(nData);

    XNE ne(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

    if(ne.isValid())
    {
        if(nData==SNE::TYPE_TOOLS)
        {
            if(!bInit[nData])
            {
                ui->widgetHex->setData(getDevice(),getOptions());
                ui->widgetHex->setEdited(isEdited());
                connect(ui->widgetHex,SIGNAL(editState(bool)),this,SLOT(setEdited(bool)));

                bInit[nData]=true;
            }
            ui->widgetHex->reload();
        }
        else if(nData==SNE::TYPE_DOS_HEADER)
        {
            if(!bInit[nData])
            {
                bInit[nData]=createHeaderTable(SNE::TYPE_DOS_HEADER,ui->tableWidget_DOS_HEADER,N_DOS_HEADER::records,lineEdit_DOS_HEADER,N_DOS_HEADER::__data_size,0);
                comboBox[CB_DOS_HEADER_e_magic]=createComboBox(ui->tableWidget_DOS_HEADER,XMSDOS::getImageMagicsS(),SNE::TYPE_DOS_HEADER,N_DOS_HEADER::e_magic,XComboBoxEx::CBTYPE_NORMAL);
            }

            blockSignals(true);

            XMSDOS_DEF::IMAGE_DOS_HEADEREX msdosheaderex=ne.getDosHeaderEx();

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
            lineEdit_DOS_HEADER[N_DOS_HEADER::e_res_0]->setValue(msdosheaderex.e_res[0]);
            lineEdit_DOS_HEADER[N_DOS_HEADER::e_res_1]->setValue(msdosheaderex.e_res[1]);
            lineEdit_DOS_HEADER[N_DOS_HEADER::e_res_2]->setValue(msdosheaderex.e_res[2]);
            lineEdit_DOS_HEADER[N_DOS_HEADER::e_res_3]->setValue(msdosheaderex.e_res[3]);
            lineEdit_DOS_HEADER[N_DOS_HEADER::e_oemid]->setValue(msdosheaderex.e_oemid);
            lineEdit_DOS_HEADER[N_DOS_HEADER::e_oeminfo]->setValue(msdosheaderex.e_oeminfo);
            lineEdit_DOS_HEADER[N_DOS_HEADER::e_res2_0]->setValue(msdosheaderex.e_res2[0]);
            lineEdit_DOS_HEADER[N_DOS_HEADER::e_res2_1]->setValue(msdosheaderex.e_res2[1]);
            lineEdit_DOS_HEADER[N_DOS_HEADER::e_res2_2]->setValue(msdosheaderex.e_res2[2]);
            lineEdit_DOS_HEADER[N_DOS_HEADER::e_res2_3]->setValue(msdosheaderex.e_res2[3]);
            lineEdit_DOS_HEADER[N_DOS_HEADER::e_res2_4]->setValue(msdosheaderex.e_res2[4]);
            lineEdit_DOS_HEADER[N_DOS_HEADER::e_res2_5]->setValue(msdosheaderex.e_res2[5]);
            lineEdit_DOS_HEADER[N_DOS_HEADER::e_res2_6]->setValue(msdosheaderex.e_res2[6]);
            lineEdit_DOS_HEADER[N_DOS_HEADER::e_res2_7]->setValue(msdosheaderex.e_res2[7]);
            lineEdit_DOS_HEADER[N_DOS_HEADER::e_res2_8]->setValue(msdosheaderex.e_res2[8]);
            lineEdit_DOS_HEADER[N_DOS_HEADER::e_res2_9]->setValue(msdosheaderex.e_res2[9]);
            lineEdit_DOS_HEADER[N_DOS_HEADER::e_lfanew]->setValue(msdosheaderex.e_lfanew);

            comboBox[CB_DOS_HEADER_e_magic]->setValue(msdosheaderex.e_magic);

            blockSignals(false);
        }
        else if(nData==SNE::TYPE_OVERLAY)
        {
            qint64 nOverLayOffset=ne.getOverlayOffset();
            qint64 nOverlaySize=ne.getOverlaySize();

            loadHexSubdevice(nOverLayOffset,nOverlaySize,nOverLayOffset,&subDevice[SD_OVERLAY],ui->widgetOverlayHex);
        }

        setReadonly(ui->checkBoxReadonly->isChecked());
    }
}

void NEWidget::widgetValueChanged(quint64 nValue)
{
    QWidget *pWidget=qobject_cast<QWidget *>(sender());
    int nStype=pWidget->property("STYPE").toInt();
    int nNdata=pWidget->property("NDATA").toInt();

    switch(nStype)
    {
        case SNE::TYPE_DOS_HEADER:
            switch(nNdata)
            {
                case N_DOS_HEADER::e_magic:
                    lineEdit_DOS_HEADER[N_DOS_HEADER::e_magic]->setValue((quint16)nValue);
                    break;
            }

            break;
    }
}

void NEWidget::on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(previous)

    if(current)
    {
        reloadData();
    }
}

void NEWidget::on_checkBoxReadonly_toggled(bool checked)
{
    setReadonly(checked);
}

void NEWidget::on_pushButtonReload_clicked()
{
    reload();
}

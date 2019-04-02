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
#include "msdoswidget.h"
#include "ui_msdoswidget.h"

MSDOSWidget::MSDOSWidget(QWidget *parent) :
    FormatWidget(parent),
    ui(new Ui::MSDOSWidget)
{
    ui->setupUi(this);
}

MSDOSWidget::MSDOSWidget(QIODevice *pDevice, OPTIONS *pOptions, QWidget *parent) :
    FormatWidget(pDevice,pOptions,parent),
    ui(new Ui::MSDOSWidget)
{
    ui->setupUi(this);

    setData(pDevice,pOptions);
}

MSDOSWidget::~MSDOSWidget()
{
    delete ui;
}

void MSDOSWidget::setData(QIODevice *pDevice, FormatWidget::OPTIONS *pOptions)
{
    FormatWidget::setData(pDevice,pOptions);

    memset(bInit,0,sizeof bInit);
    memset(lineEdit_DOS_HEADER,0,sizeof lineEdit_DOS_HEADER);
    memset(comboBox,0,sizeof comboBox);

    ui->checkBoxReadonly->setChecked(true);

    QMSDOS msdos(pDevice,getOptions()->bIsImage,getOptions()->nImageAddress);

    if(msdos.isValid())
    {
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMSDOS::TYPE_DOS_HEADER,"DOS_HEADER"));

        // TODO Sections
        // TODO Programs

        ui->treeWidgetNavi->expandAll();

        ui->treeWidgetNavi->setCurrentItem(ui->treeWidgetNavi->topLevelItem(0));
    }
}

bool MSDOSWidget::_setValue(QVariant vValue, int nStype, int nNdata, int nVtype, int nPosition)
{
    bool bResult=false;

    blockSignals(true);

    quint64 nValue=vValue.toULongLong();

    if(getDevice()->isWritable())
    {
        QMSDOS msdos(getDevice(),getOptions()->bIsImage,getOptions()->nImageAddress);
        if(msdos.isValid())
        {
            switch(nStype)
            {
            case SMSDOS::TYPE_DOS_HEADER:
                switch(nNdata)
                {
                case N_DOS_HEADER::e_magic:
                    comboBox[CB_DOS_HEADER_e_magic]->setValue(nValue);
                    msdos.set_e_magic((quint16)nValue);
                    break;
                case N_DOS_HEADER::e_cblp:
                    msdos.set_e_cblp((quint16)nValue);
                    break;
                case N_DOS_HEADER::e_cp:
                    msdos.set_e_cp((quint16)nValue);
                    break;
                case N_DOS_HEADER::e_crlc:
                    msdos.set_e_crlc((quint16)nValue);
                    break;
                case N_DOS_HEADER::e_cparhdr:
                    msdos.set_e_cparhdr((quint16)nValue);
                    break;
                case N_DOS_HEADER::e_minalloc:
                    msdos.set_e_minalloc((quint16)nValue);
                    break;
                case N_DOS_HEADER::e_maxalloc:
                    msdos.set_e_maxalloc((quint16)nValue);
                    break;
                case N_DOS_HEADER::e_ss:
                    msdos.set_e_ss((quint16)nValue);
                    break;
                case N_DOS_HEADER::e_sp:
                    msdos.set_e_sp((quint16)nValue);
                    break;
                case N_DOS_HEADER::e_csum:
                    msdos.set_e_csum((quint16)nValue);
                    break;
                case N_DOS_HEADER::e_ip:
                    msdos.set_e_ip((quint16)nValue);
                    break;
                case N_DOS_HEADER::e_cs:
                    msdos.set_e_cs((quint16)nValue);
                    break;
                case N_DOS_HEADER::e_lfarlc:
                    msdos.set_e_lfarlc((quint16)nValue);
                    break;
                case N_DOS_HEADER::e_ovno:
                    msdos.set_e_ovno((quint16)nValue);
                    break;
                case N_DOS_HEADER::e_res_0:
                    msdos.set_e_res(0,(quint16)nValue);
                    break;
                case N_DOS_HEADER::e_res_1:
                    msdos.set_e_res(1,(quint16)nValue);
                    break;
                case N_DOS_HEADER::e_res_2:
                    msdos.set_e_res(2,(quint16)nValue);
                    break;
                case N_DOS_HEADER::e_res_3:
                    msdos.set_e_res(3,(quint16)nValue);
                    break;
                case N_DOS_HEADER::e_oemid:
                    msdos.set_e_oemid((quint16)nValue);
                    break;
                case N_DOS_HEADER::e_oeminfo:
                    msdos.set_e_oeminfo((quint16)nValue);
                    break;
                case N_DOS_HEADER::e_res2_0:
                    msdos.set_e_res2(0,(quint16)nValue);
                    break;
                case N_DOS_HEADER::e_res2_1:
                    msdos.set_e_res2(1,(quint16)nValue);
                    break;
                case N_DOS_HEADER::e_res2_2:
                    msdos.set_e_res2(2,(quint16)nValue);
                    break;
                case N_DOS_HEADER::e_res2_3:
                    msdos.set_e_res2(3,(quint16)nValue);
                    break;
                case N_DOS_HEADER::e_res2_4:
                    msdos.set_e_res2(4,(quint16)nValue);
                    break;
                case N_DOS_HEADER::e_res2_5:
                    msdos.set_e_res2(5,(quint16)nValue);
                    break;
                case N_DOS_HEADER::e_res2_6:
                    msdos.set_e_res2(6,(quint16)nValue);
                    break;
                case N_DOS_HEADER::e_res2_7:
                    msdos.set_e_res2(7,(quint16)nValue);
                    break;
                case N_DOS_HEADER::e_res2_8:
                    msdos.set_e_res2(8,(quint16)nValue);
                    break;
                case N_DOS_HEADER::e_res2_9:
                    msdos.set_e_res2(9,(quint16)nValue);
                    break;
                case N_DOS_HEADER::e_lfanew:
                    msdos.set_e_lfanew((quint32)nValue);
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

void MSDOSWidget::setReadonly(bool bState)
{
    setLineEditsReadOnly(lineEdit_DOS_HEADER,N_DOS_HEADER::__data_size,bState);

    setComboBoxesReadOnly(comboBox,__CB_size,bState);
}

void MSDOSWidget::blockSignals(bool bState)
{
    _blockSignals((QObject **)lineEdit_DOS_HEADER,N_DOS_HEADER::__data_size,bState);

    _blockSignals((QObject **)comboBox,__CB_size,bState);
}

void MSDOSWidget::adjustHeaderTable(int type, QTableWidget *pTableWidget)
{
    int nSymbolWidth=getSymbolWidth();

    switch(type)
    {
    case SMSDOS::TYPE_DOS_HEADER:
        pTableWidget->setColumnWidth(0,nSymbolWidth*10);
        pTableWidget->setColumnWidth(1,nSymbolWidth*13);
        pTableWidget->setColumnWidth(2,nSymbolWidth*13);
        pTableWidget->setColumnWidth(3,nSymbolWidth*13);
        break;
    }
}

void MSDOSWidget::reloadData()
{
    int nData=ui->treeWidgetNavi->currentItem()->data(0,Qt::UserRole).toInt();
    ui->stackedWidgetInfo->setCurrentIndex(nData);

    QMSDOS msdos(getDevice(),getOptions()->bIsImage,getOptions()->nImageAddress);
    if(msdos.isValid())
    {
        if(nData==SMSDOS::TYPE_DOS_HEADER)
        {
            if(!bInit[nData])
            {
                bInit[nData]=createHeaderTable(SMSDOS::TYPE_DOS_HEADER,ui->tableWidget_DOS_HEADER,N_DOS_HEADER::records,lineEdit_DOS_HEADER,N_DOS_HEADER::__data_size,0);
                comboBox[CB_DOS_HEADER_e_magic]=createComboBox(ui->tableWidget_DOS_HEADER,QMSDOS::getImageMagicsS(),SMSDOS::TYPE_DOS_HEADER,N_DOS_HEADER::e_magic,QComboBoxEx::CBTYPE_NORMAL);
            }

            blockSignals(true);

            S_IMAGE_DOS_HEADEREX msdosheaderex=msdos.getDosHeaderEx();

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

        setReadonly(ui->checkBoxReadonly->isChecked());
    }
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
            lineEdit_DOS_HEADER[N_DOS_HEADER::e_magic]->setValue((quint8)nValue);
            break;
        }
        break;
    }
}

void MSDOSWidget::on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    reloadData();
}

void MSDOSWidget::on_checkBoxReadonly_toggled(bool checked)
{
    setReadonly(checked);
}

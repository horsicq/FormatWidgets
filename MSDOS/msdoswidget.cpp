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

MSDOSWidget::MSDOSWidget(QIODevice *pDevice, FW_DEF::OPTIONS *pOptions, QWidget *parent) :
    FormatWidget(pDevice,pOptions,0,parent),
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
    memset(bInit,0,sizeof bInit);
    memset(lineEdit_DOS_HEADER,0,sizeof lineEdit_DOS_HEADER);
    memset(comboBox,0,sizeof comboBox);
    memset(subDevice,0,sizeof subDevice);

    ui->checkBoxReadonly->setChecked(true);

    ui->treeWidgetNavi->clear();
}

void MSDOSWidget::setData(QIODevice *pDevice, FW_DEF::OPTIONS *pOptions)
{
    FormatWidget::setData(pDevice,pOptions,0);
}

void MSDOSWidget::reload()
{
    clear();

    ui->checkBoxReadonly->setEnabled(!isReadonly());

    XMSDOS msdos(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

    if(msdos.isValid())
    {
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMSDOS::TYPE_TOOLS,tr("Tools")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMSDOS::TYPE_DOS_HEADER,"DOS_HEADER"));

        if(msdos.isOverlayPresent())
        {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMSDOS::TYPE_OVERLAY,tr("Overlay")));
        }

        ui->treeWidgetNavi->expandAll();

        ui->treeWidgetNavi->setCurrentItem(ui->treeWidgetNavi->topLevelItem(1));
    }
}

bool MSDOSWidget::_setValue(QVariant vValue, int nStype, int nNdata, int nVtype, int nPosition)
{
    Q_UNUSED(nVtype)
    Q_UNUSED(nPosition)

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

    XMSDOS msdos(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

    if(msdos.isValid())
    {
        if(nData==SMSDOS::TYPE_TOOLS)
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
        else if(nData==SMSDOS::TYPE_DOS_HEADER)
        {
            if(!bInit[nData])
            {
                bInit[nData]=createHeaderTable(SMSDOS::TYPE_DOS_HEADER,ui->tableWidget_DOS_HEADER,N_DOS_HEADER::records,lineEdit_DOS_HEADER,N_DOS_HEADER::__data_size,0);
                comboBox[CB_DOS_HEADER_e_magic]=createComboBox(ui->tableWidget_DOS_HEADER,XMSDOS::getImageMagicsS(),SMSDOS::TYPE_DOS_HEADER,N_DOS_HEADER::e_magic,XComboBoxEx::CBTYPE_NORMAL);
            }

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
        else if(nData==SMSDOS::TYPE_OVERLAY)
        {
            qint64 nOverLayOffset=msdos.getOverlayOffset();
            qint64 nOverlaySize=msdos.getOverlaySize();

            loadHexSubdevice(nOverLayOffset,nOverlaySize,nOverLayOffset,&subDevice[SMSDOS::TYPE_OVERLAY],ui->widgetHex_OVERLAY);
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

void MSDOSWidget::on_checkBoxReadonly_toggled(bool checked)
{
    setReadonly(checked);
}

void MSDOSWidget::on_pushButtonReload_clicked()
{
    reload();
}

void MSDOSWidget::on_tableWidget_DOS_HEADER_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(currentRow);
    Q_UNUSED(currentColumn);
    Q_UNUSED(previousRow);
    Q_UNUSED(previousColumn);

    setHeaderTableSelection(ui->widgetHex_DOS_HEADER,ui->tableWidget_DOS_HEADER);
}

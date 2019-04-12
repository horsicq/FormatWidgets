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
#include "machwidget.h"
#include "ui_machwidget.h"

MACHWidget::MACHWidget(QWidget *parent) :
    FormatWidget(parent),
    ui(new Ui::MACHWidget)
{
    ui->setupUi(this);
}

MACHWidget::MACHWidget(QIODevice *pDevice, OPTIONS *pOptions, QWidget *parent) :
    FormatWidget(pDevice,pOptions,parent),
    ui(new Ui::MACHWidget)
{
    ui->setupUi(this);

    setData(pDevice,pOptions);
}

MACHWidget::~MACHWidget()
{
    delete ui;
}

void MACHWidget::setData(QIODevice *pDevice, FormatWidget::OPTIONS *pOptions)
{
    FormatWidget::setData(pDevice,pOptions);

    memset(bInit,0,sizeof bInit);
    memset(lineEdit_mach_header,0,sizeof lineEdit_mach_header);
    memset(comboBox,0,sizeof comboBox);

    ui->checkBoxReadonly->setChecked(true);

    ui->widgetHex->enableHeader(true);
    ui->widgetHex->enableReadOnly(false);

    XMACH mach(pDevice,getOptions()->bIsImage,getOptions()->nImageAddress);

    if(mach.isValid())
    {
        // mb TODO 32/64
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMACH::TYPE_HEX,"HEX"));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMACH::TYPE_mach_header,"mach_header"));

        // TODO Sections
        // TODO Programs

        ui->treeWidgetNavi->expandAll();

        ui->treeWidgetNavi->setCurrentItem(ui->treeWidgetNavi->topLevelItem(1));
    }
}

bool MACHWidget::_setValue(QVariant vValue, int nStype, int nNdata, int nVtype, int nPosition)
{
    bool bResult=false;

    blockSignals(true);

    quint64 nValue=vValue.toULongLong();

    if(getDevice()->isWritable())
    {
        XMACH msdos(getDevice(),getOptions()->bIsImage,getOptions()->nImageAddress);
        if(msdos.isValid())
        {
            switch(nStype)
            {

            }

            bResult=true;
        }
    }

    blockSignals(false);

    return bResult;
}

void MACHWidget::setReadonly(bool bState)
{
    setLineEditsReadOnly(lineEdit_mach_header,N_mach_header::__data_size,bState);

    setComboBoxesReadOnly(comboBox,__CB_size,bState);

    ui->widgetHex->setReadonly(bState);
}

void MACHWidget::blockSignals(bool bState)
{
    _blockSignals((QObject **)lineEdit_mach_header,N_mach_header::__data_size,bState);

    _blockSignals((QObject **)comboBox,__CB_size,bState);
}

void MACHWidget::adjustHeaderTable(int type, QTableWidget *pTableWidget)
{
    int nSymbolWidth=getSymbolWidth();

    switch(type)
    {
    case SMACH::TYPE_mach_header:
        pTableWidget->setColumnWidth(0,nSymbolWidth*10);
        pTableWidget->setColumnWidth(1,nSymbolWidth*13);
        pTableWidget->setColumnWidth(2,nSymbolWidth*13);
        pTableWidget->setColumnWidth(3,nSymbolWidth*13);
        break;
    }
}

void MACHWidget::reloadData()
{
    int nData=ui->treeWidgetNavi->currentItem()->data(0,Qt::UserRole).toInt();
    ui->stackedWidgetInfo->setCurrentIndex(nData);

    XMACH mach(getDevice(),getOptions()->bIsImage,getOptions()->nImageAddress);
    if(mach.isValid())
    {
        if(nData==SMACH::TYPE_HEX)
        {
            if(!bInit[nData])
            {
                QHexView::OPTIONS options={0};

                options.nBaseAddress=getOptions()->nImageAddress;
                options.sBackupFileName=getOptions()->sBackupFileName;

                ui->widgetHex->setData(getDevice(),&options);

                bInit[nData]=true;
            }
        }
        else if(nData==SMACH::TYPE_mach_header)
        {
            if(!bInit[nData])
            {
                if(!mach.is64())
                {
                    bInit[nData]=createHeaderTable(SMACH::TYPE_mach_header,ui->tableWidget_mach_header,N_mach_header::records32,lineEdit_mach_header,N_mach_header::__data_size-1,0);
                }
                else
                {
                    bInit[nData]=createHeaderTable(SMACH::TYPE_mach_header,ui->tableWidget_mach_header,N_mach_header::records64,lineEdit_mach_header,N_mach_header::__data_size,0);
                }

//                comboBox[CB_DOS_HEADER_e_magic]=createComboBox(ui->tableWidget_DOS_HEADER,XMACH::getImageMagicsS(),SMACH::TYPE_DOS_HEADER,N_DOS_HEADER::e_magic,QComboBoxEx::CBTYPE_NORMAL);
            }

            blockSignals(true);

            // TODO

            blockSignals(false);
        }

        setReadonly(ui->checkBoxReadonly->isChecked());
    }
}

void MACHWidget::widgetValueChanged(quint64 nValue)
{
    QWidget *pWidget=qobject_cast<QWidget *>(sender());
    int nStype=pWidget->property("STYPE").toInt();
    int nNdata=pWidget->property("NDATA").toInt();

//    switch(nStype)
//    {
//    case SMACH::TYPE_DOS_HEADER:
//        switch(nNdata)
//        {
//        case N_DOS_HEADER::e_magic:
//            lineEdit_DOS_HEADER[N_DOS_HEADER::e_magic]->setValue((quint8)nValue);
//            break;
//        }
//        break;
//    }
}

void MACHWidget::on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    reloadData();
}

void MACHWidget::on_checkBoxReadonly_toggled(bool checked)
{
    setReadonly(checked);
}

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
#include "binarywidget.h"
#include "ui_binarywidget.h"

BinaryWidget::BinaryWidget(QWidget *parent) :
    FormatWidget(parent),
    ui(new Ui::BinaryWidget)
{
    ui->setupUi(this);
}

BinaryWidget::BinaryWidget(QIODevice *pDevice, OPTIONS *pOptions, QWidget *parent) :
    FormatWidget(pDevice,pOptions,parent),
    ui(new Ui::BinaryWidget)
{
    ui->setupUi(this);

    setData(pDevice,pOptions);
}

BinaryWidget::~BinaryWidget()
{
    delete ui;
}

void BinaryWidget::clear()
{
    memset(bInit,0,sizeof bInit);

    ui->checkBoxReadonly->setChecked(true);

    ui->widgetHex->enableHeader(true);
    ui->widgetHex->enableReadOnly(false);

    ui->treeWidgetNavi->clear();
}

void BinaryWidget::setData(QIODevice *pDevice, FormatWidget::OPTIONS *pOptions)
{
    FormatWidget::setData(pDevice,pOptions);

    XBinary binary(pDevice,getOptions()->bIsImage,getOptions()->nImageAddress);

    if(binary.isValid())
    {
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SBinary::TYPE_HEX,"HEX"));

        ui->treeWidgetNavi->expandAll();

        ui->treeWidgetNavi->setCurrentItem(ui->treeWidgetNavi->topLevelItem(0));
    }
}

bool BinaryWidget::_setValue(QVariant vValue, int nStype, int nNdata, int nVtype, int nPosition)
{
    bool bResult=false;

    blockSignals(true);

    quint64 nValue=vValue.toULongLong();

    if(getDevice()->isWritable())
    {

    }

    blockSignals(false);

    return bResult;
}

void BinaryWidget::setReadonly(bool bState)
{
    ui->widgetHex->setReadonly(bState);
}

void BinaryWidget::blockSignals(bool bState)
{

}

void BinaryWidget::adjustHeaderTable(int type, QTableWidget *pTableWidget)
{
    int nSymbolWidth=getSymbolWidth();
}

void BinaryWidget::reloadData()
{
    int nData=ui->treeWidgetNavi->currentItem()->data(0,Qt::UserRole).toInt();
    ui->stackedWidgetInfo->setCurrentIndex(nData);

    XBinary binary(getDevice(),getOptions()->bIsImage,getOptions()->nImageAddress);
    if(binary.isValid())
    {
        if(nData==SBinary::TYPE_HEX)
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

        setReadonly(ui->checkBoxReadonly->isChecked());
    }
}

void BinaryWidget::widgetValueChanged(quint64 nValue)
{
    QWidget *pWidget=qobject_cast<QWidget *>(sender());
    int nStype=pWidget->property("STYPE").toInt();
    int nNdata=pWidget->property("NDATA").toInt();
}

void BinaryWidget::on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    reloadData();
}

void BinaryWidget::on_checkBoxReadonly_toggled(bool checked)
{
    setReadonly(checked);
}

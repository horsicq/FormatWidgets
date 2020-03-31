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
#include "debugheaderwidget.h"
#include "ui_debugheaderwidget.h"

DebugHeaderWidget::DebugHeaderWidget(QWidget *parent) :
    FormatWidget(parent),
    ui(new Ui::DebugHeaderWidget)
{
    ui->setupUi(this);
}

DebugHeaderWidget::DebugHeaderWidget(QIODevice *pDevice, FW_DEF::OPTIONS *pOptions, quint32 nNumber, qint64 nOffset, QWidget *parent)
{
    ui->setupUi(this);

    setData(pDevice,pOptions,nNumber,nOffset);
}

DebugHeaderWidget::~DebugHeaderWidget()
{
    delete ui;
}

void DebugHeaderWidget::clear()
{
    bInit=false;

    memset(lineEdit_DEBUG,0,sizeof lineEdit_DEBUG);
    memset(invWidget,0,sizeof invWidget);

    pSubDevice=nullptr;

    ui->checkBoxReadonly->setChecked(true);
}

void DebugHeaderWidget::setData(QIODevice *pDevice, FW_DEF::OPTIONS *pOptions, quint32 nNumber, qint64 nOffset)
{
    FormatWidget::setData(pDevice,pOptions,nNumber,nOffset);
}

void DebugHeaderWidget::reload()
{
    clear();

    ui->checkBoxReadonly->setEnabled(!isReadonly());

    reloadData();
}

bool DebugHeaderWidget::_setValue(QVariant vValue, int nStype, int nNdata, int nVtype, int nPosition, qint64 nOffset)
{
    Q_UNUSED(nVtype)
    Q_UNUSED(nOffset)

    bool bResult=false;

    quint64 nValue=vValue.toULongLong();

    if(getDevice()->isWritable())
    {
        XPE pe(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

        if(pe.isValid())
        {

            bResult=true;
        }
    }

    return bResult;
}

void DebugHeaderWidget::setReadonly(bool bState)
{
    setLineEditsReadOnly(lineEdit_DEBUG,N_IMAGE_SECTION_HEADER::__data_size,bState);
}

void DebugHeaderWidget::blockSignals(bool bState)
{
    _blockSignals((QObject **)lineEdit_DEBUG,N_IMAGE_SECTION_HEADER::__data_size,bState);

//    _blockSignals((QObject **)comboBox,__CB_size,bState);
}

void DebugHeaderWidget::adjustHeaderTable(int type, QTableWidget *pTableWidget)
{
    int nSymbolWidth=getSymbolWidth();

    pTableWidget->setColumnWidth(HEADER_COLUMN_OFFSET,nSymbolWidth*4);
    pTableWidget->setColumnWidth(HEADER_COLUMN_TYPE,nSymbolWidth*6);
    pTableWidget->setColumnWidth(HEADER_COLUMN_NAME,nSymbolWidth*20);
    pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE,nSymbolWidth*8);
    pTableWidget->setColumnWidth(HEADER_COLUMN_INFO,nSymbolWidth*16);
}

void DebugHeaderWidget::on_checkBoxReadonly_toggled(bool checked)
{
    setReadonly(checked);
}

void DebugHeaderWidget::reloadData()
{
    XPE pe(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

    if(pe.isValid())
    {
        if(!bInit)
        {
            bInit=createHeaderTable(SPE::TYPE_DEBUG,ui->tableWidget_DEBUG,N_IMAGE_DEBUG::records,lineEdit_DEBUG,N_IMAGE_IMPORT::__data_size,getNumber());
        }

        blockSignals(true);


        qint64 nOffset=pe.getImportDescriptorOffset(getNumber());
        qint64 nSize=pe.getImportDescriptorSize();
        qint64 nAddress=pe.offsetToRelAddress(nOffset);

        loadHexSubdevice(nOffset,nSize,nAddress,&pSubDevice,ui->widgetHex_DEBUG);

        blockSignals(false);

        setReadonly(ui->checkBoxReadonly->isChecked());
    }
}

void DebugHeaderWidget::on_tableWidget_DEBUG_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(currentRow)
    Q_UNUSED(currentColumn)
    Q_UNUSED(previousRow)
    Q_UNUSED(previousColumn)

    setHeaderTableSelection(ui->widgetHex_DEBUG,ui->tableWidget_DEBUG);
}

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
#include "dynamicarraytagwidget.h"
#include "ui_dynamicarraytagwidget.h"

DynamicArrayTagWidget::DynamicArrayTagWidget(QWidget *parent) :
    FormatWidget(parent),
    ui(new Ui::DynamicArrayTagWidget)
{
    ui->setupUi(this);
}

DynamicArrayTagWidget::~DynamicArrayTagWidget()
{
    delete ui;
}

void DynamicArrayTagWidget::clear()
{
    bInit=false;

    memset(lineEdit_DynamicArrayTag,0,sizeof lineEdit_DynamicArrayTag);
    memset(comboBox,0,sizeof comboBox);
    memset(invWidget,0,sizeof invWidget);

    pSubDevice=nullptr;

    ui->checkBoxReadonly->setChecked(true);
}

void DynamicArrayTagWidget::setData(QIODevice *pDevice, FW_DEF::OPTIONS *pOptions, quint32 nNumber)
{
    FormatWidget::setData(pDevice,pOptions,nNumber);
}

void DynamicArrayTagWidget::reload()
{
    clear();

    ui->checkBoxReadonly->setEnabled(!isReadonly());

    reloadData();
}

bool DynamicArrayTagWidget::_setValue(QVariant vValue, int nStype, int nNdata, int nVtype, int nPosition)
{
    return false;
}

void DynamicArrayTagWidget::setReadonly(bool bState)
{
    setLineEditsReadOnly(lineEdit_DynamicArrayTag,N_Elf_Phdr32::__data_size,bState);

    setComboBoxesReadOnly(comboBox,__CB_size,bState);
}

void DynamicArrayTagWidget::blockSignals(bool bState)
{
    _blockSignals((QObject **)lineEdit_DynamicArrayTag,N_Elf_Phdr32::__data_size,bState);

    _blockSignals((QObject **)comboBox,__CB_size,bState);
}

void DynamicArrayTagWidget::adjustHeaderTable(int type, QTableWidget *pTableWidget)
{
    int nSymbolWidth=getSymbolWidth();

    pTableWidget->setColumnWidth(HEADER_COLUMN_OFFSET,nSymbolWidth*4);
    pTableWidget->setColumnWidth(HEADER_COLUMN_TYPE,nSymbolWidth*8);
    pTableWidget->setColumnWidth(HEADER_COLUMN_NAME,nSymbolWidth*8);
    pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE,nSymbolWidth*12);
    pTableWidget->setColumnWidth(HEADER_COLUMN_INFO,nSymbolWidth*16);
}

void DynamicArrayTagWidget::on_checkBoxReadonly_toggled(bool checked)
{
    setReadonly(checked);
}

void DynamicArrayTagWidget::reloadData()
{
    XELF elf(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

    if(elf.isValid())
    {
        bool bIs64=elf.is64();
    }
}

void DynamicArrayTagWidget::widgetValueChanged(quint64 nValue)
{

}

void DynamicArrayTagWidget::on_tableWidget_DynamicArrayTag_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(currentRow)
    Q_UNUSED(currentColumn)
    Q_UNUSED(previousRow)
    Q_UNUSED(previousColumn)

    setHeaderTableSelection(ui->widgetHex_DynamicArrayTag,ui->tableWidget_DynamicArrayTag);
}

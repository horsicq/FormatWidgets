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
#include "sectionheaderwidget.h"
#include "ui_sectionheaderwidget.h"

SectionHeaderWidget::SectionHeaderWidget(QWidget *parent):
    FormatWidget(parent),
    ui(new Ui::SectionHeaderWidget)
{
    ui->setupUi(this);
}

SectionHeaderWidget::SectionHeaderWidget(QIODevice *pDevice, FW_DEF::OPTIONS *pOptions, quint32 nNumber, QWidget *parent):
    FormatWidget(pDevice,pOptions,nNumber,parent),
    ui(new Ui::SectionHeaderWidget)
{
    ui->setupUi(this);

    setData(pDevice,pOptions,nNumber);
}

SectionHeaderWidget::~SectionHeaderWidget()
{
    delete ui;
}

void SectionHeaderWidget::clear()
{
    bInit=false;

    memset(lineEdit_Elf_Shdr,0,sizeof lineEdit_Elf_Shdr);
    memset(comboBox,0,sizeof comboBox);
    memset(invWidget,0,sizeof invWidget);

    pSubDevice=nullptr;

    ui->checkBoxReadonly->setChecked(true);
}

void SectionHeaderWidget::setData(QIODevice *pDevice, FW_DEF::OPTIONS *pOptions, quint32 nNumber)
{
    FormatWidget::setData(pDevice,pOptions,nNumber);
}

void SectionHeaderWidget::reload()
{
    clear();

    ui->checkBoxReadonly->setEnabled(!isReadonly());

    reloadData();
}

bool SectionHeaderWidget::_setValue(QVariant vValue, int nStype, int nNdata, int nVtype,int nPosition)
{
    Q_UNUSED(nVtype)

    bool bResult=false;

    quint64 nValue=vValue.toULongLong();
    QString sValue=vValue.toString();

    if(getDevice()->isWritable())
    {
        XELF elf(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

        if(elf.isValid())
        {
            bResult=true;
        }
    }

    return bResult;
}
void SectionHeaderWidget::setReadonly(bool bState)
{
    setLineEditsReadOnly(lineEdit_Elf_Shdr,N_Elf_Shdr::__data_size,bState);

    setComboBoxesReadOnly(comboBox,__CB_size,bState);
}

void SectionHeaderWidget::blockSignals(bool bState)
{
    _blockSignals((QObject **)lineEdit_Elf_Shdr,N_Elf_Shdr::__data_size,bState);

    _blockSignals((QObject **)comboBox,__CB_size,bState);
}

void SectionHeaderWidget::adjustHeaderTable(int type, QTableWidget *pTableWidget)
{
    int nSymbolWidth=getSymbolWidth();

    pTableWidget->setColumnWidth(HEADER_COLUMN_OFFSET,nSymbolWidth*4);
    pTableWidget->setColumnWidth(HEADER_COLUMN_TYPE,nSymbolWidth*8);
    pTableWidget->setColumnWidth(HEADER_COLUMN_NAME,nSymbolWidth*8);
    pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE,nSymbolWidth*12);
    pTableWidget->setColumnWidth(HEADER_COLUMN_INFO,nSymbolWidth*16);
}

void SectionHeaderWidget::on_checkBoxReadonly_toggled(bool checked)
{
    setReadonly(checked);
}

void SectionHeaderWidget::reloadData()
{
    XELF elf(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

    if(elf.isValid())
    {
        bool bIs64=elf.is64();

        if(!bInit)
        {
            bInit=createHeaderTable(SELF::TYPE_Elf_Shdr,ui->tableWidget_Elf_Shdr,bIs64?(N_Elf_Shdr::records64):(N_Elf_Shdr::records32),lineEdit_Elf_Shdr,N_Elf_Shdr::__data_size,getNumber());
        }

        blockSignals(true);

        qint64 nOffset=elf.getShdrOffset(getNumber());
        qint64 nSize=elf.getShdrSize();
        qint64 nAddress=elf.offsetToRelAddress(nOffset);

        loadHexSubdevice(nOffset,nSize,nAddress,&pSubDevice,ui->widgetHex_Elf_Shdr);

        blockSignals(false);

        setReadonly(ui->checkBoxReadonly->isChecked());
    }
}

void SectionHeaderWidget::widgetValueChanged(quint64 nValue)
{
    QWidget *pWidget=qobject_cast<QWidget *>(sender());
    int nStype=pWidget->property("STYPE").toInt();
    int nNdata=pWidget->property("NDATA").toInt();
}

void SectionHeaderWidget::on_tableWidget_Elf_Shdr_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(currentRow)
    Q_UNUSED(currentColumn)
    Q_UNUSED(previousRow)
    Q_UNUSED(previousColumn)

    setHeaderTableSelection(ui->widgetHex_Elf_Shdr,ui->tableWidget_Elf_Shdr);
}

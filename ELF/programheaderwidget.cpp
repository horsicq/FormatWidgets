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
#include "programheaderwidget.h"
#include "ui_programheaderwidget.h"

ProgramHeaderWidget::ProgramHeaderWidget(QWidget *parent) :
    FormatWidget(parent),
    ui(new Ui::ProgramHeaderWidget)
{
    ui->setupUi(this);
}

ProgramHeaderWidget::~ProgramHeaderWidget()
{
    delete ui;
}

void ProgramHeaderWidget::clear()
{
    bInit=false;

    memset(lineEdit_Elf_Phdr,0,sizeof lineEdit_Elf_Phdr);
    memset(comboBox,0,sizeof comboBox);
    memset(invWidget,0,sizeof invWidget);

    pSubDevice=nullptr;

    ui->checkBoxReadonly->setChecked(true);
}

void ProgramHeaderWidget::setData(QIODevice *pDevice, FW_DEF::OPTIONS *pOptions, quint32 nNumber)
{
    FormatWidget::setData(pDevice,pOptions,nNumber);
}

void ProgramHeaderWidget::reload()
{
    clear();

    ui->checkBoxReadonly->setEnabled(!isReadonly());

    reloadData();
}

bool ProgramHeaderWidget::_setValue(QVariant vValue, int nStype, int nNdata, int nVtype, int nPosition)
{
    return false;
}

void ProgramHeaderWidget::setReadonly(bool bState)
{
    setLineEditsReadOnly(lineEdit_Elf_Phdr,N_Elf_Phdr32::__data_size,bState);

    setComboBoxesReadOnly(comboBox,__CB_size,bState);
}

void ProgramHeaderWidget::blockSignals(bool bState)
{
    _blockSignals((QObject **)lineEdit_Elf_Phdr,N_Elf_Phdr32::__data_size,bState);

    _blockSignals((QObject **)comboBox,__CB_size,bState);
}

void ProgramHeaderWidget::adjustHeaderTable(int type, QTableWidget *pTableWidget)
{
    int nSymbolWidth=getSymbolWidth();

    pTableWidget->setColumnWidth(HEADER_COLUMN_OFFSET,nSymbolWidth*4);
    pTableWidget->setColumnWidth(HEADER_COLUMN_TYPE,nSymbolWidth*8);
    pTableWidget->setColumnWidth(HEADER_COLUMN_NAME,nSymbolWidth*8);
    pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE,nSymbolWidth*12);
    pTableWidget->setColumnWidth(HEADER_COLUMN_INFO,nSymbolWidth*16);
}

void ProgramHeaderWidget::on_checkBoxReadonly_toggled(bool checked)
{
    setReadonly(checked);
}

void ProgramHeaderWidget::reloadData()
{
    XELF elf(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

    if(elf.isValid())
    {
        bool bIs64=elf.is64();

        if(!bInit)
        {
            bInit=createHeaderTable(SELF::TYPE_Elf_Phdr,ui->tableWidget_Elf_Phdr,bIs64?(N_Elf_Phdr64::records):(N_Elf_Phdr32::records),lineEdit_Elf_Phdr,bIs64?(N_Elf_Phdr64::__data_size):(N_Elf_Phdr32::__data_size),getNumber());
        }

        blockSignals(true);

        if(bIs64)
        {
            XELF_DEF::Elf64_Phdr phdr64=elf.getElf64_Phdr(getNumber());

            lineEdit_Elf_Phdr[N_Elf_Phdr64::p_type]->setValue(phdr64.p_type);
            lineEdit_Elf_Phdr[N_Elf_Phdr64::p_flags]->setValue(phdr64.p_flags);
            lineEdit_Elf_Phdr[N_Elf_Phdr64::p_offset]->setValue(phdr64.p_offset);
            lineEdit_Elf_Phdr[N_Elf_Phdr64::p_vaddr]->setValue(phdr64.p_vaddr);
            lineEdit_Elf_Phdr[N_Elf_Phdr64::p_paddr]->setValue(phdr64.p_paddr);
            lineEdit_Elf_Phdr[N_Elf_Phdr64::p_filesz]->setValue(phdr64.p_filesz);
            lineEdit_Elf_Phdr[N_Elf_Phdr64::p_memsz]->setValue(phdr64.p_memsz);
            lineEdit_Elf_Phdr[N_Elf_Phdr64::p_align]->setValue(phdr64.p_align);
        }
        else
        {
            XELF_DEF::Elf32_Phdr phdr32=elf.getElf32_Phdr(getNumber());

            lineEdit_Elf_Phdr[N_Elf_Phdr32::p_type]->setValue(phdr32.p_type);
            lineEdit_Elf_Phdr[N_Elf_Phdr32::p_offset]->setValue(phdr32.p_offset);
            lineEdit_Elf_Phdr[N_Elf_Phdr32::p_vaddr]->setValue(phdr32.p_vaddr);
            lineEdit_Elf_Phdr[N_Elf_Phdr32::p_paddr]->setValue(phdr32.p_paddr);
            lineEdit_Elf_Phdr[N_Elf_Phdr32::p_filesz]->setValue(phdr32.p_filesz);
            lineEdit_Elf_Phdr[N_Elf_Phdr32::p_memsz]->setValue(phdr32.p_memsz);
            lineEdit_Elf_Phdr[N_Elf_Phdr32::p_flags]->setValue(phdr32.p_flags);
            lineEdit_Elf_Phdr[N_Elf_Phdr32::p_align]->setValue(phdr32.p_align);
        }

        qint64 nOffset=elf.getPhdrOffset(getNumber());
        qint64 nSize=elf.getPhdrSize();
        qint64 nAddress=elf.offsetToRelAddress(nOffset);

        loadHexSubdevice(nOffset,nSize,nAddress,&pSubDevice,ui->widgetHex_Elf_Phdr);

        blockSignals(false);

        setReadonly(ui->checkBoxReadonly->isChecked());
    }
}

void ProgramHeaderWidget::on_tableWidget_Elf_Phdr_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(currentRow)
    Q_UNUSED(currentColumn)
    Q_UNUSED(previousRow)
    Q_UNUSED(previousColumn)

    setHeaderTableSelection(ui->widgetHex_Elf_Phdr,ui->tableWidget_Elf_Phdr);
}

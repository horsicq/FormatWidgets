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
    reset();

    memset(lineEdit,0,sizeof lineEdit);
    memset(comboBox,0,sizeof comboBox);
    memset(invWidget,0,sizeof invWidget);

    pSubDevice=nullptr;

    ui->checkBoxReadonly->setChecked(true);
}

void ProgramHeaderWidget::cleanup()
{

}

void ProgramHeaderWidget::reset()
{
    bInit=false;
}

void ProgramHeaderWidget::setData(QIODevice *pDevice, FW_DEF::OPTIONS *pOptions, quint32 nNumber,qint64 nOffset)
{
    FormatWidget::setData(pDevice,pOptions,nNumber,nOffset,0);
}

void ProgramHeaderWidget::reload()
{
    clear();

    ui->checkBoxReadonly->setEnabled(!isReadonly());

    reloadData();
}

bool ProgramHeaderWidget::_setValue(QVariant vValue, int nStype, int nNdata, int nVtype, int nPosition, qint64 nOffset)
{
    Q_UNUSED(nVtype)
    Q_UNUSED(nOffset)

    bool bResult=false;

    quint64 nValue=vValue.toULongLong();

    if(getDevice()->isWritable())
    {
        XELF elf(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

        if(elf.isValid())
        {
            switch(nStype)
            {
                case SELF::TYPE_Elf_Phdr:
                    if(elf.is64())
                    {
                        switch(nNdata)
                        {
                            case N_Elf_Phdr64::p_type:      elf.setElf64_Phdr_type((quint32)nPosition,(quint32)nValue);         break;
                            case N_Elf_Phdr64::p_flags:     elf.setElf64_Phdr_flags((quint32)nPosition,(quint32)nValue);        break;
                            case N_Elf_Phdr64::p_offset:    elf.setElf64_Phdr_offset((quint64)nPosition,(quint32)nValue);       break;
                            case N_Elf_Phdr64::p_vaddr:     elf.setElf64_Phdr_vaddr((quint64)nPosition,(quint32)nValue);        break;
                            case N_Elf_Phdr64::p_paddr:     elf.setElf64_Phdr_paddr((quint64)nPosition,(quint32)nValue);        break;
                            case N_Elf_Phdr64::p_filesz:    elf.setElf64_Phdr_filesz((quint64)nPosition,(quint32)nValue);       break;
                            case N_Elf_Phdr64::p_memsz:     elf.setElf64_Phdr_memsz((quint64)nPosition,(quint32)nValue);        break;
                            case N_Elf_Phdr64::p_align:     elf.setElf64_Phdr_align((quint64)nPosition,(quint32)nValue);        break;
                        }
                    }
                    else
                    {
                        switch(nNdata)
                        {
                            case N_Elf_Phdr32::p_type:      elf.setElf32_Phdr_type((quint32)nPosition,(quint32)nValue);         break;
                            case N_Elf_Phdr32::p_offset:    elf.setElf32_Phdr_offset((quint32)nPosition,(quint32)nValue);       break;
                            case N_Elf_Phdr32::p_vaddr:     elf.setElf32_Phdr_vaddr((quint32)nPosition,(quint32)nValue);        break;
                            case N_Elf_Phdr32::p_paddr:     elf.setElf32_Phdr_paddr((quint32)nPosition,(quint32)nValue);        break;
                            case N_Elf_Phdr32::p_filesz:    elf.setElf32_Phdr_filesz((quint32)nPosition,(quint32)nValue);       break;
                            case N_Elf_Phdr32::p_memsz:     elf.setElf32_Phdr_memsz((quint32)nPosition,(quint32)nValue);        break;
                            case N_Elf_Phdr32::p_flags:     elf.setElf32_Phdr_flags((quint32)nPosition,(quint32)nValue);        break;
                            case N_Elf_Phdr32::p_align:     elf.setElf32_Phdr_align((quint32)nPosition,(quint32)nValue);        break;
                        }
                    }

                    ui->widgetHex->reload();

                    break;
            }

            bResult=true;
        }
    }

    return bResult;
}

void ProgramHeaderWidget::setReadonly(bool bState)
{
    setLineEditsReadOnly(lineEdit,N_Elf_Phdr32::__data_size,bState);

    setComboBoxesReadOnly(comboBox,__CB_size,bState);
}

void ProgramHeaderWidget::blockSignals(bool bState)
{
    _blockSignals((QObject **)lineEdit,N_Elf_Phdr32::__data_size,bState);

    _blockSignals((QObject **)comboBox,__CB_size,bState);
}

void ProgramHeaderWidget::adjustHeaderTable(int type, QTableWidget *pTableWidget)
{
    Q_UNUSED(type)

    int nSymbolWidth=XLineEditHEX::getSymbolWidth(this);

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
            bInit=createHeaderTable(SELF::TYPE_Elf_Phdr,ui->tableWidget,bIs64?(N_Elf_Phdr64::records):(N_Elf_Phdr32::records),lineEdit,bIs64?(N_Elf_Phdr64::__data_size):(N_Elf_Phdr32::__data_size),getNumber());
            comboBox[CB_TYPE]=createComboBox(ui->tableWidget,XELF::getProgramTypesS(),SELF::TYPE_Elf_Phdr,bIs64?(N_Elf_Phdr64::p_type):(N_Elf_Phdr32::p_type),XComboBoxEx::CBTYPE_NORMAL);
            comboBox[CB_FLAGS]=createComboBox(ui->tableWidget,XELF::getProgramFlagsS(),SELF::TYPE_Elf_Phdr,bIs64?(N_Elf_Phdr64::p_flags):(N_Elf_Phdr32::p_flags),XComboBoxEx::CBTYPE_FLAGS);
        }

        blockSignals(true);

        if(bIs64)
        {
            XELF_DEF::Elf64_Phdr phdr64=elf.getElf64_Phdr(getNumber());

            lineEdit[N_Elf_Phdr64::p_type]->setValue(phdr64.p_type);
            lineEdit[N_Elf_Phdr64::p_flags]->setValue(phdr64.p_flags);
            lineEdit[N_Elf_Phdr64::p_offset]->setValue(phdr64.p_offset);
            lineEdit[N_Elf_Phdr64::p_vaddr]->setValue(phdr64.p_vaddr);
            lineEdit[N_Elf_Phdr64::p_paddr]->setValue(phdr64.p_paddr);
            lineEdit[N_Elf_Phdr64::p_filesz]->setValue(phdr64.p_filesz);
            lineEdit[N_Elf_Phdr64::p_memsz]->setValue(phdr64.p_memsz);
            lineEdit[N_Elf_Phdr64::p_align]->setValue(phdr64.p_align);

            comboBox[CB_TYPE]->setValue(phdr64.p_type);
            comboBox[CB_FLAGS]->setValue(phdr64.p_flags);
        }
        else
        {
            XELF_DEF::Elf32_Phdr phdr32=elf.getElf32_Phdr(getNumber());

            lineEdit[N_Elf_Phdr32::p_type]->setValue(phdr32.p_type);
            lineEdit[N_Elf_Phdr32::p_offset]->setValue(phdr32.p_offset);
            lineEdit[N_Elf_Phdr32::p_vaddr]->setValue(phdr32.p_vaddr);
            lineEdit[N_Elf_Phdr32::p_paddr]->setValue(phdr32.p_paddr);
            lineEdit[N_Elf_Phdr32::p_filesz]->setValue(phdr32.p_filesz);
            lineEdit[N_Elf_Phdr32::p_memsz]->setValue(phdr32.p_memsz);
            lineEdit[N_Elf_Phdr32::p_flags]->setValue(phdr32.p_flags);
            lineEdit[N_Elf_Phdr32::p_align]->setValue(phdr32.p_align);

            comboBox[CB_TYPE]->setValue(phdr32.p_type);
            comboBox[CB_FLAGS]->setValue(phdr32.p_flags);
        }

        qint64 nOffset=elf.getPhdrOffset(getNumber());
        qint64 nSize=elf.getPhdrSize();
        qint64 nAddress=elf.offsetToRelAddress(nOffset);

        loadHexSubdevice(nOffset,nSize,nAddress,&pSubDevice,ui->widgetHex);

        blockSignals(false);

        setReadonly(ui->checkBoxReadonly->isChecked());
    }
}

void ProgramHeaderWidget::widgetValueChanged(quint64 nValue)
{
    QWidget *pWidget=qobject_cast<QWidget *>(sender());
    int nStype=pWidget->property("STYPE").toInt();
    int nNdata=pWidget->property("NDATA").toInt();

    XELF elf(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

    if(elf.isValid())
    {
        bool bIs64=elf.is64();

        switch(nStype)
        {
            case SELF::TYPE_Elf_Phdr:

                if(bIs64)
                {
                    switch(nNdata)
                    {
                        case N_Elf_Phdr64::p_type:
                            lineEdit[N_Elf_Phdr64::p_type]->setValue((quint32)nValue);
                            this->comboBox[CB_TYPE]->setValue(nValue);
                            break;

                        case N_Elf_Phdr64::p_flags:
                            lineEdit[N_Elf_Phdr64::p_flags]->setValue((quint32)nValue);
                            this->comboBox[CB_FLAGS]->setValue(nValue);
                            break;
                    }
                }
                else
                {
                    switch(nNdata)
                    {
                        case N_Elf_Phdr32::p_type:
                            lineEdit[N_Elf_Phdr32::p_type]->setValue((quint32)nValue);
                            this->comboBox[CB_TYPE]->setValue(nValue);
                            break;

                        case N_Elf_Phdr32::p_flags:
                            lineEdit[N_Elf_Phdr32::p_flags]->setValue((quint32)nValue);
                            this->comboBox[CB_FLAGS]->setValue(nValue);
                            break;
                    }
                }

                break;
        }
    }
}

void ProgramHeaderWidget::on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(currentRow)
    Q_UNUSED(currentColumn)
    Q_UNUSED(previousRow)
    Q_UNUSED(previousColumn)

    setHeaderTableSelection(ui->widgetHex,ui->tableWidget);
}

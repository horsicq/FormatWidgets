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

SectionHeaderWidget::SectionHeaderWidget(QIODevice *pDevice, FW_DEF::OPTIONS *pOptions, quint32 nNumber,qint64 nOffset, QWidget *parent):
    FormatWidget(pDevice,pOptions,nNumber,nOffset,parent),
    ui(new Ui::SectionHeaderWidget)
{
    ui->setupUi(this);

    setData(pDevice,pOptions,nNumber,nOffset);
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

void SectionHeaderWidget::setData(QIODevice *pDevice, FW_DEF::OPTIONS *pOptions, quint32 nNumber,qint64 nOffset)
{
    FormatWidget::setData(pDevice,pOptions,nNumber,nOffset);
}

void SectionHeaderWidget::reload()
{
    clear();

    ui->checkBoxReadonly->setEnabled(!isReadonly());

    reloadData();
}

bool SectionHeaderWidget::_setValue(QVariant vValue, int nStype, int nNdata, int nVtype, int nPosition, qint64 nOffset)
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
                // TODO Comment name
                case SELF::TYPE_Elf_Shdr:
                    if(elf.is64())
                    {
                        switch(nNdata)
                        {
                            case N_Elf_Shdr::sh_name:       elf.setElf64_Shdr_name((quint32)nPosition,(quint32)nValue);         break;
                            case N_Elf_Shdr::sh_type:       elf.setElf64_Shdr_type((quint32)nPosition,(quint32)nValue);         break;
                            case N_Elf_Shdr::sh_flags:      elf.setElf64_Shdr_flags((quint64)nPosition,(quint32)nValue);        break;
                            case N_Elf_Shdr::sh_addr:       elf.setElf64_Shdr_addr((quint64)nPosition,(quint32)nValue);         break;
                            case N_Elf_Shdr::sh_offset:     elf.setElf64_Shdr_offset((quint64)nPosition,(quint32)nValue);       break;
                            case N_Elf_Shdr::sh_size:       elf.setElf64_Shdr_size((quint64)nPosition,(quint32)nValue);         break;
                            case N_Elf_Shdr::sh_link:       elf.setElf64_Shdr_link((quint32)nPosition,(quint32)nValue);         break;
                            case N_Elf_Shdr::sh_info:       elf.setElf64_Shdr_info((quint32)nPosition,(quint32)nValue);         break;
                            case N_Elf_Shdr::sh_addralign:  elf.setElf64_Shdr_addralign((quint64)nPosition,(quint32)nValue);    break;
                            case N_Elf_Shdr::sh_entsize:    elf.setElf64_Shdr_entsize((quint64)nPosition,(quint32)nValue);      break;
                        }
                    }
                    else
                    {
                        switch(nNdata)
                        {
                            case N_Elf_Shdr::sh_name:       elf.setElf32_Shdr_name((quint32)nPosition,(quint32)nValue);         break;
                            case N_Elf_Shdr::sh_type:       elf.setElf32_Shdr_type((quint32)nPosition,(quint32)nValue);         break;
                            case N_Elf_Shdr::sh_flags:      elf.setElf32_Shdr_flags((quint32)nPosition,(quint32)nValue);        break;
                            case N_Elf_Shdr::sh_addr:       elf.setElf32_Shdr_addr((quint32)nPosition,(quint32)nValue);         break;
                            case N_Elf_Shdr::sh_offset:     elf.setElf32_Shdr_offset((quint32)nPosition,(quint32)nValue);       break;
                            case N_Elf_Shdr::sh_size:       elf.setElf32_Shdr_size((quint32)nPosition,(quint32)nValue);         break;
                            case N_Elf_Shdr::sh_link:       elf.setElf32_Shdr_link((quint32)nPosition,(quint32)nValue);         break;
                            case N_Elf_Shdr::sh_info:       elf.setElf32_Shdr_info((quint32)nPosition,(quint32)nValue);         break;
                            case N_Elf_Shdr::sh_addralign:  elf.setElf32_Shdr_addralign((quint32)nPosition,(quint32)nValue);    break;
                            case N_Elf_Shdr::sh_entsize:    elf.setElf32_Shdr_entsize((quint32)nPosition,(quint32)nValue);      break;
                        }
                    }

                    ui->widgetHex_Elf_Shdr->reload();

                    break;
            }

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
    Q_UNUSED(type)

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
            comboBox[CB_TYPE]=createComboBox(ui->tableWidget_Elf_Shdr,XELF::getSectionTypesS(),SELF::TYPE_Elf_Shdr,N_Elf_Shdr::sh_type,XComboBoxEx::CBTYPE_NORMAL);
            comboBox[CB_FLAGS]=createComboBox(ui->tableWidget_Elf_Shdr,XELF::getSectionFlagsS(),SELF::TYPE_Elf_Shdr,N_Elf_Shdr::sh_flags,XComboBoxEx::CBTYPE_FLAGS);
        }

        blockSignals(true);

        if(bIs64)
        {
            XELF_DEF::Elf64_Shdr shdr64=elf.getElf64_Shdr(getNumber());

            lineEdit_Elf_Shdr[N_Elf_Shdr::sh_name]->setValue(shdr64.sh_name);
            lineEdit_Elf_Shdr[N_Elf_Shdr::sh_type]->setValue(shdr64.sh_type);
            lineEdit_Elf_Shdr[N_Elf_Shdr::sh_flags]->setValue(shdr64.sh_flags);
            lineEdit_Elf_Shdr[N_Elf_Shdr::sh_addr]->setValue(shdr64.sh_addr);
            lineEdit_Elf_Shdr[N_Elf_Shdr::sh_offset]->setValue(shdr64.sh_offset);
            lineEdit_Elf_Shdr[N_Elf_Shdr::sh_size]->setValue(shdr64.sh_size);
            lineEdit_Elf_Shdr[N_Elf_Shdr::sh_link]->setValue(shdr64.sh_link);
            lineEdit_Elf_Shdr[N_Elf_Shdr::sh_info]->setValue(shdr64.sh_info);
            lineEdit_Elf_Shdr[N_Elf_Shdr::sh_addralign]->setValue(shdr64.sh_addralign);
            lineEdit_Elf_Shdr[N_Elf_Shdr::sh_entsize]->setValue(shdr64.sh_entsize);

            comboBox[CB_TYPE]->setValue(shdr64.sh_type);
            comboBox[CB_FLAGS]->setValue(shdr64.sh_flags);
        }
        else
        {
            XELF_DEF::Elf32_Shdr shdr32=elf.getElf32_Shdr(getNumber());

            lineEdit_Elf_Shdr[N_Elf_Shdr::sh_name]->setValue(shdr32.sh_name);
            lineEdit_Elf_Shdr[N_Elf_Shdr::sh_type]->setValue(shdr32.sh_type);
            lineEdit_Elf_Shdr[N_Elf_Shdr::sh_flags]->setValue(shdr32.sh_flags);
            lineEdit_Elf_Shdr[N_Elf_Shdr::sh_addr]->setValue(shdr32.sh_addr);
            lineEdit_Elf_Shdr[N_Elf_Shdr::sh_offset]->setValue(shdr32.sh_offset);
            lineEdit_Elf_Shdr[N_Elf_Shdr::sh_size]->setValue(shdr32.sh_size);
            lineEdit_Elf_Shdr[N_Elf_Shdr::sh_link]->setValue(shdr32.sh_link);
            lineEdit_Elf_Shdr[N_Elf_Shdr::sh_info]->setValue(shdr32.sh_info);
            lineEdit_Elf_Shdr[N_Elf_Shdr::sh_addralign]->setValue(shdr32.sh_addralign);
            lineEdit_Elf_Shdr[N_Elf_Shdr::sh_entsize]->setValue(shdr32.sh_entsize);

            comboBox[CB_TYPE]->setValue(shdr32.sh_type);
            comboBox[CB_FLAGS]->setValue(shdr32.sh_flags);
        }

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

    XELF elf(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

    if(elf.isValid())
    {
        bool bIs64=elf.is64();

        switch(nStype)
        {
            case SELF::TYPE_Elf_Shdr:

                switch(nNdata)
                {
                    case N_Elf_Shdr::sh_type:
                        lineEdit_Elf_Shdr[N_Elf_Shdr::sh_type]->setValue((quint32)nValue);
                        this->comboBox[CB_TYPE]->setValue(nValue);
                        break;

                    case N_Elf_Shdr::sh_flags:
                        lineEdit_Elf_Shdr[N_Elf_Shdr::sh_flags]->setValue(bIs64?((quint64)nValue):((quint32)nValue));
                        this->comboBox[CB_FLAGS]->setValue(nValue);
                        break;
                }

                break;
        }
    }
}

void SectionHeaderWidget::on_tableWidget_Elf_Shdr_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(currentRow)
    Q_UNUSED(currentColumn)
    Q_UNUSED(previousRow)
    Q_UNUSED(previousColumn)

    setHeaderTableSelection(ui->widgetHex_Elf_Shdr,ui->tableWidget_Elf_Shdr);
}

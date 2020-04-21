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

SectionHeaderWidget::SectionHeaderWidget(QIODevice *pDevice, FW_DEF::OPTIONS *pOptions, quint32 nNumber,qint64 nOffset, qint32 nType, QWidget *parent):
    FormatWidget(pDevice,pOptions,nNumber,nOffset,nType,parent),
    ui(new Ui::SectionHeaderWidget)
{
    ui->setupUi(this);

    ppLinedEdit=0;
    nLineEditSize=0;
    ppComboBox=0;
    nComboBoxSize=0;
    ppInvWidget=0;
    nInvWidgetSize=0;

    if(nType==SELF::TYPE_Elf_Shdr)
    {
        nLineEditSize=N_Elf_Shdr::__data_size;
        nComboBoxSize=N_Elf_Shdr::__CB_size;
        nInvWidgetSize=N_Elf_Shdr::__INV_size;
    }
    else if(nType==SELF::TYPE_Elf_Phdr)
    {
        nLineEditSize=N_Elf_Phdr32::__data_size;
        nComboBoxSize=N_Elf_Phdr32::__CB_size;
        nInvWidgetSize=N_Elf_Phdr32::__INV_size;
    }
    else if(nType==SELF::TYPE_Elf_DynamicArrayTags)
    {
        nLineEditSize=N_Elf_DynamicArrayTags::__data_size;
        nComboBoxSize=N_Elf_DynamicArrayTags::__CB_size;
        nInvWidgetSize=N_Elf_DynamicArrayTags::__INV_size;
    }
    else if(nType==SELF::TYPE_SYMBOLTABLE)
    {
        nLineEditSize=N_Elf32_Sym::__data_size;
        nComboBoxSize=N_Elf32_Sym::__CB_size;
        nInvWidgetSize=N_Elf32_Sym::__INV_size;
    }

    if(nLineEditSize)
    {
        ppLinedEdit=new PXLineEditHEX[nLineEditSize];
    }

    if(nComboBoxSize)
    {
        ppComboBox=new PXComboBoxEx[nComboBoxSize];
    }

    if(nInvWidgetSize)
    {
        ppInvWidget=new PInvWidget[nInvWidgetSize];
    }

    setData(pDevice,pOptions,nNumber,nOffset,nType);
}

SectionHeaderWidget::~SectionHeaderWidget()
{
    if(ppLinedEdit)
    {
        delete[] ppLinedEdit;
    }

    if(ppComboBox)
    {
        delete[] ppComboBox;
    }

    if(ppInvWidget)
    {
        delete[] ppInvWidget;
    }

    delete ui;
}

void SectionHeaderWidget::clear()
{
    reset();

    memset(ppLinedEdit,0,nLineEditSize*sizeof(XLineEditHEX *));
    memset(ppComboBox,0,nComboBoxSize*sizeof(XComboBoxEx *));
    memset(ppInvWidget,0,nInvWidgetSize*sizeof(InvWidget *));

    pSubDevice=nullptr;

    ui->checkBoxReadonly->setChecked(true);
}

void SectionHeaderWidget::cleanup()
{

}

void SectionHeaderWidget::reset()
{
    bInit=false;
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

                    break;

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

                    break;

                case SELF::TYPE_Elf_DynamicArrayTags:
                    switch(nNdata)
                    {
                        case N_Elf_DynamicArrayTags::d_tag:         elf.setDynamicArrayTag(nOffset,nValue);         break;
                        case N_Elf_DynamicArrayTags::d_value:       elf.setDynamicArrayValue(nOffset,nValue);       break;
                    }

                    break;
            }

            ui->widgetHex->reload();

            bResult=true;
        }
    }

    return bResult;
}
void SectionHeaderWidget::setReadonly(bool bState)
{
    setLineEditsReadOnly(ppLinedEdit,nLineEditSize,bState);

    setComboBoxesReadOnly(ppComboBox,nComboBoxSize,bState);
}

void SectionHeaderWidget::blockSignals(bool bState)
{
    _blockSignals((QObject **)ppLinedEdit,nLineEditSize,bState);

    _blockSignals((QObject **)ppComboBox,nComboBoxSize,bState);
}

void SectionHeaderWidget::adjustHeaderTable(int type, QTableWidget *pTableWidget)
{
    int nSymbolWidth=XLineEditHEX::getSymbolWidth(this);

    pTableWidget->setColumnWidth(HEADER_COLUMN_OFFSET,nSymbolWidth*4);
    pTableWidget->setColumnWidth(HEADER_COLUMN_TYPE,nSymbolWidth*8);
    pTableWidget->setColumnWidth(HEADER_COLUMN_NAME,nSymbolWidth*8);

    switch(type)
    {
        case SELF::TYPE_Elf_Shdr:
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE,nSymbolWidth*12);
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO,nSymbolWidth*16);
            break;

        case SELF::TYPE_Elf_Phdr:
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE,nSymbolWidth*12);
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO,nSymbolWidth*16);
            break;

        case SELF::TYPE_Elf_DynamicArrayTags:
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE,nSymbolWidth*12);
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO,nSymbolWidth*16);
            break;

        case SELF::TYPE_SYMBOLTABLE:
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE,nSymbolWidth*12);
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO,nSymbolWidth*16);
            break;
    }
}

void SectionHeaderWidget::on_checkBoxReadonly_toggled(bool checked)
{
    setReadonly(checked);
}

void SectionHeaderWidget::reloadData()
{
    int nType=getType();

    XELF elf(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

    if(elf.isValid())
    {       
        bool bIs64=elf.is64();

        if(!bInit)
        {
            if(nType==SELF::TYPE_Elf_Shdr)
            {
                bInit=createHeaderTable(SELF::TYPE_Elf_Shdr,ui->tableWidget,bIs64?(N_Elf_Shdr::records64):(N_Elf_Shdr::records32),ppLinedEdit,N_Elf_Shdr::__data_size,getNumber());
                ppComboBox[N_Elf_Shdr::CB_TYPE]=createComboBox(ui->tableWidget,XELF::getSectionTypesS(),SELF::TYPE_Elf_Shdr,N_Elf_Shdr::sh_type,XComboBoxEx::CBTYPE_NORMAL);
                ppComboBox[N_Elf_Shdr::CB_FLAGS]=createComboBox(ui->tableWidget,XELF::getSectionFlagsS(),SELF::TYPE_Elf_Shdr,N_Elf_Shdr::sh_flags,XComboBoxEx::CBTYPE_FLAGS);

                blockSignals(true);

                if(bIs64)
                {
                    XELF_DEF::Elf64_Shdr shdr64=elf.getElf64_Shdr(getNumber());

                    ppLinedEdit[N_Elf_Shdr::sh_name]->setValue(shdr64.sh_name);
                    ppLinedEdit[N_Elf_Shdr::sh_type]->setValue(shdr64.sh_type);
                    ppLinedEdit[N_Elf_Shdr::sh_flags]->setValue(shdr64.sh_flags);
                    ppLinedEdit[N_Elf_Shdr::sh_addr]->setValue(shdr64.sh_addr);
                    ppLinedEdit[N_Elf_Shdr::sh_offset]->setValue(shdr64.sh_offset);
                    ppLinedEdit[N_Elf_Shdr::sh_size]->setValue(shdr64.sh_size);
                    ppLinedEdit[N_Elf_Shdr::sh_link]->setValue(shdr64.sh_link);
                    ppLinedEdit[N_Elf_Shdr::sh_info]->setValue(shdr64.sh_info);
                    ppLinedEdit[N_Elf_Shdr::sh_addralign]->setValue(shdr64.sh_addralign);
                    ppLinedEdit[N_Elf_Shdr::sh_entsize]->setValue(shdr64.sh_entsize);

                    ppComboBox[N_Elf_Shdr::CB_TYPE]->setValue(shdr64.sh_type);
                    ppComboBox[N_Elf_Shdr::CB_FLAGS]->setValue(shdr64.sh_flags);
                }
                else
                {
                    XELF_DEF::Elf32_Shdr shdr32=elf.getElf32_Shdr(getNumber());

                    ppLinedEdit[N_Elf_Shdr::sh_name]->setValue(shdr32.sh_name);
                    ppLinedEdit[N_Elf_Shdr::sh_type]->setValue(shdr32.sh_type);
                    ppLinedEdit[N_Elf_Shdr::sh_flags]->setValue(shdr32.sh_flags);
                    ppLinedEdit[N_Elf_Shdr::sh_addr]->setValue(shdr32.sh_addr);
                    ppLinedEdit[N_Elf_Shdr::sh_offset]->setValue(shdr32.sh_offset);
                    ppLinedEdit[N_Elf_Shdr::sh_size]->setValue(shdr32.sh_size);
                    ppLinedEdit[N_Elf_Shdr::sh_link]->setValue(shdr32.sh_link);
                    ppLinedEdit[N_Elf_Shdr::sh_info]->setValue(shdr32.sh_info);
                    ppLinedEdit[N_Elf_Shdr::sh_addralign]->setValue(shdr32.sh_addralign);
                    ppLinedEdit[N_Elf_Shdr::sh_entsize]->setValue(shdr32.sh_entsize);

                    ppComboBox[N_Elf_Shdr::CB_TYPE]->setValue(shdr32.sh_type);
                    ppComboBox[N_Elf_Shdr::CB_FLAGS]->setValue(shdr32.sh_flags);
                }

                qint64 nOffset=elf.getShdrOffset(getNumber());
                qint64 nSize=elf.getShdrSize();
                qint64 nAddress=elf.offsetToRelAddress(nOffset);

                loadHexSubdevice(nOffset,nSize,nAddress,&pSubDevice,ui->widgetHex);

                blockSignals(false);
            }
            else if(nType==SELF::TYPE_Elf_Phdr)
            {
                bInit=createHeaderTable(SELF::TYPE_Elf_Phdr,ui->tableWidget,bIs64?(N_Elf_Phdr64::records):(N_Elf_Phdr32::records),ppLinedEdit,bIs64?(N_Elf_Phdr64::__data_size):(N_Elf_Phdr32::__data_size),getNumber());
                ppComboBox[N_Elf_Phdr32::CB_TYPE]=createComboBox(ui->tableWidget,XELF::getProgramTypesS(),SELF::TYPE_Elf_Phdr,bIs64?(N_Elf_Phdr64::p_type):(N_Elf_Phdr32::p_type),XComboBoxEx::CBTYPE_NORMAL);
                ppComboBox[N_Elf_Phdr32::CB_FLAGS]=createComboBox(ui->tableWidget,XELF::getProgramFlagsS(),SELF::TYPE_Elf_Phdr,bIs64?(N_Elf_Phdr64::p_flags):(N_Elf_Phdr32::p_flags),XComboBoxEx::CBTYPE_FLAGS);

                blockSignals(true);

                if(bIs64)
                {
                    XELF_DEF::Elf64_Phdr phdr64=elf.getElf64_Phdr(getNumber());

                    ppLinedEdit[N_Elf_Phdr64::p_type]->setValue(phdr64.p_type);
                    ppLinedEdit[N_Elf_Phdr64::p_flags]->setValue(phdr64.p_flags);
                    ppLinedEdit[N_Elf_Phdr64::p_offset]->setValue(phdr64.p_offset);
                    ppLinedEdit[N_Elf_Phdr64::p_vaddr]->setValue(phdr64.p_vaddr);
                    ppLinedEdit[N_Elf_Phdr64::p_paddr]->setValue(phdr64.p_paddr);
                    ppLinedEdit[N_Elf_Phdr64::p_filesz]->setValue(phdr64.p_filesz);
                    ppLinedEdit[N_Elf_Phdr64::p_memsz]->setValue(phdr64.p_memsz);
                    ppLinedEdit[N_Elf_Phdr64::p_align]->setValue(phdr64.p_align);

                    ppComboBox[N_Elf_Phdr32::CB_TYPE]->setValue(phdr64.p_type);
                    ppComboBox[N_Elf_Phdr32::CB_FLAGS]->setValue(phdr64.p_flags);
                }
                else
                {
                    XELF_DEF::Elf32_Phdr phdr32=elf.getElf32_Phdr(getNumber());

                    ppLinedEdit[N_Elf_Phdr32::p_type]->setValue(phdr32.p_type);
                    ppLinedEdit[N_Elf_Phdr32::p_offset]->setValue(phdr32.p_offset);
                    ppLinedEdit[N_Elf_Phdr32::p_vaddr]->setValue(phdr32.p_vaddr);
                    ppLinedEdit[N_Elf_Phdr32::p_paddr]->setValue(phdr32.p_paddr);
                    ppLinedEdit[N_Elf_Phdr32::p_filesz]->setValue(phdr32.p_filesz);
                    ppLinedEdit[N_Elf_Phdr32::p_memsz]->setValue(phdr32.p_memsz);
                    ppLinedEdit[N_Elf_Phdr32::p_flags]->setValue(phdr32.p_flags);
                    ppLinedEdit[N_Elf_Phdr32::p_align]->setValue(phdr32.p_align);

                    ppComboBox[N_Elf_Phdr32::CB_TYPE]->setValue(phdr32.p_type);
                    ppComboBox[N_Elf_Phdr32::CB_FLAGS]->setValue(phdr32.p_flags);
                }

                qint64 nOffset=elf.getPhdrOffset(getNumber());
                qint64 nSize=elf.getPhdrSize();
                qint64 nAddress=elf.offsetToRelAddress(nOffset);

                loadHexSubdevice(nOffset,nSize,nAddress,&pSubDevice,ui->widgetHex);

                blockSignals(false);
            }
            else if(nType==SELF::TYPE_Elf_DynamicArrayTags)
            {
                bInit=createHeaderTable(SELF::TYPE_Elf_DynamicArrayTags,ui->tableWidget,bIs64?(N_Elf_DynamicArrayTags::records64):(N_Elf_DynamicArrayTags::records32),ppLinedEdit,N_Elf_DynamicArrayTags::__data_size,getNumber(),getOffset());
                ppComboBox[N_Elf_DynamicArrayTags::CB_TAG]=createComboBox(ui->tableWidget,XELF::getDynamicTagsS(),SELF::TYPE_Elf_DynamicArrayTags,N_Elf_DynamicArrayTags::d_tag,XComboBoxEx::CBTYPE_NORMAL);

                blockSignals(true);

                qint64 nOffset=getOffset();

                qint64 nTag=elf.getDynamicArrayTag(nOffset);
                qint64 nValue=elf.getDynamicArrayValue(nOffset);

                ppLinedEdit[N_Elf_DynamicArrayTags::d_tag]->setValue(bIs64?((qint64)nTag):((qint32)nTag));
                ppLinedEdit[N_Elf_DynamicArrayTags::d_value]->setValue(bIs64?((qint64)nValue):((qint32)nValue));

                ppComboBox[N_Elf_DynamicArrayTags::CB_TAG]->setValue(nTag);

                qint64 nSize=elf.getDynamicArraySize();
                qint64 nAddress=elf.offsetToRelAddress(nOffset);

                loadHexSubdevice(nOffset,nSize,nAddress,&pSubDevice,ui->widgetHex);

                blockSignals(false);
            }
            else if(nType==SELF::TYPE_SYMBOLTABLE)
            {
                bInit=createHeaderTable(SELF::TYPE_SYMBOLTABLE,ui->tableWidget,bIs64?(N_Elf64_Sym::records):(N_Elf32_Sym::records),ppLinedEdit,N_Elf32_Sym::__data_size,getNumber(),getOffset());

                blockSignals(true);

                qint64 nOffset=getOffset();

                qint64 nSize=elf.getSymSize();
                qint64 nAddress=elf.offsetToRelAddress(nOffset);

                loadHexSubdevice(nOffset,nSize,nAddress,&pSubDevice,ui->widgetHex);

                blockSignals(false);
            }
        }

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
                        ppLinedEdit[N_Elf_Shdr::sh_type]->setValue((quint32)nValue);
                        ppComboBox[N_Elf_Shdr::CB_TYPE]->setValue(nValue);
                        break;

                    case N_Elf_Shdr::sh_flags:
                        ppLinedEdit[N_Elf_Shdr::sh_flags]->setValue(bIs64?((quint64)nValue):((quint32)nValue));
                        ppComboBox[N_Elf_Shdr::CB_FLAGS]->setValue(nValue);
                        break;
                }
                break;

            case SELF::TYPE_Elf_Phdr:
                if(bIs64)
                {
                    switch(nNdata)
                    {
                        case N_Elf_Phdr64::p_type:
                            ppLinedEdit[N_Elf_Phdr64::p_type]->setValue((quint32)nValue);
                            ppComboBox[N_Elf_Phdr32::CB_TYPE]->setValue(nValue);
                            break;

                        case N_Elf_Phdr64::p_flags:
                            ppLinedEdit[N_Elf_Phdr64::p_flags]->setValue((quint32)nValue);
                            ppComboBox[N_Elf_Phdr32::CB_FLAGS]->setValue(nValue);
                            break;
                    }
                }
                else
                {
                    switch(nNdata)
                    {
                        case N_Elf_Phdr32::p_type:
                            ppLinedEdit[N_Elf_Phdr32::p_type]->setValue((quint32)nValue);
                            ppComboBox[N_Elf_Phdr32::CB_TYPE]->setValue(nValue);
                            break;

                        case N_Elf_Phdr32::p_flags:
                            ppLinedEdit[N_Elf_Phdr32::p_flags]->setValue((quint32)nValue);
                            ppComboBox[N_Elf_Phdr32::CB_FLAGS]->setValue(nValue);
                            break;
                    }
                }

                break;

            case SELF::TYPE_Elf_DynamicArrayTags:

                switch(nNdata)
                {
                    case N_Elf_DynamicArrayTags::d_tag:
                        ppLinedEdit[N_Elf_DynamicArrayTags::d_tag]->setValue(bIs64?((qint64)nValue):((qint32)nValue));
                        ppComboBox[N_Elf_DynamicArrayTags::CB_TAG]->setValue(nValue);
                        break;
                }

                break;
        }
    }
}

void SectionHeaderWidget::on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(currentRow)
    Q_UNUSED(currentColumn)
    Q_UNUSED(previousRow)
    Q_UNUSED(previousColumn)

    setHeaderTableSelection(ui->widgetHex,ui->tableWidget);
}

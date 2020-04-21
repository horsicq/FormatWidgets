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
#include "elfprocessdata.h"

ELFProcessData::ELFProcessData(int type, QStandardItemModel **ppModel, XELF *pELF, qint64 nOffset, qint64 nSize, qint64 nStringTableOffset, qint64 nStringTableSize)
{
    this->type=type;
    this->ppModel=ppModel;
    this->pELF=pELF;
    this->nOffset=nOffset;
    this->nSize=nSize;
    this->nStringTableOffset=nStringTableOffset;
    this->nStringTableSize=nStringTableSize;
}

void ELFProcessData::_process()
{
    if(type==SELF::TYPE_Elf_Shdr)
    {
        bool bIs64=pELF->is64();

        QList<QString> listLabels;
        listLabels.append("");

        if(bIs64)
        {
            listLabels.append(getStructList(N_Elf_Shdr::records64,N_Elf_Shdr::__data_size));
        }
        else
        {
            listLabels.append(getStructList(N_Elf_Shdr::records32,N_Elf_Shdr::__data_size));
        }

        listLabels.append("Name");
        listLabels.append("Type");

        QMap<quint64,QString> mapTypes=XELF::getSectionTypesS();

        QList<XELF_DEF::Elf64_Shdr> listSections64;
        QList<XELF_DEF::Elf32_Shdr> listSections32;

        int nCount=0;

        if(bIs64)
        {
            listSections64=pELF->getElf64_ShdrList();
            nCount=listSections64.count();
        }
        else
        {
            listSections32=pELF->getElf32_ShdrList();
            nCount=listSections32.count();
        }

        *ppModel=new QStandardItemModel(nCount,listLabels.count());

        setMaximum(nCount);

        setHeader(*ppModel,&listLabels);

        quint32 nMainStringSection=pELF->getSectionStringTable();

        XBinary::OFFSETSIZE osStringTable=pELF->getSectionOffsetSize(nMainStringSection);

        for(int i=0;(i<nCount)&&(isRun());i++)
        {
            if(bIs64)
            {
                QStandardItem *pItem=new QStandardItem(QString::number(i));

                if(pELF->isImage())
                {
                    pItem->setData(listSections64.at(i).sh_addr,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET);
                }
                else
                {
                    pItem->setData(listSections64.at(i).sh_offset,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET);
                }

                pItem->setData(listSections64.at(i).sh_size,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE);
                pItem->setData(listSections64.at(i).sh_addr,Qt::UserRole+FW_DEF::SECTION_DATA_ADDRESS);

                (*ppModel)->setItem(i,0,                              pItem);
                (*ppModel)->setItem(i,N_Elf_Shdr::sh_name+1,          new QStandardItem(XBinary::valueToHex(listSections64.at(i).sh_name)));
                (*ppModel)->setItem(i,N_Elf_Shdr::sh_type+1,          new QStandardItem(XBinary::valueToHex(listSections64.at(i).sh_type)));
                (*ppModel)->setItem(i,N_Elf_Shdr::sh_flags+1,         new QStandardItem(XBinary::valueToHex(listSections64.at(i).sh_flags)));
                (*ppModel)->setItem(i,N_Elf_Shdr::sh_addr+1,          new QStandardItem(XBinary::valueToHex(listSections64.at(i).sh_addr)));
                (*ppModel)->setItem(i,N_Elf_Shdr::sh_offset+1,        new QStandardItem(XBinary::valueToHex(listSections64.at(i).sh_offset)));
                (*ppModel)->setItem(i,N_Elf_Shdr::sh_size+1,          new QStandardItem(XBinary::valueToHex(listSections64.at(i).sh_size)));
                (*ppModel)->setItem(i,N_Elf_Shdr::sh_link+1,          new QStandardItem(XBinary::valueToHex(listSections64.at(i).sh_link)));
                (*ppModel)->setItem(i,N_Elf_Shdr::sh_info+1,          new QStandardItem(XBinary::valueToHex(listSections64.at(i).sh_info)));
                (*ppModel)->setItem(i,N_Elf_Shdr::sh_addralign+1,     new QStandardItem(XBinary::valueToHex(listSections64.at(i).sh_addralign)));
                (*ppModel)->setItem(i,N_Elf_Shdr::sh_entsize+1,       new QStandardItem(XBinary::valueToHex(listSections64.at(i).sh_entsize)));
                (*ppModel)->setItem(i,N_Elf_Shdr::sh_entsize+2,       new QStandardItem(pELF->getStringFromIndex(osStringTable.nOffset,osStringTable.nSize,listSections64.at(i).sh_name)));
                (*ppModel)->setItem(i,N_Elf_Shdr::sh_entsize+3,       new QStandardItem(mapTypes.value(listSections64.at(i).sh_type)));
            }
            else
            {
                QStandardItem *pItem=new QStandardItem(QString::number(i));

                if(pELF->isImage())
                {
                    pItem->setData(listSections32.at(i).sh_addr,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET);
                }
                else
                {
                    pItem->setData(listSections32.at(i).sh_offset,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET);
                }

                pItem->setData(listSections32.at(i).sh_size,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE);
                pItem->setData(listSections32.at(i).sh_addr,Qt::UserRole+FW_DEF::SECTION_DATA_ADDRESS);

                (*ppModel)->setItem(i,0,                              pItem);
                (*ppModel)->setItem(i,N_Elf_Shdr::sh_name+1,          new QStandardItem(XBinary::valueToHex(listSections32.at(i).sh_name)));
                (*ppModel)->setItem(i,N_Elf_Shdr::sh_type+1,          new QStandardItem(XBinary::valueToHex(listSections32.at(i).sh_type)));
                (*ppModel)->setItem(i,N_Elf_Shdr::sh_flags+1,         new QStandardItem(XBinary::valueToHex(listSections32.at(i).sh_flags)));
                (*ppModel)->setItem(i,N_Elf_Shdr::sh_addr+1,          new QStandardItem(XBinary::valueToHex(listSections32.at(i).sh_addr)));
                (*ppModel)->setItem(i,N_Elf_Shdr::sh_offset+1,        new QStandardItem(XBinary::valueToHex(listSections32.at(i).sh_offset)));
                (*ppModel)->setItem(i,N_Elf_Shdr::sh_size+1,          new QStandardItem(XBinary::valueToHex(listSections32.at(i).sh_size)));
                (*ppModel)->setItem(i,N_Elf_Shdr::sh_link+1,          new QStandardItem(XBinary::valueToHex(listSections32.at(i).sh_link)));
                (*ppModel)->setItem(i,N_Elf_Shdr::sh_info+1,          new QStandardItem(XBinary::valueToHex(listSections32.at(i).sh_info)));
                (*ppModel)->setItem(i,N_Elf_Shdr::sh_addralign+1,     new QStandardItem(XBinary::valueToHex(listSections32.at(i).sh_addralign)));
                (*ppModel)->setItem(i,N_Elf_Shdr::sh_entsize+1,       new QStandardItem(XBinary::valueToHex(listSections32.at(i).sh_entsize)));
                (*ppModel)->setItem(i,N_Elf_Shdr::sh_entsize+2,       new QStandardItem(pELF->getStringFromIndex(osStringTable.nOffset,osStringTable.nSize,listSections32.at(i).sh_name)));
                (*ppModel)->setItem(i,N_Elf_Shdr::sh_entsize+3,       new QStandardItem(mapTypes.value(listSections32.at(i).sh_type)));
            }

            incValue();
        }
    }
    else if(type==SELF::TYPE_Elf_Phdr)
    {
        bool bIs64=pELF->is64();

        QList<QString> listLabels;
        listLabels.append("");

        if(bIs64)
        {
            listLabels.append(getStructList(N_Elf_Phdr64::records,N_Elf_Phdr64::__data_size));
        }
        else
        {
            listLabels.append(getStructList(N_Elf_Phdr32::records,N_Elf_Phdr32::__data_size));
        }

        listLabels.append("Type");

        QMap<quint64,QString> mapTypes=XELF::getProgramTypesS();

        QList<XELF_DEF::Elf64_Phdr> listPrograms64;
        QList<XELF_DEF::Elf32_Phdr> listPrograms32;

        int nCount=0;

        if(bIs64)
        {
            listPrograms64=pELF->getElf64_PhdrList();
            nCount=listPrograms64.count();
        }
        else
        {
            listPrograms32=pELF->getElf32_PhdrList();
            nCount=listPrograms32.count();
        }

        *ppModel=new QStandardItemModel(nCount,listLabels.count());

        setMaximum(nCount);

        setHeader(*ppModel,&listLabels);

        for(int i=0;(i<nCount)&&(isRun());i++)
        {
            if(bIs64)
            {
                QStandardItem *pItem=new QStandardItem(QString::number(i));

                if(pELF->isImage())
                {
                    pItem->setData(listPrograms64.at(i).p_memsz,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE);
                    pItem->setData(listPrograms64.at(i).p_vaddr,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET);
                }
                else
                {
                    pItem->setData(listPrograms64.at(i).p_filesz,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE);
                    pItem->setData(listPrograms64.at(i).p_offset,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET);
                }

                pItem->setData(listPrograms64.at(i).p_vaddr,Qt::UserRole+FW_DEF::SECTION_DATA_ADDRESS);

                (*ppModel)->setItem(i,0,                              pItem);
                (*ppModel)->setItem(i,N_Elf_Phdr64::p_type+1,         new QStandardItem(XBinary::valueToHex(listPrograms64.at(i).p_type)));
                (*ppModel)->setItem(i,N_Elf_Phdr64::p_flags+1,        new QStandardItem(XBinary::valueToHex(listPrograms64.at(i).p_flags)));
                (*ppModel)->setItem(i,N_Elf_Phdr64::p_offset+1,       new QStandardItem(XBinary::valueToHex(listPrograms64.at(i).p_offset)));
                (*ppModel)->setItem(i,N_Elf_Phdr64::p_vaddr+1,        new QStandardItem(XBinary::valueToHex(listPrograms64.at(i).p_vaddr)));
                (*ppModel)->setItem(i,N_Elf_Phdr64::p_paddr+1,        new QStandardItem(XBinary::valueToHex(listPrograms64.at(i).p_paddr)));
                (*ppModel)->setItem(i,N_Elf_Phdr64::p_filesz+1,       new QStandardItem(XBinary::valueToHex(listPrograms64.at(i).p_filesz)));
                (*ppModel)->setItem(i,N_Elf_Phdr64::p_memsz+1,        new QStandardItem(XBinary::valueToHex(listPrograms64.at(i).p_memsz)));
                (*ppModel)->setItem(i,N_Elf_Phdr64::p_align+1,        new QStandardItem(XBinary::valueToHex(listPrograms64.at(i).p_align)));
                (*ppModel)->setItem(i,N_Elf_Phdr64::p_align+2,        new QStandardItem(mapTypes.value(listPrograms64.at(i).p_type)));
            }
            else
            {
                QStandardItem *pItem=new QStandardItem(QString::number(i));

                if(pELF->isImage())
                {
                    pItem->setData(listPrograms32.at(i).p_memsz,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE);
                    pItem->setData(listPrograms32.at(i).p_vaddr,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET);
                }
                else
                {
                    pItem->setData(listPrograms32.at(i).p_filesz,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE);
                    pItem->setData(listPrograms32.at(i).p_offset,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET);
                }

                pItem->setData(listPrograms32.at(i).p_vaddr,Qt::UserRole+FW_DEF::SECTION_DATA_ADDRESS);

                (*ppModel)->setItem(i,0,                              pItem);
                (*ppModel)->setItem(i,N_Elf_Phdr32::p_type+1,         new QStandardItem(XBinary::valueToHex(listPrograms32.at(i).p_type)));
                (*ppModel)->setItem(i,N_Elf_Phdr32::p_offset+1,       new QStandardItem(XBinary::valueToHex(listPrograms32.at(i).p_offset)));
                (*ppModel)->setItem(i,N_Elf_Phdr32::p_vaddr+1,        new QStandardItem(XBinary::valueToHex(listPrograms32.at(i).p_vaddr)));
                (*ppModel)->setItem(i,N_Elf_Phdr32::p_paddr+1,        new QStandardItem(XBinary::valueToHex(listPrograms32.at(i).p_paddr)));
                (*ppModel)->setItem(i,N_Elf_Phdr32::p_filesz+1,       new QStandardItem(XBinary::valueToHex(listPrograms32.at(i).p_filesz)));
                (*ppModel)->setItem(i,N_Elf_Phdr32::p_memsz+1,        new QStandardItem(XBinary::valueToHex(listPrograms32.at(i).p_memsz)));
                (*ppModel)->setItem(i,N_Elf_Phdr32::p_flags+1,        new QStandardItem(XBinary::valueToHex(listPrograms32.at(i).p_flags)));
                (*ppModel)->setItem(i,N_Elf_Phdr32::p_align+1,        new QStandardItem(XBinary::valueToHex(listPrograms32.at(i).p_align)));
                (*ppModel)->setItem(i,N_Elf_Phdr32::p_align+2,        new QStandardItem(mapTypes.value(listPrograms32.at(i).p_type)));
            }
        }
    }
    else if(type==SELF::TYPE_SYMBOLTABLE)
    {
        if(pELF->is64())
        {
            QList<QString> listLabels;
            listLabels.append("");
            listLabels.append(getStructList(N_Elf64_Sym::records,N_Elf64_Sym::__data_size));
            listLabels.append("Name");

            QList<XELF_DEF::Elf64_Sym> listSymbols=pELF->getElf64_SymList(nOffset,nSize);

            int nCount=listSymbols.count();

            *ppModel=new QStandardItemModel(nCount,listLabels.count());

            setMaximum(nCount);

            setHeader(*ppModel,&listLabels);

            for(int i=0;(i<nCount)&&(isRun());i++)
            {
                QStandardItem *pItem=new QStandardItem;
                pItem->setText(QString::number(i));
                pItem->setTextAlignment(Qt::AlignRight);

                pItem->setData(nOffset+i*sizeof(XELF_DEF::Elf64_Sym),Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET);

                (*ppModel)->setItem(i,0,                                pItem);
                (*ppModel)->setItem(i,N_Elf64_Sym::st_name+1,           new QStandardItem(XBinary::valueToHex(listSymbols.at(i).st_name)));
                (*ppModel)->setItem(i,N_Elf64_Sym::st_info+1,           new QStandardItem(XBinary::valueToHex(listSymbols.at(i).st_info)));
                (*ppModel)->setItem(i,N_Elf64_Sym::st_other+1,          new QStandardItem(XBinary::valueToHex(listSymbols.at(i).st_other)));
                (*ppModel)->setItem(i,N_Elf64_Sym::st_shndx+1,          new QStandardItem(XBinary::valueToHex(listSymbols.at(i).st_shndx)));
                (*ppModel)->setItem(i,N_Elf64_Sym::st_value+1,          new QStandardItem(XBinary::valueToHex(listSymbols.at(i).st_value)));
                (*ppModel)->setItem(i,N_Elf64_Sym::st_size+1,           new QStandardItem(XBinary::valueToHex(listSymbols.at(i).st_size)));

                QString sName=pELF->getStringFromIndex(nStringTableOffset,nStringTableSize,listSymbols.at(i).st_name);

                (*ppModel)->setItem(i,N_Elf64_Sym::st_size+2,           new QStandardItem(sName));

                incValue();
            }
        }
        else
        {
            QList<QString> listLabels;
            listLabels.append("");
            listLabels.append(getStructList(N_Elf32_Sym::records,N_Elf32_Sym::__data_size));
            listLabels.append("Name");

            QList<XELF_DEF::Elf32_Sym> listSymbols=pELF->getElf32_SymList(nOffset,nSize);

            int nCount=listSymbols.count();

            *ppModel=new QStandardItemModel(nCount,listLabels.count());

            setMaximum(nCount);

            setHeader(*ppModel,&listLabels);

            for(int i=0;(i<nCount)&&(isRun());i++)
            {
                QStandardItem *pItem=new QStandardItem;
                pItem->setText(QString::number(i));
                pItem->setTextAlignment(Qt::AlignRight);

                pItem->setData(nOffset+i*sizeof(XELF_DEF::Elf32_Sym),Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET);

                (*ppModel)->setItem(i,0,                                pItem);
                (*ppModel)->setItem(i,N_Elf32_Sym::st_name+1,           new QStandardItem(XBinary::valueToHex(listSymbols.at(i).st_name)));
                (*ppModel)->setItem(i,N_Elf32_Sym::st_value+1,          new QStandardItem(XBinary::valueToHex(listSymbols.at(i).st_value)));
                (*ppModel)->setItem(i,N_Elf32_Sym::st_size+1,           new QStandardItem(XBinary::valueToHex(listSymbols.at(i).st_size)));
                (*ppModel)->setItem(i,N_Elf32_Sym::st_info+1,           new QStandardItem(XBinary::valueToHex(listSymbols.at(i).st_info)));
                (*ppModel)->setItem(i,N_Elf32_Sym::st_other+1,          new QStandardItem(XBinary::valueToHex(listSymbols.at(i).st_other)));
                (*ppModel)->setItem(i,N_Elf32_Sym::st_shndx+1,          new QStandardItem(XBinary::valueToHex(listSymbols.at(i).st_shndx)));

                QString sName=pELF->getStringFromIndex(nStringTableOffset,nStringTableSize,listSymbols.at(i).st_name);

                (*ppModel)->setItem(i,N_Elf32_Sym::st_shndx+2,          new QStandardItem(sName));

                incValue();
            }
        }
    }
    else if(type==SELF::TYPE_Elf_DynamicArrayTags)
    {
        bool bIs64=pELF->is64();

        QList<QString> listLabels;
        listLabels.append("");

        if(bIs64)
        {
            listLabels.append(getStructList(N_Elf_DynamicArrayTags::records64,N_Elf_DynamicArrayTags::__data_size));
        }
        else
        {
            listLabels.append(getStructList(N_Elf_DynamicArrayTags::records32,N_Elf_DynamicArrayTags::__data_size));
        }

        listLabels.append("Type");

        QList<XELF::TAG_STRUCT> listTagStructs=pELF->_getTagStructs(nOffset,nSize,bIs64,pELF->isBigEndian());

        int nCount=listTagStructs.count();

        *ppModel=new QStandardItemModel(nCount,listLabels.count());

        setMaximum(nCount);

        setHeader(*ppModel,&listLabels);

        QMap<quint64,QString> mapTags=pELF->getDynamicTagsS();

        for(int i=0; i<nCount; i++)
        {
            QStandardItem *pItem=new QStandardItem(QString::number(i));

            pItem->setData(listTagStructs.at(i).nOffset,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET);

            (*ppModel)->setItem(i,0,                                      pItem);
            (*ppModel)->setItem(i,N_Elf_DynamicArrayTags::d_tag+1,        new QStandardItem(XBinary::valueToHex(bIs64?((quint64)listTagStructs.at(i).nTag):((quint32)listTagStructs.at(i).nTag))));
            (*ppModel)->setItem(i,N_Elf_DynamicArrayTags::d_value+1,      new QStandardItem(XBinary::valueToHex(bIs64?((quint64)listTagStructs.at(i).nValue):((quint32)listTagStructs.at(i).nValue))));
            (*ppModel)->setItem(i,N_Elf_DynamicArrayTags::d_value+2,      new QStandardItem(mapTags.value(listTagStructs.at(i).nTag)));
        }
    }
}

void ELFProcessData::ajustTableView(QWidget *pWidget,QTableView *pTableView)
{
    int nSymbolWidth=XLineEditHEX::getSymbolWidth(pWidget);

    if(type==SELF::TYPE_SYMBOLTABLE)
    {
        if(pELF->is64())
        {
            pTableView->setColumnWidth(0,nSymbolWidth*4);
            pTableView->setColumnWidth(N_Elf64_Sym::st_name+1,nSymbolWidth*8);
            pTableView->setColumnWidth(N_Elf64_Sym::st_info+1,nSymbolWidth*3);
            pTableView->setColumnWidth(N_Elf64_Sym::st_other+1,nSymbolWidth*3);
            pTableView->setColumnWidth(N_Elf64_Sym::st_shndx+1,nSymbolWidth*4);
            pTableView->setColumnWidth(N_Elf64_Sym::st_value+1,nSymbolWidth*12);
            pTableView->setColumnWidth(N_Elf64_Sym::st_size+1,nSymbolWidth*12);
            pTableView->setColumnWidth(N_Elf64_Sym::st_size+2,nSymbolWidth*35);
        }
        else
        {
            pTableView->setColumnWidth(0,nSymbolWidth*4);
            pTableView->setColumnWidth(N_Elf32_Sym::st_name+1,nSymbolWidth*8);
            pTableView->setColumnWidth(N_Elf32_Sym::st_value+1,nSymbolWidth*8);
            pTableView->setColumnWidth(N_Elf32_Sym::st_size+1,nSymbolWidth*8);
            pTableView->setColumnWidth(N_Elf32_Sym::st_info+1,nSymbolWidth*3);
            pTableView->setColumnWidth(N_Elf32_Sym::st_other+1,nSymbolWidth*3);
            pTableView->setColumnWidth(N_Elf32_Sym::st_shndx+1,nSymbolWidth*4);
            pTableView->setColumnWidth(N_Elf32_Sym::st_shndx+2,nSymbolWidth*35);
        }
    }
    else if(type==SELF::TYPE_Elf_Shdr)
    {
        if(pELF->is64())
        {
            pTableView->setColumnWidth(0,nSymbolWidth*4);
            pTableView->setColumnWidth(1,nSymbolWidth*8);
            pTableView->setColumnWidth(2,nSymbolWidth*8);
            pTableView->setColumnWidth(3,nSymbolWidth*12);
            pTableView->setColumnWidth(4,nSymbolWidth*12);
            pTableView->setColumnWidth(5,nSymbolWidth*12);
            pTableView->setColumnWidth(6,nSymbolWidth*12);
            pTableView->setColumnWidth(7,nSymbolWidth*8);
            pTableView->setColumnWidth(8,nSymbolWidth*8);
            pTableView->setColumnWidth(9,nSymbolWidth*12);
            pTableView->setColumnWidth(10,nSymbolWidth*12);
            pTableView->setColumnWidth(11,nSymbolWidth*12);
            pTableView->setColumnWidth(12,nSymbolWidth*12);
        }
        else
        {
            pTableView->setColumnWidth(0,nSymbolWidth*4);
            pTableView->setColumnWidth(1,nSymbolWidth*8);
            pTableView->setColumnWidth(2,nSymbolWidth*8);
            pTableView->setColumnWidth(3,nSymbolWidth*12);
            pTableView->setColumnWidth(4,nSymbolWidth*12);
            pTableView->setColumnWidth(5,nSymbolWidth*12);
            pTableView->setColumnWidth(6,nSymbolWidth*12);
            pTableView->setColumnWidth(7,nSymbolWidth*8);
            pTableView->setColumnWidth(8,nSymbolWidth*8);
            pTableView->setColumnWidth(9,nSymbolWidth*12);
            pTableView->setColumnWidth(10,nSymbolWidth*12);
            pTableView->setColumnWidth(11,nSymbolWidth*12);
            pTableView->setColumnWidth(12,nSymbolWidth*12);
        }
    }
    else if(type==SELF::TYPE_Elf_Phdr)
    {
        if(pELF->is64())
        {
            pTableView->setColumnWidth(0,nSymbolWidth*4);
            pTableView->setColumnWidth(1,nSymbolWidth*8);
            pTableView->setColumnWidth(2,nSymbolWidth*8);
            pTableView->setColumnWidth(3,nSymbolWidth*12);
            pTableView->setColumnWidth(4,nSymbolWidth*12);
            pTableView->setColumnWidth(5,nSymbolWidth*12);
            pTableView->setColumnWidth(6,nSymbolWidth*12);
            pTableView->setColumnWidth(7,nSymbolWidth*12);
            pTableView->setColumnWidth(8,nSymbolWidth*12);
            pTableView->setColumnWidth(9,nSymbolWidth*12);
        }
        else
        {
            pTableView->setColumnWidth(0,nSymbolWidth*4);
            pTableView->setColumnWidth(1,nSymbolWidth*8);
            pTableView->setColumnWidth(2,nSymbolWidth*8);
            pTableView->setColumnWidth(3,nSymbolWidth*12);
            pTableView->setColumnWidth(4,nSymbolWidth*12);
            pTableView->setColumnWidth(5,nSymbolWidth*12);
            pTableView->setColumnWidth(6,nSymbolWidth*12);
            pTableView->setColumnWidth(7,nSymbolWidth*12);
            pTableView->setColumnWidth(8,nSymbolWidth*12);
            pTableView->setColumnWidth(9,nSymbolWidth*12);
        }
    }
    else if(type==SELF::TYPE_Elf_DynamicArrayTags)
    {
        pTableView->setColumnWidth(0,nSymbolWidth*4);
        pTableView->setColumnWidth(1,nSymbolWidth*12);
        pTableView->setColumnWidth(2,nSymbolWidth*12);
        pTableView->setColumnWidth(3,nSymbolWidth*20);
    }
}

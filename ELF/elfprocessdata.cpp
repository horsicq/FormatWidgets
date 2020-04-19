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

ELFProcessData::ELFProcessData(int type, QStandardItemModel **ppModel, XELF *pELF, qint64 nOffset, qint64 nSize)
{
    this->type=type;
    this->ppModel=ppModel;
    this->pELF=pELF;
    this->nOffset=nOffset;
    this->nSize=nSize;
}

void ELFProcessData::_process()
{
    if(*ppModel)
    {
        delete (*ppModel);
    }

    if(type==SELF::TYPE_SYMBOLTABLE)
    {
        if(pELF->is64())
        {
            QList<XELF_DEF::Elf64_Sym> listSymbols=pELF->getElf64_SymList(nOffset,nSize);

            int nCount=listSymbols.count();

            *ppModel=new QStandardItemModel(nCount,N_Elf64_Sym::__data_size+1);

            setMaximum(nCount);

            for(int i=0;(i<nCount)&&(isRun());i++)
            {

                QStandardItem *pItem=new QStandardItem;
                pItem->setText(QString::number(i));
                pItem->setTextAlignment(Qt::AlignRight);

                pItem->setData(Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET,nOffset+i*sizeof(XELF_DEF::Elf64_Sym));

                (*ppModel)->setItem(i,0,pItem);
                (*ppModel)->setItem(i,N_Elf64_Sym::st_name+1,          new QStandardItem(XBinary::valueToHex(listSymbols.at(i).st_name)));
                (*ppModel)->setItem(i,N_Elf64_Sym::st_info+1,          new QStandardItem(XBinary::valueToHex(listSymbols.at(i).st_info)));
                (*ppModel)->setItem(i,N_Elf64_Sym::st_other+1,         new QStandardItem(XBinary::valueToHex(listSymbols.at(i).st_other)));
                (*ppModel)->setItem(i,N_Elf64_Sym::st_shndx+1,         new QStandardItem(XBinary::valueToHex(listSymbols.at(i).st_shndx)));
                (*ppModel)->setItem(i,N_Elf64_Sym::st_value+1,         new QStandardItem(XBinary::valueToHex(listSymbols.at(i).st_value)));
                (*ppModel)->setItem(i,N_Elf64_Sym::st_size+1,          new QStandardItem(XBinary::valueToHex(listSymbols.at(i).st_size)));

                increment();
            }
        }
    }
}

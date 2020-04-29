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
#include "peprocessdata.h"

PEProcessData::PEProcessData(int type, QStandardItemModel **ppModel, XPE *pPE, qint64 nOffset, qint64 nSize)
{
    this->type=type;
    this->ppModel=ppModel;
    this->pPE=pPE;
    this->nOffset=nOffset;
    this->nSize=nSize;
}

void PEProcessData::_process()
{
    if(type==SPE::TYPE_RELOCS_POSITION)
    {
        QList<QString> listLabels;
        listLabels.append(getStructList(N_IMAGE_RELOCS_POSITION::records,N_IMAGE_RELOCS_POSITION::__data_size));
        listLabels.append("Type");
        listLabels.append("Address");

        QList<XPE::RELOCS_POSITION> listRelocsPositions=pPE->getRelocsPositions(nOffset);

        int nCount=listRelocsPositions.count();

        *ppModel=new QStandardItemModel(nCount,listLabels.count());

        setMaximum(nCount);

        setHeader(*ppModel,&listLabels);

        QMap<quint64,QString> mapTypes=pPE->getImageRelBasedS();

        for(int i=0; i<nCount; i++)
        {
            (*ppModel)->setItem(i,N_IMAGE_RELOCS_POSITION::TypeOffset,       new QStandardItem(XBinary::valueToHex(listRelocsPositions.at(i).nTypeOffset)));
            (*ppModel)->setItem(i,N_IMAGE_RELOCS_POSITION::TypeOffset+1,     new QStandardItem(mapTypes.value(listRelocsPositions.at(i).nType)));
            (*ppModel)->setItem(i,N_IMAGE_RELOCS_POSITION::TypeOffset+2,     new QStandardItem(XBinary::valueToHex((quint32)listRelocsPositions.at(i).nAddress)));

            incValue();
        }
    }
}

void PEProcessData::ajustTableView(QWidget *pWidget, QTableView *pTableView)
{
    int nSymbolWidth=XLineEditHEX::getSymbolWidth(pWidget);

    if(type==SPE::TYPE_RELOCS_POSITION)
    {
        pTableView->setColumnWidth(0,nSymbolWidth*6);
        pTableView->setColumnWidth(1,nSymbolWidth*12);
        pTableView->setColumnWidth(2,nSymbolWidth*8);
    }
}

// copyright (c) 2017-2021 hors<horsicq@gmail.com>
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
#include "machprocessdata.h"


MACHProcessData::MACHProcessData(int nType, QStandardItemModel **ppModel, XMACH *pXMACH, qint64 nOffset, qint64 nSize) : ProcessData()
{
    this->g_nType=nType;
    this->g_ppModel=ppModel;
    this->g_pXMACH=pXMACH;
    this->g_nOffset=nOffset;
    this->g_nSize=nSize;
}

void MACHProcessData::_process()
{
    if(g_nType==SMACH::TYPE_mach_commands)
    {
        QList<QString> listLabels;
        listLabels.append("");
        listLabels.append(getStructList(N_mach_commands::records,N_mach_commands::__data_size));
        listLabels.append("");

        QList<XMACH::COMMAND_RECORD> listCommandRecords=g_pXMACH->getCommandRecords();
        QMap<quint64,QString> mapLC=g_pXMACH->getLoadCommandTypesS();

        int nNumberOfRecords=listCommandRecords.count();

        *g_ppModel=new QStandardItemModel(nNumberOfRecords,listLabels.count());

        setMaximum(nNumberOfRecords);

        setHeader(*g_ppModel,&listLabels);

        for(int i=0; i<nNumberOfRecords; i++)
        {
            QStandardItem *pItem=new QStandardItem;
            pItem->setData(i,Qt::DisplayRole);
            pItem->setData(listCommandRecords.at(i).nOffset,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET);
            pItem->setData(listCommandRecords.at(i).nSize,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE);
            pItem->setData(listCommandRecords.at(i).nOffset,Qt::UserRole+FW_DEF::SECTION_DATA_ADDRESS);
            (*g_ppModel)->setItem(i,0,                                          pItem);
            (*g_ppModel)->setItem(i,N_mach_commands::cmd+1,                     new QStandardItem(XBinary::valueToHex((quint32)listCommandRecords.at(i).nType)));
            (*g_ppModel)->setItem(i,N_mach_commands::cmdsize+1,                 new QStandardItem(XBinary::valueToHex((quint32)listCommandRecords.at(i).nSize)));
            (*g_ppModel)->setItem(i,N_mach_commands::cmdsize+2,                 new QStandardItem(mapLC.value(listCommandRecords.at(i).nType)));

            incValue();
        }
    }
}

void MACHProcessData::ajustTableView(QWidget *pWidget, QTableView *pTableView)
{
    int nSymbolWidth=XLineEditHEX::getSymbolWidth(pWidget);

    if(g_nType==SMACH::TYPE_mach_commands)
    {
        pTableView->setColumnWidth(0,nSymbolWidth*4);
        pTableView->setColumnWidth(1,nSymbolWidth*10);
        pTableView->setColumnWidth(2,nSymbolWidth*10);
        pTableView->setColumnWidth(3,nSymbolWidth*20);
    }
}

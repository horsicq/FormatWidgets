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
            pItem->setData(listCommandRecords.at(i).nOffset,Qt::UserRole+FW_DEF::SECTION_DATA_HEADEROFFSET);
            pItem->setData(listCommandRecords.at(i).nSize,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE);
            pItem->setData(listCommandRecords.at(i).nOffset,Qt::UserRole+FW_DEF::SECTION_DATA_ADDRESS);
            (*g_ppModel)->setItem(i,0,                                          pItem);
            (*g_ppModel)->setItem(i,N_mach_commands::cmd+1,                     new QStandardItem(XBinary::valueToHex((quint32)listCommandRecords.at(i).nType)));
            (*g_ppModel)->setItem(i,N_mach_commands::cmdsize+1,                 new QStandardItem(XBinary::valueToHex((quint32)listCommandRecords.at(i).nSize)));
            (*g_ppModel)->setItem(i,N_mach_commands::cmdsize+2,                 new QStandardItem(mapLC.value(listCommandRecords.at(i).nType)));

            incValue();
        }
    }
    else if(g_nType==SMACH::TYPE_mach_libraries)
    {
        QList<QString> listLabels;
        listLabels.append("");
        listLabels.append(getStructList(N_mach_libraries::records,N_mach_libraries::__data_size));
        listLabels.append(tr("Library"));

        QList<XMACH::LIBRARY_RECORD> listLibraries=g_pXMACH->getLibraryRecords();

        int nNumberOfRecords=listLibraries.count();

        *g_ppModel=new QStandardItemModel(nNumberOfRecords,listLabels.count());

        setMaximum(nNumberOfRecords);

        setHeader(*g_ppModel,&listLabels);

        for(int i=0; i<nNumberOfRecords; i++)
        {
            QStandardItem *pItem=new QStandardItem;
            pItem->setData(i,Qt::DisplayRole);
            (*g_ppModel)->setItem(i,0,                                              pItem);
            (*g_ppModel)->setItem(i,N_mach_libraries::timestamp+1,                  new QStandardItem(XBinary::valueToHex(listLibraries.at(i).timestamp)));
            (*g_ppModel)->setItem(i,N_mach_libraries::current_version+1,            new QStandardItem(XBinary::valueToHex(listLibraries.at(i).current_version)));
            (*g_ppModel)->setItem(i,N_mach_libraries::compatibility_version+1,      new QStandardItem(XBinary::valueToHex(listLibraries.at(i).compatibility_version)));
            (*g_ppModel)->setItem(i,N_mach_libraries::compatibility_version+2,      new QStandardItem(listLibraries.at(i).sFullName));

            incValue();
        }
    }
    else if(g_nType==SMACH::TYPE_mach_segments)
    {
        bool bIs64=g_pXMACH->is64();

        QList<QString> listLabels;
        listLabels.append("");

        if(bIs64)
        {
            listLabels.append(getStructList(N_mach_segments::records64,N_mach_segments::__data_size));
        }
        else
        {
            listLabels.append(getStructList(N_mach_segments::records32,N_mach_segments::__data_size));
        }

        QList<XMACH::SEGMENT_RECORD> listSegmentRecords=g_pXMACH->getSegmentRecords();

        int nNumberOfRecords=listSegmentRecords.count();

        *g_ppModel=new QStandardItemModel(nNumberOfRecords,listLabels.count());

        setMaximum(nNumberOfRecords);

        setHeader(*g_ppModel,&listLabels);

        for(int i=0; i<nNumberOfRecords; i++)
        {
            QStandardItem *pItem=new QStandardItem;
            pItem->setData(i,Qt::DisplayRole);

            if(g_pXMACH->isImage())
            {
                pItem->setData(listSegmentRecords.at(i).vmaddr,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET);
                pItem->setData(listSegmentRecords.at(i).vmsize,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE);
            }
            else
            {
                pItem->setData(listSegmentRecords.at(i).fileoff,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET);
                pItem->setData(listSegmentRecords.at(i).filesize,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE);
            }

            pItem->setData(listSegmentRecords.at(i).vmaddr,Qt::UserRole+FW_DEF::SECTION_DATA_ADDRESS);
            pItem->setData(listSegmentRecords.at(i).nStructOffset,Qt::UserRole+FW_DEF::SECTION_DATA_HEADEROFFSET);

            (*g_ppModel)->setItem(i,0,                                  pItem);
            (*g_ppModel)->setItem(i,N_mach_segments::segname+1,         new QStandardItem(listSegmentRecords.at(i).segname));

            if(bIs64)
            {
                (*g_ppModel)->setItem(i,N_mach_segments::vmaddr+1,      new QStandardItem(XBinary::valueToHex((quint64)listSegmentRecords.at(i).vmaddr)));
                (*g_ppModel)->setItem(i,N_mach_segments::vmsize+1,      new QStandardItem(XBinary::valueToHex((quint64)listSegmentRecords.at(i).vmsize)));
                (*g_ppModel)->setItem(i,N_mach_segments::fileoff+1,     new QStandardItem(XBinary::valueToHex((quint64)listSegmentRecords.at(i).fileoff)));
                (*g_ppModel)->setItem(i,N_mach_segments::filesize+1,    new QStandardItem(XBinary::valueToHex((quint64)listSegmentRecords.at(i).filesize)));
            }
            else
            {
                (*g_ppModel)->setItem(i,N_mach_segments::vmaddr+1,      new QStandardItem(XBinary::valueToHex((quint32)listSegmentRecords.at(i).vmaddr)));
                (*g_ppModel)->setItem(i,N_mach_segments::vmsize+1,      new QStandardItem(XBinary::valueToHex((quint32)listSegmentRecords.at(i).vmsize)));
                (*g_ppModel)->setItem(i,N_mach_segments::fileoff+1,     new QStandardItem(XBinary::valueToHex((quint32)listSegmentRecords.at(i).fileoff)));
                (*g_ppModel)->setItem(i,N_mach_segments::filesize+1,    new QStandardItem(XBinary::valueToHex((quint32)listSegmentRecords.at(i).filesize)));
            }

            (*g_ppModel)->setItem(i,N_mach_segments::maxprot+1,         new QStandardItem(XBinary::valueToHex((quint32)listSegmentRecords.at(i).maxprot)));
            (*g_ppModel)->setItem(i,N_mach_segments::initprot+1,        new QStandardItem(XBinary::valueToHex((quint32)listSegmentRecords.at(i).initprot)));
            (*g_ppModel)->setItem(i,N_mach_segments::nsects+1,          new QStandardItem(XBinary::valueToHex((quint32)listSegmentRecords.at(i).nsects)));
            (*g_ppModel)->setItem(i,N_mach_segments::flags+1,           new QStandardItem(XBinary::valueToHex((quint32)listSegmentRecords.at(i).flags)));

            incValue();
        }
    }
    else if(g_nType==SMACH::TYPE_mach_sections)
    {
        bool bIs64=g_pXMACH->is64();

        QList<QString> listLabels;
        listLabels.append("");

        if(bIs64)
        {
            listLabels.append(getStructList(N_mach_sections::records64,N_mach_segments::__data_size));
        }
        else
        {
            listLabels.append(getStructList(N_mach_sections::records32,N_mach_segments::__data_size));
        }

        QList<XMACH::SECTION_RECORD> listSectionRecords=g_pXMACH->getSectionRecords();

        int nNumberOfRecords=listSectionRecords.count();

        *g_ppModel=new QStandardItemModel(nNumberOfRecords,listLabels.count());

        setMaximum(nNumberOfRecords);

        setHeader(*g_ppModel,&listLabels);

        for(int i=0; i<nNumberOfRecords; i++)
        {
            QStandardItem *pItem=new QStandardItem;
            pItem->setData(i,Qt::DisplayRole);

            if(g_pXMACH->isImage())
            {
                pItem->setData(listSectionRecords.at(i).addr,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET);
            }
            else
            {
                pItem->setData(listSectionRecords.at(i).offset,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET);
            }

            pItem->setData(listSectionRecords.at(i).size,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE);
            pItem->setData(listSectionRecords.at(i).addr,Qt::UserRole+FW_DEF::SECTION_DATA_ADDRESS);
            pItem->setData(listSectionRecords.at(i).nStructOffset,Qt::UserRole+FW_DEF::SECTION_DATA_HEADEROFFSET);

            (*g_ppModel)->setItem(i,0,                                  pItem);
            (*g_ppModel)->setItem(i,N_mach_sections::segname+1,         new QStandardItem(listSectionRecords.at(i).segname));
            (*g_ppModel)->setItem(i,N_mach_sections::sectname+1,        new QStandardItem(listSectionRecords.at(i).sectname));

            if(bIs64)
            {
                (*g_ppModel)->setItem(i,N_mach_sections::addr+1,        new QStandardItem(XBinary::valueToHex((quint64)listSectionRecords.at(i).addr)));
                (*g_ppModel)->setItem(i,N_mach_sections::size+1,        new QStandardItem(XBinary::valueToHex((quint64)listSectionRecords.at(i).size)));
            }
            else
            {
                (*g_ppModel)->setItem(i,N_mach_sections::addr+1,        new QStandardItem(XBinary::valueToHex((quint32)listSectionRecords.at(i).addr)));
                (*g_ppModel)->setItem(i,N_mach_sections::size+1,        new QStandardItem(XBinary::valueToHex((quint32)listSectionRecords.at(i).size)));
            }

            (*g_ppModel)->setItem(i,N_mach_sections::offset+1,          new QStandardItem(XBinary::valueToHex((quint32)listSectionRecords.at(i).offset)));
            (*g_ppModel)->setItem(i,N_mach_sections::align+1,           new QStandardItem(XBinary::valueToHex((quint32)listSectionRecords.at(i).align)));
            (*g_ppModel)->setItem(i,N_mach_sections::reloff+1,          new QStandardItem(XBinary::valueToHex((quint32)listSectionRecords.at(i).reloff)));
            (*g_ppModel)->setItem(i,N_mach_sections::nreloc+1,          new QStandardItem(XBinary::valueToHex((quint32)listSectionRecords.at(i).nreloc)));
            (*g_ppModel)->setItem(i,N_mach_sections::flags+1,           new QStandardItem(XBinary::valueToHex((quint32)listSectionRecords.at(i).flags)));

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
    else if(g_nType==SMACH::TYPE_mach_libraries)
    {
        pTableView->setColumnWidth(0,nSymbolWidth*4);
        pTableView->setColumnWidth(1,nSymbolWidth*10);
        pTableView->setColumnWidth(2,nSymbolWidth*10);
        pTableView->setColumnWidth(3,nSymbolWidth*10);
        pTableView->setColumnWidth(4,nSymbolWidth*45);
    }
    else if(g_nType==SMACH::TYPE_mach_segments)
    {
        pTableView->setColumnWidth(0,nSymbolWidth*4);
        pTableView->setColumnWidth(1,nSymbolWidth*8);
        pTableView->setColumnWidth(2,nSymbolWidth*12);
        pTableView->setColumnWidth(3,nSymbolWidth*12);
        pTableView->setColumnWidth(4,nSymbolWidth*12);
        pTableView->setColumnWidth(5,nSymbolWidth*12);
        pTableView->setColumnWidth(6,nSymbolWidth*8);
        pTableView->setColumnWidth(7,nSymbolWidth*8);
        pTableView->setColumnWidth(8,nSymbolWidth*8);
        pTableView->setColumnWidth(9,nSymbolWidth*8);
    }
    else if(g_nType==SMACH::TYPE_mach_sections)
    {
        pTableView->setColumnWidth(0,nSymbolWidth*4);
        pTableView->setColumnWidth(1,nSymbolWidth*12);
        pTableView->setColumnWidth(2,nSymbolWidth*8);
        pTableView->setColumnWidth(3,nSymbolWidth*12);
        pTableView->setColumnWidth(4,nSymbolWidth*12);
        pTableView->setColumnWidth(5,nSymbolWidth*8);
        pTableView->setColumnWidth(6,nSymbolWidth*8);
        pTableView->setColumnWidth(7,nSymbolWidth*8);
        pTableView->setColumnWidth(8,nSymbolWidth*8);
        pTableView->setColumnWidth(9,nSymbolWidth*8);
    }
}

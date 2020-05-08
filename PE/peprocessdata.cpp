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

PEProcessData::PEProcessData(int type, QStandardItemModel **ppModel, XPE *pPE, qint32 nNumber, qint64 nOffset, qint64 nSize)
{
    this->type=type;
    this->ppModel=ppModel;
    this->pPE=pPE;
    this->nNumber=nNumber;
    this->nOffset=nOffset;
    this->nSize=nSize;
}

void PEProcessData::_process()
{
    if(type==SPE::TYPE_SECTIONS)
    {
        QList<QString> listLabels;
        listLabels.append("");
        listLabels.append(getStructList(N_IMAGE_SECTION_HEADER::records,N_IMAGE_SECTION_HEADER::__data_size));

        QList<XPE_DEF::IMAGE_SECTION_HEADER> listSections=pPE->getSectionHeaders();

        int nCount=listSections.count();

        *ppModel=new QStandardItemModel(nCount,listLabels.count());

        setMaximum(nCount);

        setHeader(*ppModel,&listLabels);

        for(int i=0; i<nCount; i++)
        {
            QStandardItem *itemNumber=new QStandardItem(QString::number(i));

            itemNumber->setData(listSections.at(i).VirtualAddress,Qt::UserRole+FW_DEF::SECTION_DATA_ADDRESS);

            if(pPE->isImage())
            {
                itemNumber->setData(listSections.at(i).Misc.VirtualSize,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE);
                itemNumber->setData(listSections.at(i).VirtualAddress,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET);
            }
            else
            {
                itemNumber->setData(listSections.at(i).SizeOfRawData,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE);
                itemNumber->setData(listSections.at(i).PointerToRawData,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET);
            }

            (*ppModel)->setItem(i,0,itemNumber);

            QStandardItem *itemName=new QStandardItem();
            QString sName=QString((char *)listSections.at(i).Name);
            sName.resize(qMin(sName.length(),XPE_DEF::S_IMAGE_SIZEOF_SHORT_NAME));
            itemName->setText(sName);
            (*ppModel)->setItem(i,N_IMAGE_SECTION_HEADER::Name+1,itemName);

            (*ppModel)->setItem(i,N_IMAGE_SECTION_HEADER::VirtualSize+1,          new QStandardItem(XBinary::valueToHex(listSections.at(i).Misc.VirtualSize)));
            (*ppModel)->setItem(i,N_IMAGE_SECTION_HEADER::VirtualAddress+1,       new QStandardItem(XBinary::valueToHex(listSections.at(i).VirtualAddress)));
            (*ppModel)->setItem(i,N_IMAGE_SECTION_HEADER::SizeOfRawData+1,        new QStandardItem(XBinary::valueToHex(listSections.at(i).SizeOfRawData)));
            (*ppModel)->setItem(i,N_IMAGE_SECTION_HEADER::PointerToRawData+1,     new QStandardItem(XBinary::valueToHex(listSections.at(i).PointerToRawData)));
            (*ppModel)->setItem(i,N_IMAGE_SECTION_HEADER::PointerToRelocations+1, new QStandardItem(XBinary::valueToHex(listSections.at(i).PointerToRelocations)));
            (*ppModel)->setItem(i,N_IMAGE_SECTION_HEADER::PointerToLinenumbers+1, new QStandardItem(XBinary::valueToHex(listSections.at(i).PointerToLinenumbers)));
            (*ppModel)->setItem(i,N_IMAGE_SECTION_HEADER::NumberOfRelocations+1,  new QStandardItem(XBinary::valueToHex(listSections.at(i).NumberOfRelocations)));
            (*ppModel)->setItem(i,N_IMAGE_SECTION_HEADER::NumberOfLinenumbers+1,  new QStandardItem(XBinary::valueToHex(listSections.at(i).NumberOfLinenumbers)));
            (*ppModel)->setItem(i,N_IMAGE_SECTION_HEADER::Characteristics+1,      new QStandardItem(XBinary::valueToHex(listSections.at(i).Characteristics)));
        }
    }
    else if(type==SPE::TYPE_RELOCS_POSITION)
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
    else if(type==SPE::TYPE_IMPORT)
    {
        QList<QString> listLabels;
        listLabels.append(getStructList(N_IMAGE_IMPORT::records,N_IMAGE_IMPORT::__data_size));
        listLabels.append("");

        QList<XPE::IMAGE_IMPORT_DESCRIPTOR_EX> listID=pPE->getImportDescriptorsEx();

        int nCount=listID.count();

        *ppModel=new QStandardItemModel(nCount,listLabels.count());

        setMaximum(nCount);

        setHeader(*ppModel,&listLabels);

        for(int i=0; i<nCount; i++)
        {
            (*ppModel)->setItem(i,N_IMAGE_IMPORT::OriginalFirstThunk,              new QStandardItem(XBinary::valueToHex(listID.at(i).OriginalFirstThunk)));
            (*ppModel)->setItem(i,N_IMAGE_IMPORT::TimeDateStamp,                   new QStandardItem(XBinary::valueToHex(listID.at(i).TimeDateStamp)));
            (*ppModel)->setItem(i,N_IMAGE_IMPORT::ForwarderChain,                  new QStandardItem(XBinary::valueToHex(listID.at(i).ForwarderChain)));
            (*ppModel)->setItem(i,N_IMAGE_IMPORT::Name,                            new QStandardItem(XBinary::valueToHex(listID.at(i).Name)));
            (*ppModel)->setItem(i,N_IMAGE_IMPORT::FirstThunk,                      new QStandardItem(XBinary::valueToHex(listID.at(i).FirstThunk)));
            (*ppModel)->setItem(i,N_IMAGE_IMPORT::FirstThunk+1,                    new QStandardItem(listID.at(i).sLibrary));

            incValue();
        }
    }
    else if(type==SPE::TYPE_IMPORT_FUNCTION)
    {
        QList<QString> listLabels;
        listLabels.append("");
        listLabels.append(getStructList(N_IMAGE_IMPORT_FUNCTION::records32,N_IMAGE_IMPORT_FUNCTION::__data_size));
        listLabels.append("Name");

        bool bIs64=pPE->is64();
        QList<XPE::IMPORT_POSITION> listIP=pPE->getImportPositions(nNumber);

        int nCount=listIP.count();

        *ppModel=new QStandardItemModel(nCount,listLabels.count());

        setMaximum(nCount);

        setHeader(*ppModel,&listLabels);

        for(int i=0; i<nCount; i++)
        {
            QStandardItem *pItem=new QStandardItem(QString::number(i));

            (*ppModel)->setItem(i,0,pItem);

            if(listIP.at(i).nOrdinal)
            {
                QString sOrdinal;

                if(bIs64)
                {
                    sOrdinal=XBinary::valueToHex((quint64)listIP.at(i).nOrdinal);
                }
                else
                {
                    sOrdinal=XBinary::valueToHex((quint32)listIP.at(i).nOrdinal);
                }

                (*ppModel)->setItem(i,N_IMAGE_IMPORT_FUNCTION::Ordinal+1,   new QStandardItem(sOrdinal));
            }
            else
            {
                QString sThunk;

                if(bIs64)
                {
                    sThunk=XBinary::valueToHex((quint64)listIP.at(i).nThunkValue);
                }
                else
                {
                    sThunk=XBinary::valueToHex((quint32)listIP.at(i).nThunkValue);
                }

                (*ppModel)->setItem(i,N_IMAGE_IMPORT_FUNCTION::Thunk+1,     new QStandardItem(sThunk));
                (*ppModel)->setItem(i,N_IMAGE_IMPORT_FUNCTION::Hint+1,      new QStandardItem(XBinary::valueToHex(listIP.at(i).nHint)));
                (*ppModel)->setItem(i,N_IMAGE_IMPORT_FUNCTION::Hint+2,      new QStandardItem(listIP.at(i).sName));
            }

            incValue();
        }
    }
    else if(type==SPE::TYPE_EXCEPTION)
    {
        QList<QString> listLabels;
        listLabels.append(getStructList(N_IMAGE_EXCEPTIONS::records,N_IMAGE_EXCEPTIONS::__data_size));

        QList<XPE_DEF::S_IMAGE_RUNTIME_FUNCTION_ENTRY> listRFE=pPE->getExceptionsList();

        int nCount=listRFE.count();

        *ppModel=new QStandardItemModel(nCount,listLabels.count());

        setMaximum(nCount);

        setHeader(*ppModel,&listLabels);

        XBinary::_MEMORY_MAP memoryMap=pPE->getMemoryMap();

        for(int i=0; i<nCount; i++)
        {
            QStandardItem *pItem=new QStandardItem(XBinary::valueToHex(listRFE.at(i).BeginAddress));
//                pItem->setData(Qt::UserRole,listRH.at(i).nOffset);

            pItem->setData(listRFE.at(i).BeginAddress,Qt::UserRole+FW_DEF::SECTION_DATA_ADDRESS);
            pItem->setData(listRFE.at(i).EndAddress-listRFE.at(i).BeginAddress,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE);
            pItem->setData(pPE->addressToOffset(&memoryMap,memoryMap.nBaseAddress+listRFE.at(i).BeginAddress),Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET);

            (*ppModel)->setItem(i,N_IMAGE_EXCEPTIONS::BeginAddress,         pItem);
            (*ppModel)->setItem(i,N_IMAGE_EXCEPTIONS::EndAddress,           new QStandardItem(XBinary::valueToHex(listRFE.at(i).EndAddress)));
            (*ppModel)->setItem(i,N_IMAGE_EXCEPTIONS::UnwindInfoAddress,    new QStandardItem(XBinary::valueToHex(listRFE.at(i).UnwindInfoAddress)));

            incValue();
        }
    }
    else if(type==SPE::TYPE_DELAYIMPORT)
    {
        QList<QString> listLabels;
        listLabels.append(getStructList(N_IMAGE_DELAYIMPORT::records,N_IMAGE_DELAYIMPORT::__data_size));

        QList<XPE_DEF::S_IMAGE_DELAYLOAD_DESCRIPTOR> listDelayImport=pPE->getDelayImportsList();

        int nCount=listDelayImport.count();

        *ppModel=new QStandardItemModel(nCount,listLabels.count());

        setMaximum(nCount);

        setHeader(*ppModel,&listLabels);

        for(int i=0; i<nCount; i++)
        {
            QStandardItem *pItem=new QStandardItem(XBinary::valueToHex(listDelayImport.at(i).AllAttributes));
            (*ppModel)->setItem(i,N_IMAGE_DELAYIMPORT::AllAttributes,                  pItem);
            (*ppModel)->setItem(i,N_IMAGE_DELAYIMPORT::DllNameRVA,                     new QStandardItem(XBinary::valueToHex(listDelayImport.at(i).DllNameRVA)));
            (*ppModel)->setItem(i,N_IMAGE_DELAYIMPORT::ModuleHandleRVA,                new QStandardItem(XBinary::valueToHex(listDelayImport.at(i).ModuleHandleRVA)));
            (*ppModel)->setItem(i,N_IMAGE_DELAYIMPORT::ImportAddressTableRVA,          new QStandardItem(XBinary::valueToHex(listDelayImport.at(i).ImportAddressTableRVA)));
            (*ppModel)->setItem(i,N_IMAGE_DELAYIMPORT::ImportNameTableRVA,             new QStandardItem(XBinary::valueToHex(listDelayImport.at(i).ImportNameTableRVA)));
            (*ppModel)->setItem(i,N_IMAGE_DELAYIMPORT::BoundImportAddressTableRVA,     new QStandardItem(XBinary::valueToHex(listDelayImport.at(i).BoundImportAddressTableRVA)));
            (*ppModel)->setItem(i,N_IMAGE_DELAYIMPORT::UnloadInformationTableRVA,      new QStandardItem(XBinary::valueToHex(listDelayImport.at(i).UnloadInformationTableRVA)));
            (*ppModel)->setItem(i,N_IMAGE_DELAYIMPORT::TimeDateStamp,                  new QStandardItem(XBinary::valueToHex(listDelayImport.at(i).TimeDateStamp)));

            incValue();
        }
    }
}

void PEProcessData::ajustTableView(QWidget *pWidget, QTableView *pTableView)
{
    int nSymbolWidth=XLineEditHEX::getSymbolWidth(pWidget);

    if(type==SPE::TYPE_SECTIONS)
    {
        pTableView->setColumnWidth(0,nSymbolWidth*4);
        pTableView->setColumnWidth(1,nSymbolWidth*8);
        pTableView->setColumnWidth(2,nSymbolWidth*8);
        pTableView->setColumnWidth(3,nSymbolWidth*8);
        pTableView->setColumnWidth(4,nSymbolWidth*8);
        pTableView->setColumnWidth(5,nSymbolWidth*8);
        pTableView->setColumnWidth(6,nSymbolWidth*8);
        pTableView->setColumnWidth(7,nSymbolWidth*8);
        pTableView->setColumnWidth(8,nSymbolWidth*8);
        pTableView->setColumnWidth(9,nSymbolWidth*8);
        pTableView->setColumnWidth(10,nSymbolWidth*8);
    }
    else if(type==SPE::TYPE_RELOCS_POSITION)
    {
        pTableView->setColumnWidth(0,nSymbolWidth*8);
        pTableView->setColumnWidth(1,nSymbolWidth*12);
        pTableView->setColumnWidth(2,nSymbolWidth*12);
    }
    else if(type==SPE::TYPE_IMPORT)
    {
        pTableView->setColumnWidth(0,nSymbolWidth*8);
        pTableView->setColumnWidth(1,nSymbolWidth*8);
        pTableView->setColumnWidth(2,nSymbolWidth*8);
        pTableView->setColumnWidth(3,nSymbolWidth*8);
        pTableView->setColumnWidth(4,nSymbolWidth*8);
        pTableView->setColumnWidth(5,nSymbolWidth*12);
    }
    else if(type==SPE::TYPE_IMPORT_FUNCTION)
    {
        pTableView->setColumnWidth(0,nSymbolWidth*4);
        pTableView->setColumnWidth(1,nSymbolWidth*12);
        pTableView->setColumnWidth(2,nSymbolWidth*8);
        pTableView->setColumnWidth(3,nSymbolWidth*6);
        pTableView->setColumnWidth(4,nSymbolWidth*22);
    }
    else if(type==SPE::TYPE_EXCEPTION)
    {
        pTableView->setColumnWidth(0,nSymbolWidth*4);
        pTableView->setColumnWidth(1,nSymbolWidth*12);
        pTableView->setColumnWidth(2,nSymbolWidth*8);
        pTableView->setColumnWidth(3,nSymbolWidth*6);
        pTableView->setColumnWidth(4,nSymbolWidth*22);
    }
    else if(type==SPE::TYPE_DELAYIMPORT)
    {
        pTableView->setColumnWidth(0,nSymbolWidth*8);
        pTableView->setColumnWidth(1,nSymbolWidth*8);
        pTableView->setColumnWidth(2,nSymbolWidth*8);
        pTableView->setColumnWidth(3,nSymbolWidth*8);
        pTableView->setColumnWidth(4,nSymbolWidth*8);
        pTableView->setColumnWidth(5,nSymbolWidth*8);
        pTableView->setColumnWidth(6,nSymbolWidth*8);
        pTableView->setColumnWidth(7,nSymbolWidth*8);
    }
}

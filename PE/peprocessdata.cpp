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
            QStandardItem *itemNumber=new QStandardItem;
            itemNumber->setData(i,Qt::DisplayRole);

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

            incValue();
        }
    }
    else if(type==SPE::TYPE_RELOCS)
    {
        QList<QString> listLabels;
        listLabels.append("");
        listLabels.append(getStructList(N_IMAGE_RELOCS::records,N_IMAGE_RELOCS::__data_size));
        listLabels.append("");
        listLabels.append("");

        QList<XPE::RELOCS_HEADER> listRH=pPE->getRelocsHeaders();

        int nCount=listRH.count();

        *ppModel=new QStandardItemModel(nCount,listLabels.count());

        setMaximum(nCount);

        setHeader(*ppModel,&listLabels);

        for(int i=0; i<nCount; i++)
        {
            QStandardItem *pItem=new QStandardItem;
            pItem->setData(i,Qt::DisplayRole);
            pItem->setData(listRH.at(i).nOffset,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET);
            (*ppModel)->setItem(i,0,                                            pItem);
            (*ppModel)->setItem(i,N_IMAGE_RELOCS::VirtualAddress+1,             new QStandardItem(XBinary::valueToHex(listRH.at(i).ibr.VirtualAddress)));
            (*ppModel)->setItem(i,N_IMAGE_RELOCS::SizeOfBlock+1,                new QStandardItem(XBinary::valueToHex(listRH.at(i).ibr.SizeOfBlock)));
            (*ppModel)->setItem(i,N_IMAGE_RELOCS::SizeOfBlock+2,                new QStandardItem(QString::number(listRH.at(i).nCount)));
            (*ppModel)->setItem(i,N_IMAGE_RELOCS::SizeOfBlock+3,                new QStandardItem(pPE->getMemoryRecordInfoByRelAddress(listRH.at(i).ibr.VirtualAddress))); // Comment

            incValue();
        }
    }
    else if(type==SPE::TYPE_RELOCS_POSITION)
    {
        QList<QString> listLabels;
        listLabels.append("");
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
            QStandardItem *pItem=new QStandardItem;
            pItem->setData(i,Qt::DisplayRole);
            (*ppModel)->setItem(i,0,                                            pItem);
            (*ppModel)->setItem(i,N_IMAGE_RELOCS_POSITION::TypeOffset+1,        new QStandardItem(XBinary::valueToHex(listRelocsPositions.at(i).nTypeOffset)));
            (*ppModel)->setItem(i,N_IMAGE_RELOCS_POSITION::TypeOffset+2,        new QStandardItem(mapTypes.value(listRelocsPositions.at(i).nType)));
            (*ppModel)->setItem(i,N_IMAGE_RELOCS_POSITION::TypeOffset+3,        new QStandardItem(XBinary::valueToHex((quint32)listRelocsPositions.at(i).nAddress)));

            incValue();
        }
    }
    else if(type==SPE::TYPE_IMPORT)
    {
        QList<QString> listLabels;
        listLabels.append("");
        listLabels.append(getStructList(N_IMAGE_IMPORT::records,N_IMAGE_IMPORT::__data_size));
        listLabels.append(QString("Hash"));
        listLabels.append("");

        XBinary::_MEMORY_MAP memoryMap=pPE->getMemoryMap();

        QList<quint32> listHashes=pPE->getImportPositionHashes(&memoryMap);

        QList<XPE::IMAGE_IMPORT_DESCRIPTOR_EX> listID=pPE->getImportDescriptorsEx(&memoryMap);

        int nCount=listID.count();

        *ppModel=new QStandardItemModel(nCount,listLabels.count());

        setMaximum(nCount);

        setHeader(*ppModel,&listLabels);

        for(int i=0; i<nCount; i++)
        {
            QStandardItem *pItem=new QStandardItem;
            pItem->setData(i,Qt::DisplayRole);
            (*ppModel)->setItem(i,0,                                                pItem);
            (*ppModel)->setItem(i,N_IMAGE_IMPORT::OriginalFirstThunk+1,             new QStandardItem(XBinary::valueToHex(listID.at(i).OriginalFirstThunk)));
            (*ppModel)->setItem(i,N_IMAGE_IMPORT::TimeDateStamp+1,                  new QStandardItem(XBinary::valueToHex(listID.at(i).TimeDateStamp)));
            (*ppModel)->setItem(i,N_IMAGE_IMPORT::ForwarderChain+1,                 new QStandardItem(XBinary::valueToHex(listID.at(i).ForwarderChain)));
            (*ppModel)->setItem(i,N_IMAGE_IMPORT::Name+1,                           new QStandardItem(XBinary::valueToHex(listID.at(i).Name)));
            (*ppModel)->setItem(i,N_IMAGE_IMPORT::FirstThunk+1,                     new QStandardItem(XBinary::valueToHex(listID.at(i).FirstThunk)));
            (*ppModel)->setItem(i,N_IMAGE_IMPORT::FirstThunk+2,                     new QStandardItem(XBinary::valueToHex(listHashes.at(i))));
            (*ppModel)->setItem(i,N_IMAGE_IMPORT::FirstThunk+3,                     new QStandardItem(listID.at(i).sLibrary));

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
            QStandardItem *pItem=new QStandardItem;
            pItem->setData(i,Qt::DisplayRole);

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
        listLabels.append("");
        listLabels.append(getStructList(N_IMAGE_EXCEPTIONS::records,N_IMAGE_EXCEPTIONS::__data_size));
        listLabels.append("");

        QList<XPE_DEF::S_IMAGE_RUNTIME_FUNCTION_ENTRY> listRFE=pPE->getExceptionsList();

        int nCount=listRFE.count();

        *ppModel=new QStandardItemModel(nCount,listLabels.count());

        setMaximum(nCount);

        setHeader(*ppModel,&listLabels);

        XBinary::_MEMORY_MAP memoryMap=pPE->getMemoryMap();

        for(int i=0; i<nCount; i++)
        {
            QStandardItem *pItem=new QStandardItem;
            pItem->setData(i,Qt::DisplayRole);
//                pItem->setData(Qt::UserRole,listRH.at(i).nOffset);

            pItem->setData(listRFE.at(i).BeginAddress,Qt::UserRole+FW_DEF::SECTION_DATA_ADDRESS);
            pItem->setData(listRFE.at(i).EndAddress-listRFE.at(i).BeginAddress,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE);
            pItem->setData(pPE->addressToOffset(&memoryMap,memoryMap.nBaseAddress+listRFE.at(i).BeginAddress),Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET);

            (*ppModel)->setItem(i,0,                                        pItem);
            (*ppModel)->setItem(i,N_IMAGE_EXCEPTIONS::BeginAddress+1,       new QStandardItem(XBinary::valueToHex(listRFE.at(i).BeginAddress)));
            (*ppModel)->setItem(i,N_IMAGE_EXCEPTIONS::EndAddress+1,         new QStandardItem(XBinary::valueToHex(listRFE.at(i).EndAddress)));
            (*ppModel)->setItem(i,N_IMAGE_EXCEPTIONS::UnwindInfoAddress+1,  new QStandardItem(XBinary::valueToHex(listRFE.at(i).UnwindInfoAddress)));
            (*ppModel)->setItem(i,N_IMAGE_EXCEPTIONS::UnwindInfoAddress+2,  new QStandardItem(pPE->getMemoryRecordInfoByRelAddress(listRFE.at(i).BeginAddress))); // Comment

            incValue();
        }
    }
    else if(type==SPE::TYPE_DELAYIMPORT)
    {
        QList<QString> listLabels;
        listLabels.append("");
        listLabels.append(getStructList(N_IMAGE_DELAYIMPORT::records,N_IMAGE_DELAYIMPORT::__data_size));
        listLabels.append("");

        QList<XPE_DEF::S_IMAGE_DELAYLOAD_DESCRIPTOR> listDelayImport=pPE->getDelayImportsList();

        int nCount=listDelayImport.count();

        *ppModel=new QStandardItemModel(nCount,listLabels.count());

        setMaximum(nCount);

        setHeader(*ppModel,&listLabels);

        XBinary::_MEMORY_MAP memoryMap=pPE->getMemoryMap();

        for(int i=0; i<nCount; i++)
        {
            QString sLibraryName=pPE->read_ansiString(pPE->relAddressToOffset(&memoryMap,listDelayImport.at(i).DllNameRVA));

            QStandardItem *pItem=new QStandardItem;
            pItem->setData(i,Qt::DisplayRole);

            (*ppModel)->setItem(i,0,                                                    pItem);
            (*ppModel)->setItem(i,N_IMAGE_DELAYIMPORT::AllAttributes+1,                 new QStandardItem(XBinary::valueToHex(listDelayImport.at(i).AllAttributes)));
            (*ppModel)->setItem(i,N_IMAGE_DELAYIMPORT::DllNameRVA+1,                    new QStandardItem(XBinary::valueToHex(listDelayImport.at(i).DllNameRVA)));
            (*ppModel)->setItem(i,N_IMAGE_DELAYIMPORT::ModuleHandleRVA+1,               new QStandardItem(XBinary::valueToHex(listDelayImport.at(i).ModuleHandleRVA)));
            (*ppModel)->setItem(i,N_IMAGE_DELAYIMPORT::ImportAddressTableRVA+1,         new QStandardItem(XBinary::valueToHex(listDelayImport.at(i).ImportAddressTableRVA)));
            (*ppModel)->setItem(i,N_IMAGE_DELAYIMPORT::ImportNameTableRVA+1,            new QStandardItem(XBinary::valueToHex(listDelayImport.at(i).ImportNameTableRVA)));
            (*ppModel)->setItem(i,N_IMAGE_DELAYIMPORT::BoundImportAddressTableRVA+1,    new QStandardItem(XBinary::valueToHex(listDelayImport.at(i).BoundImportAddressTableRVA)));
            (*ppModel)->setItem(i,N_IMAGE_DELAYIMPORT::UnloadInformationTableRVA+1,     new QStandardItem(XBinary::valueToHex(listDelayImport.at(i).UnloadInformationTableRVA)));
            (*ppModel)->setItem(i,N_IMAGE_DELAYIMPORT::TimeDateStamp+1,                 new QStandardItem(XBinary::valueToHex(listDelayImport.at(i).TimeDateStamp)));
            (*ppModel)->setItem(i,N_IMAGE_DELAYIMPORT::TimeDateStamp+2,                 new QStandardItem(sLibraryName));

            incValue();
        }
    }
    else if(type==SPE::TYPE_EXPORT_FUNCTION)
    {
        QList<QString> listLabels;
        listLabels.append(getStructList(N_IMAGE_EXPORT_FUNCTION::records,N_IMAGE_EXPORT_FUNCTION::__data_size));
        listLabels.append("");

        XPE::EXPORT_HEADER eh=pPE->getExport();

        int nCount=eh.listPositions.count();

        *ppModel=new QStandardItemModel(nCount,listLabels.count());

        setMaximum(nCount);

        setHeader(*ppModel,&listLabels);

        for(int i=0; i<nCount; i++)
        {
            (*ppModel)->setItem(i,N_IMAGE_EXPORT_FUNCTION::Ordinal,                 new QStandardItem(XBinary::valueToHex(eh.listPositions.at(i).nOrdinal)));
            (*ppModel)->setItem(i,N_IMAGE_EXPORT_FUNCTION::RVA,                     new QStandardItem(XBinary::valueToHex(eh.listPositions.at(i).nRVA)));
            (*ppModel)->setItem(i,N_IMAGE_EXPORT_FUNCTION::Name,                    new QStandardItem(XBinary::valueToHex(eh.listPositions.at(i).nNameRVA)));
            (*ppModel)->setItem(i,N_IMAGE_EXPORT_FUNCTION::Name+1,                  new QStandardItem(eh.listPositions.at(i).sFunctionName));

            incValue();
        }
    }
    else if(type==SPE::TYPE_DEBUG)
    {
        QList<QString> listLabels;
        listLabels.append("");
        listLabels.append(getStructList(N_IMAGE_DEBUG::records,N_IMAGE_DEBUG::__data_size));

        QList<XPE_DEF::S_IMAGE_DEBUG_DIRECTORY> listDebug=pPE->getDebugList();

        int nCount=listDebug.count();

        *ppModel=new QStandardItemModel(nCount,listLabels.count());

        setMaximum(nCount);

        setHeader(*ppModel,&listLabels);

        for(int i=0; i<nCount; i++)
        {
            QStandardItem *pItem=new QStandardItem;
            pItem->setData(i,Qt::DisplayRole);
            pItem->setData(listDebug.at(i).AddressOfRawData,Qt::UserRole+FW_DEF::SECTION_DATA_ADDRESS);
            pItem->setData(listDebug.at(i).SizeOfData,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE);
            pItem->setData(listDebug.at(i).PointerToRawData,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET);
            (*ppModel)->setItem(i,0,                                                pItem);
            (*ppModel)->setItem(i,N_IMAGE_DEBUG::Characteristics+1,                 new QStandardItem(XBinary::valueToHex(listDebug.at(i).Characteristics)));
            (*ppModel)->setItem(i,N_IMAGE_DEBUG::TimeDateStamp+1,                   new QStandardItem(XBinary::valueToHex(listDebug.at(i).TimeDateStamp)));
            (*ppModel)->setItem(i,N_IMAGE_DEBUG::MajorVersion+1,                    new QStandardItem(XBinary::valueToHex(listDebug.at(i).MajorVersion)));
            (*ppModel)->setItem(i,N_IMAGE_DEBUG::MinorVersion+1,                    new QStandardItem(XBinary::valueToHex(listDebug.at(i).MinorVersion)));
            (*ppModel)->setItem(i,N_IMAGE_DEBUG::Type+1,                            new QStandardItem(XBinary::valueToHex(listDebug.at(i).Type)));
            (*ppModel)->setItem(i,N_IMAGE_DEBUG::SizeOfData+1,                      new QStandardItem(XBinary::valueToHex(listDebug.at(i).SizeOfData)));
            (*ppModel)->setItem(i,N_IMAGE_DEBUG::AddressOfRawData+1,                new QStandardItem(XBinary::valueToHex(listDebug.at(i).AddressOfRawData)));
            (*ppModel)->setItem(i,N_IMAGE_DEBUG::PointerToRawData+1,                new QStandardItem(XBinary::valueToHex(listDebug.at(i).PointerToRawData)));

            incValue();
        }
    }
    else if(type==SPE::TYPE_DELAYIMPORT_FUNCTION)
    {
        QList<QString> listLabels;
        listLabels.append("");
        listLabels.append(getStructList(N_IMAGE_DELAYIMPORT_FUNCTION::records32,N_IMAGE_DELAYIMPORT_FUNCTION::__data_size));
        listLabels.append("Name");

        bool bIs64=pPE->is64();
        QList<XPE::DELAYIMPORT_POSITION> listDIP=pPE->getDelayImportPositions(nNumber);

        int nCount=listDIP.count();

        *ppModel=new QStandardItemModel(nCount,listLabels.count());

        setMaximum(nCount);

        setHeader(*ppModel,&listLabels);

        for(int i=0; i<nCount; i++)
        {
            QStandardItem *pItem=new QStandardItem;
            pItem->setData(i,Qt::DisplayRole);

            (*ppModel)->setItem(i,0,pItem);

            if(bIs64)
            {
                (*ppModel)->setItem(i,N_IMAGE_DELAYIMPORT_FUNCTION::AddressThunk+1,     new QStandardItem(XBinary::valueToHex((quint64)listDIP.at(i).nAddressThunkRVA)));
                (*ppModel)->setItem(i,N_IMAGE_DELAYIMPORT_FUNCTION::BoundThunk+1,       new QStandardItem(XBinary::valueToHex((quint64)listDIP.at(i).nBoundThunkRVA)));
            }
            else
            {
                (*ppModel)->setItem(i,N_IMAGE_DELAYIMPORT_FUNCTION::AddressThunk+1,     new QStandardItem(XBinary::valueToHex((quint32)listDIP.at(i).nAddressThunkRVA)));
                (*ppModel)->setItem(i,N_IMAGE_DELAYIMPORT_FUNCTION::BoundThunk+1,       new QStandardItem(XBinary::valueToHex((quint32)listDIP.at(i).nBoundThunkRVA)));
            }

            if(listDIP.at(i).nOrdinal)
            {
                QString sOrdinal;

                if(bIs64)
                {
                    sOrdinal=XBinary::valueToHex((quint64)listDIP.at(i).nOrdinal);
                }
                else
                {
                    sOrdinal=XBinary::valueToHex((quint32)listDIP.at(i).nOrdinal);
                }

                (*ppModel)->setItem(i,N_IMAGE_DELAYIMPORT_FUNCTION::Ordinal+1,   new QStandardItem(sOrdinal));
            }
            else
            {
                QString sThunk;

                if(bIs64)
                {
                    sThunk=XBinary::valueToHex((quint64)listDIP.at(i).nNameThunkValue);
                }
                else
                {
                    sThunk=XBinary::valueToHex((quint32)listDIP.at(i).nNameThunkValue);
                }

                (*ppModel)->setItem(i,N_IMAGE_DELAYIMPORT_FUNCTION::NameThunk+1,    new QStandardItem(sThunk));
                (*ppModel)->setItem(i,N_IMAGE_DELAYIMPORT_FUNCTION::Hint+1,         new QStandardItem(XBinary::valueToHex(listDIP.at(i).nHint)));
                (*ppModel)->setItem(i,N_IMAGE_DELAYIMPORT_FUNCTION::Hint+2,         new QStandardItem(listDIP.at(i).sName));
            }

            incValue();
        }
    }
    else if(type==SPE::TYPE_RESOURCE)
    {
        XPE::RESOURCE_HEADER rh=pPE->getResourceHeader();

        *ppModel=new QStandardItemModel;

        int nHeaderCount=rh.listPositions.count();

        if(nHeaderCount)
        {
            QStandardItem *pRoot=new QStandardItem();
            pRoot->setText(tr("Resources"));

            (*ppModel)->appendRow(pRoot);

            for(int i=0; i<nHeaderCount; i++)
            {
                XPE::RESOURCE_POSITION pos=rh.listPositions.at(i);
                QStandardItem *pPos=new QStandardItem;
                QString sPosText;

                if(pos.rin.bIsName)
                {
                    sPosText=QString("\"%1\"").arg(pos.rin.sName);
                }
                else
                {
                    QMap<quint64, QString> mapRT=XPE::getResourceTypes();
                    QString sType=mapRT.value(pos.rin.nID);

                    if(sType!="")
                    {
                        sPosText=QString("%1(%2)").arg(sType).arg(pos.rin.nID);
                    }
                    else
                    {
                        sPosText=QString("%1").arg(pos.rin.nID);
                    }
                }

                pPos->setText(sPosText);

                pRoot->appendRow(pPos);

                int nPosCount=pos.listPositions.count();

                for(int j=0; j<nPosCount; j++)
                {
                    XPE::RESOURCE_POSITION subpos=rh.listPositions.at(i).listPositions.at(j);
                    QStandardItem *pSubPos=new QStandardItem;
                    QString sSubPosText;

                    if(subpos.rin.bIsName)
                    {
                        sSubPosText=QString("\"%1\"").arg(subpos.rin.sName);
                    }
                    else
                    {
                        sSubPosText=QString("%1").arg(subpos.rin.nID);
                    }

                    pSubPos->setText(sSubPosText);

                    pPos->appendRow(pSubPos);

                    int nSubPosCount=subpos.listPositions.count();

                    for(int k=0; k<nSubPosCount; k++)
                    {
                        XPE::RESOURCE_POSITION record1=rh.listPositions.at(i);
                        XPE::RESOURCE_POSITION record2=rh.listPositions.at(i).listPositions.at(j);
                        XPE::RESOURCE_POSITION record3=rh.listPositions.at(i).listPositions.at(j).listPositions.at(k);
                        QStandardItem *pRecord=new QStandardItem;

                        QString sRecordText1;
                        QString sRecordText2;
                        QString sRecordText3;

                        if(record1.rin.bIsName)
                        {
                            sRecordText1=QString("\"%1\"").arg(record1.rin.sName);
                        }
                        else
                        {
                            sRecordText1=QString("%1").arg(record1.rin.nID);
                        }

                        if(record2.rin.bIsName)
                        {
                            sRecordText2=QString("\"%1\"").arg(record2.rin.sName);
                        }
                        else
                        {
                            sRecordText2=QString("%1").arg(record2.rin.nID);
                        }

                        if(record3.rin.bIsName)
                        {
                            sRecordText3=QString("\"%1\"").arg(record3.rin.sName);
                        }
                        else
                        {
                            sRecordText3=QString("%1").arg(record3.rin.nID);
                        }

                        pRecord->setText(sRecordText3);

                        pRecord->setData(sRecordText1,Qt::UserRole+FW_DEF::SECTION_DATA_VALUE1);
                        pRecord->setData(sRecordText2,Qt::UserRole+FW_DEF::SECTION_DATA_VALUE2);
                        pRecord->setData(sRecordText3,Qt::UserRole+FW_DEF::SECTION_DATA_VALUE3);
                        pRecord->setData(record3.data_entry.Size,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE);
                        pRecord->setData(record3.data_entry.OffsetToData,Qt::UserRole+FW_DEF::SECTION_DATA_ADDRESS);

                        if(pPE->isImage())
                        {
                            pRecord->setData(record3.data_entry.OffsetToData,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET);
                        }
                        else
                        {
                            pRecord->setData(record3.nDataOffset,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET);
                        }

                        pSubPos->appendRow(pRecord);
                    }
                }

                incValue();
            }
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
    else if(type==SPE::TYPE_RELOCS)
    {
        pTableView->setColumnWidth(0,nSymbolWidth*4);
        pTableView->setColumnWidth(1,nSymbolWidth*8);
        pTableView->setColumnWidth(2,nSymbolWidth*8);
        pTableView->setColumnWidth(3,nSymbolWidth*8);
        pTableView->setColumnWidth(4,nSymbolWidth*30);
    }
    else if(type==SPE::TYPE_RELOCS_POSITION)
    {
        pTableView->setColumnWidth(0,nSymbolWidth*4);
        pTableView->setColumnWidth(1,nSymbolWidth*8);
        pTableView->setColumnWidth(2,nSymbolWidth*12);
        pTableView->setColumnWidth(3,nSymbolWidth*12);
    }
    else if(type==SPE::TYPE_IMPORT)
    {
        pTableView->setColumnWidth(0,nSymbolWidth*4);
        pTableView->setColumnWidth(1,nSymbolWidth*8);
        pTableView->setColumnWidth(2,nSymbolWidth*8);
        pTableView->setColumnWidth(3,nSymbolWidth*8);
        pTableView->setColumnWidth(4,nSymbolWidth*8);
        pTableView->setColumnWidth(5,nSymbolWidth*8);
        pTableView->setColumnWidth(6,nSymbolWidth*8);
        pTableView->setColumnWidth(7,nSymbolWidth*16);
    }
    else if(type==SPE::TYPE_IMPORT_FUNCTION)
    {
        pTableView->setColumnWidth(0,nSymbolWidth*4);
        pTableView->setColumnWidth(1,nSymbolWidth*12);
        pTableView->setColumnWidth(2,nSymbolWidth*12);
        pTableView->setColumnWidth(3,nSymbolWidth*6);
        pTableView->setColumnWidth(4,nSymbolWidth*22);
    }
    else if(type==SPE::TYPE_EXCEPTION)
    {
        pTableView->setColumnWidth(0,nSymbolWidth*4);
        pTableView->setColumnWidth(1,nSymbolWidth*8);
        pTableView->setColumnWidth(2,nSymbolWidth*8);
        pTableView->setColumnWidth(3,nSymbolWidth*8);
        pTableView->setColumnWidth(4,nSymbolWidth*30);
    }
    else if(type==SPE::TYPE_DELAYIMPORT)
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
        pTableView->setColumnWidth(9,nSymbolWidth*15);
    }
    else if(type==SPE::TYPE_EXPORT_FUNCTION)
    {
        pTableView->setColumnWidth(0,nSymbolWidth*4);
        pTableView->setColumnWidth(1,nSymbolWidth*8);
        pTableView->setColumnWidth(2,nSymbolWidth*8);
        pTableView->setColumnWidth(3,nSymbolWidth*30);
    }
    else if(type==SPE::TYPE_DEBUG)
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
    }
    else if(type==SPE::TYPE_DELAYIMPORT_FUNCTION)
    {
        pTableView->setColumnWidth(0,nSymbolWidth*4);
        pTableView->setColumnWidth(1,nSymbolWidth*12);
        pTableView->setColumnWidth(2,nSymbolWidth*12);
        pTableView->setColumnWidth(3,nSymbolWidth*12);
        pTableView->setColumnWidth(4,nSymbolWidth*6);
        pTableView->setColumnWidth(5,nSymbolWidth*22);
    }
}

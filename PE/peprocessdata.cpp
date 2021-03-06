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
#include "peprocessdata.h"

PEProcessData::PEProcessData(int nType, QStandardItemModel **ppModel, XPE *pPE, qint32 nNumber, qint64 nOffset, qint64 nSize, QVariant varInfo) : ProcessData()
{
    this->g_nType=nType;
    this->g_ppModel=ppModel;
    this->g_pPE=pPE;
    this->g_nNumber=nNumber;
    this->g_nOffset=nOffset;
    this->g_nSize=nSize;
    this->g_varInfo=varInfo;
}

void PEProcessData::_process()
{
    if(g_nType==SPE::TYPE_SECTIONS)
    {
        QList<QString> listLabels;
        listLabels.append("");
        listLabels.append(getStructList(N_IMAGE_SECTION_HEADER::records,N_IMAGE_SECTION_HEADER::__data_size));

        QList<XPE_DEF::IMAGE_SECTION_HEADER> listSections=g_pPE->getSectionHeaders();

        int nNumberOfSections=listSections.count();

        *g_ppModel=new QStandardItemModel(nNumberOfSections,listLabels.count());

        setMaximum(nNumberOfSections);

        setHeader(*g_ppModel,&listLabels);

        for(int i=0; (i<nNumberOfSections)&&(isRun()); i++)
        {
            QStandardItem *pItemNumber=new QStandardItem;
            pItemNumber->setData(i,Qt::DisplayRole);

            pItemNumber->setData(listSections.at(i).VirtualAddress,Qt::UserRole+FW_DEF::SECTION_DATA_ADDRESS);

            if(g_pPE->isImage())
            {
                pItemNumber->setData(listSections.at(i).Misc.VirtualSize,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE);
                pItemNumber->setData(listSections.at(i).VirtualAddress,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET);
            }
            else
            {
                pItemNumber->setData(listSections.at(i).SizeOfRawData,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE);
                pItemNumber->setData(listSections.at(i).PointerToRawData,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET);
            }

            (*g_ppModel)->setItem(i,0,pItemNumber);

            QStandardItem *pItemName=new QStandardItem();
            QString sName=QString((char *)listSections.at(i).Name);
            sName.resize(qMin(sName.length(),XPE_DEF::S_IMAGE_SIZEOF_SHORT_NAME));
            pItemName->setText(sName);
            (*g_ppModel)->setItem(i,N_IMAGE_SECTION_HEADER::Name+1,pItemName);

            (*g_ppModel)->setItem(i,N_IMAGE_SECTION_HEADER::VirtualSize+1,          new QStandardItem(XBinary::valueToHex(listSections.at(i).Misc.VirtualSize)));
            (*g_ppModel)->setItem(i,N_IMAGE_SECTION_HEADER::VirtualAddress+1,       new QStandardItem(XBinary::valueToHex(listSections.at(i).VirtualAddress)));
            (*g_ppModel)->setItem(i,N_IMAGE_SECTION_HEADER::SizeOfRawData+1,        new QStandardItem(XBinary::valueToHex(listSections.at(i).SizeOfRawData)));
            (*g_ppModel)->setItem(i,N_IMAGE_SECTION_HEADER::PointerToRawData+1,     new QStandardItem(XBinary::valueToHex(listSections.at(i).PointerToRawData)));
            (*g_ppModel)->setItem(i,N_IMAGE_SECTION_HEADER::PointerToRelocations+1, new QStandardItem(XBinary::valueToHex(listSections.at(i).PointerToRelocations)));
            (*g_ppModel)->setItem(i,N_IMAGE_SECTION_HEADER::PointerToLinenumbers+1, new QStandardItem(XBinary::valueToHex(listSections.at(i).PointerToLinenumbers)));
            (*g_ppModel)->setItem(i,N_IMAGE_SECTION_HEADER::NumberOfRelocations+1,  new QStandardItem(XBinary::valueToHex(listSections.at(i).NumberOfRelocations)));
            (*g_ppModel)->setItem(i,N_IMAGE_SECTION_HEADER::NumberOfLinenumbers+1,  new QStandardItem(XBinary::valueToHex(listSections.at(i).NumberOfLinenumbers)));
            (*g_ppModel)->setItem(i,N_IMAGE_SECTION_HEADER::Characteristics+1,      new QStandardItem(XBinary::valueToHex(listSections.at(i).Characteristics)));

            incValue();
        }
    }
    else if(g_nType==SPE::TYPE_RELOCS)
    {
        QList<QString> listLabels;
        listLabels.append("");
        listLabels.append(getStructList(N_IMAGE_RELOCS::records,N_IMAGE_RELOCS::__data_size));
        listLabels.append("");
        listLabels.append("");

        QList<XPE::RELOCS_HEADER> listRelocsHeaders=g_pPE->getRelocsHeaders();

        int nNumberOfRecords=listRelocsHeaders.count();

        *g_ppModel=new QStandardItemModel(nNumberOfRecords,listLabels.count());

        setMaximum(nNumberOfRecords);

        setHeader(*g_ppModel,&listLabels);

        for(int i=0; (i<nNumberOfRecords)&&(isRun()); i++)
        {
            QStandardItem *pItem=new QStandardItem;
            pItem->setData(i,Qt::DisplayRole);
            pItem->setData(listRelocsHeaders.at(i).nOffset,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET);
            pItem->setData(listRelocsHeaders.at(i).nOffset,Qt::UserRole+FW_DEF::SECTION_DATA_HEADEROFFSET);
            (*g_ppModel)->setItem(i,0,                                            pItem);
            (*g_ppModel)->setItem(i,N_IMAGE_RELOCS::VirtualAddress+1,             new QStandardItem(XBinary::valueToHex(listRelocsHeaders.at(i).baseRelocation.VirtualAddress)));
            (*g_ppModel)->setItem(i,N_IMAGE_RELOCS::SizeOfBlock+1,                new QStandardItem(XBinary::valueToHex(listRelocsHeaders.at(i).baseRelocation.SizeOfBlock)));
            (*g_ppModel)->setItem(i,N_IMAGE_RELOCS::SizeOfBlock+2,                new QStandardItem(QString::number(listRelocsHeaders.at(i).nCount)));
            (*g_ppModel)->setItem(i,N_IMAGE_RELOCS::SizeOfBlock+3,                new QStandardItem(g_pPE->getMemoryRecordInfoByRelAddress(listRelocsHeaders.at(i).baseRelocation.VirtualAddress))); // Comment

            incValue();
        }
    }
    else if(g_nType==SPE::TYPE_RELOCS_POSITION)
    {
        QList<QString> listLabels;
        listLabels.append("");
        listLabels.append(getStructList(N_IMAGE_RELOCS_POSITION::records,N_IMAGE_RELOCS_POSITION::__data_size));
        listLabels.append("Type");
        listLabels.append("Address");

        QList<XPE::RELOCS_POSITION> listRelocsPositions=g_pPE->getRelocsPositions(g_nOffset);

        int nNumberOfRelocs=listRelocsPositions.count();

        *g_ppModel=new QStandardItemModel(nNumberOfRelocs,listLabels.count());

        setMaximum(nNumberOfRelocs);

        setHeader(*g_ppModel,&listLabels);

        QMap<quint64,QString> mapTypes=g_pPE->getImageRelBasedS();

        for(int i=0; (i<nNumberOfRelocs)&&(isRun()); i++)
        {
            QStandardItem *pItem=new QStandardItem;
            pItem->setData(i,Qt::DisplayRole);
            (*g_ppModel)->setItem(i,0,                                            pItem);
            (*g_ppModel)->setItem(i,N_IMAGE_RELOCS_POSITION::TypeOffset+1,        new QStandardItem(XBinary::valueToHex(listRelocsPositions.at(i).nTypeOffset)));
            (*g_ppModel)->setItem(i,N_IMAGE_RELOCS_POSITION::TypeOffset+2,        new QStandardItem(mapTypes.value(listRelocsPositions.at(i).nType)));
            (*g_ppModel)->setItem(i,N_IMAGE_RELOCS_POSITION::TypeOffset+3,        new QStandardItem(XBinary::valueToHex((quint32)listRelocsPositions.at(i).nAddress)));

            incValue();
        }
    }
    else if(g_nType==SPE::TYPE_IMPORT)
    {
        QList<QString> listLabels;
        listLabels.append("");
        listLabels.append(getStructList(N_IMAGE_IMPORT::records,N_IMAGE_IMPORT::__data_size));
        listLabels.append(QString("Hash"));
        listLabels.append("");

        XBinary::_MEMORY_MAP memoryMap=g_pPE->getMemoryMap();

        QList<XPE::IMPORT_HEADER> listImport=g_pPE->getImports(&memoryMap);

        QList<quint32> listImportPositionHashes=g_pPE->getImportPositionHashes(&listImport);

        QList<XPE::IMAGE_IMPORT_DESCRIPTOR_EX> listID=g_pPE->getImportDescriptorsEx(&memoryMap);

        int nNumberOfIDs=listID.count();

        *g_ppModel=new QStandardItemModel(nNumberOfIDs,listLabels.count());

        setMaximum(nNumberOfIDs);

        setHeader(*g_ppModel,&listLabels);

        for(int i=0; (i<nNumberOfIDs)&&(isRun()); i++)
        {
            QStandardItem *pItem=new QStandardItem;
            pItem->setData(i,Qt::DisplayRole);
            (*g_ppModel)->setItem(i,0,                                                pItem);
            (*g_ppModel)->setItem(i,N_IMAGE_IMPORT::OriginalFirstThunk+1,             new QStandardItem(XBinary::valueToHex(listID.at(i).OriginalFirstThunk)));
            (*g_ppModel)->setItem(i,N_IMAGE_IMPORT::TimeDateStamp+1,                  new QStandardItem(XBinary::valueToHex(listID.at(i).TimeDateStamp)));
            (*g_ppModel)->setItem(i,N_IMAGE_IMPORT::ForwarderChain+1,                 new QStandardItem(XBinary::valueToHex(listID.at(i).ForwarderChain)));
            (*g_ppModel)->setItem(i,N_IMAGE_IMPORT::Name+1,                           new QStandardItem(XBinary::valueToHex(listID.at(i).Name)));
            (*g_ppModel)->setItem(i,N_IMAGE_IMPORT::FirstThunk+1,                     new QStandardItem(XBinary::valueToHex(listID.at(i).FirstThunk)));
            (*g_ppModel)->setItem(i,N_IMAGE_IMPORT::FirstThunk+2,                     new QStandardItem(XBinary::valueToHex(listImportPositionHashes.at(i))));
            (*g_ppModel)->setItem(i,N_IMAGE_IMPORT::FirstThunk+3,                     new QStandardItem(listID.at(i).sLibrary));

            incValue();
        }
    }
    else if(g_nType==SPE::TYPE_IMPORT_FUNCTION)
    {
        QList<QString> listLabels;
        listLabels.append("");
        listLabels.append(getStructList(N_IMAGE_IMPORT_FUNCTION::records32,N_IMAGE_IMPORT_FUNCTION::__data_size));
        listLabels.append("Name");

        bool bIs64=g_pPE->is64();
        QList<XPE::IMPORT_POSITION> listImportPositions=g_pPE->getImportPositions(g_nNumber);

        int nNumberOfIPs=listImportPositions.count();

        *g_ppModel=new QStandardItemModel(nNumberOfIPs,listLabels.count());

        setMaximum(nNumberOfIPs);

        setHeader(*g_ppModel,&listLabels);

        for(int i=0; (i<nNumberOfIPs)&&(isRun()); i++)
        {
            QStandardItem *pItem=new QStandardItem;
            pItem->setData(i,Qt::DisplayRole);

            (*g_ppModel)->setItem(i,0,pItem);

            if(listImportPositions.at(i).nOrdinal)
            {
                QString sOrdinal;

                if(bIs64)
                {
                    sOrdinal=XBinary::valueToHex((quint64)listImportPositions.at(i).nOrdinal);
                }
                else
                {
                    sOrdinal=XBinary::valueToHex((quint32)listImportPositions.at(i).nOrdinal);
                }

                (*g_ppModel)->setItem(i,N_IMAGE_IMPORT_FUNCTION::Ordinal+1,   new QStandardItem(sOrdinal));
            }
            else
            {
                QString sThunk;

                if(bIs64)
                {
                    sThunk=XBinary::valueToHex((quint64)listImportPositions.at(i).nThunkValue);
                }
                else
                {
                    sThunk=XBinary::valueToHex((quint32)listImportPositions.at(i).nThunkValue);
                }

                (*g_ppModel)->setItem(i,N_IMAGE_IMPORT_FUNCTION::Thunk+1,     new QStandardItem(sThunk));
                (*g_ppModel)->setItem(i,N_IMAGE_IMPORT_FUNCTION::Hint+1,      new QStandardItem(XBinary::valueToHex(listImportPositions.at(i).nHint)));
                (*g_ppModel)->setItem(i,N_IMAGE_IMPORT_FUNCTION::Hint+2,      new QStandardItem(listImportPositions.at(i).sName));
            }

            incValue();
        }
    }
    else if(g_nType==SPE::TYPE_EXCEPTION)
    {
        QList<QString> listLabels;
        listLabels.append("");
        listLabels.append(getStructList(N_IMAGE_EXCEPTIONS::records,N_IMAGE_EXCEPTIONS::__data_size));
        listLabels.append("");

        QList<XPE_DEF::S_IMAGE_RUNTIME_FUNCTION_ENTRY> listRFE=g_pPE->getExceptionsList();

        int nNumberOfRFEs=listRFE.count();

        *g_ppModel=new QStandardItemModel(nNumberOfRFEs,listLabels.count());

        setMaximum(nNumberOfRFEs);

        setHeader(*g_ppModel,&listLabels);

        XBinary::_MEMORY_MAP memoryMap=g_pPE->getMemoryMap();

        for(int i=0; (i<nNumberOfRFEs)&&(isRun()); i++)
        {
            QStandardItem *pItem=new QStandardItem;
            pItem->setData(i,Qt::DisplayRole);

            pItem->setData(listRFE.at(i).BeginAddress,Qt::UserRole+FW_DEF::SECTION_DATA_ADDRESS);
            pItem->setData(listRFE.at(i).EndAddress-listRFE.at(i).BeginAddress,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE);
            pItem->setData(g_pPE->addressToOffset(&memoryMap,memoryMap.nModuleAddress+listRFE.at(i).BeginAddress),Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET);

            (*g_ppModel)->setItem(i,0,                                        pItem);
            (*g_ppModel)->setItem(i,N_IMAGE_EXCEPTIONS::BeginAddress+1,       new QStandardItem(XBinary::valueToHex(listRFE.at(i).BeginAddress)));
            (*g_ppModel)->setItem(i,N_IMAGE_EXCEPTIONS::EndAddress+1,         new QStandardItem(XBinary::valueToHex(listRFE.at(i).EndAddress)));
            (*g_ppModel)->setItem(i,N_IMAGE_EXCEPTIONS::UnwindInfoAddress+1,  new QStandardItem(XBinary::valueToHex(listRFE.at(i).UnwindInfoAddress)));
            (*g_ppModel)->setItem(i,N_IMAGE_EXCEPTIONS::UnwindInfoAddress+2,  new QStandardItem(g_pPE->getMemoryRecordInfoByRelAddress(listRFE.at(i).BeginAddress))); // Comment

            incValue();
        }
    }
    else if(g_nType==SPE::TYPE_DELAYIMPORT)
    {
        QList<QString> listLabels;
        listLabels.append("");
        listLabels.append(getStructList(N_IMAGE_DELAYIMPORT::records,N_IMAGE_DELAYIMPORT::__data_size));
        listLabels.append("");

        QList<XPE_DEF::S_IMAGE_DELAYLOAD_DESCRIPTOR> listDelayImport=g_pPE->getDelayImportsList();

        int nNumberOfDelayImports=listDelayImport.count();

        *g_ppModel=new QStandardItemModel(nNumberOfDelayImports,listLabels.count());

        setMaximum(nNumberOfDelayImports);

        setHeader(*g_ppModel,&listLabels);

        XBinary::_MEMORY_MAP memoryMap=g_pPE->getMemoryMap();

        for(int i=0; (i<nNumberOfDelayImports)&&(isRun()); i++)
        {
            QString sLibraryName=g_pPE->read_ansiString(g_pPE->relAddressToOffset(&memoryMap,listDelayImport.at(i).DllNameRVA));

            QStandardItem *pItem=new QStandardItem;
            pItem->setData(i,Qt::DisplayRole);

            (*g_ppModel)->setItem(i,0,                                                    pItem);
            (*g_ppModel)->setItem(i,N_IMAGE_DELAYIMPORT::AllAttributes+1,                 new QStandardItem(XBinary::valueToHex(listDelayImport.at(i).AllAttributes)));
            (*g_ppModel)->setItem(i,N_IMAGE_DELAYIMPORT::DllNameRVA+1,                    new QStandardItem(XBinary::valueToHex(listDelayImport.at(i).DllNameRVA)));
            (*g_ppModel)->setItem(i,N_IMAGE_DELAYIMPORT::ModuleHandleRVA+1,               new QStandardItem(XBinary::valueToHex(listDelayImport.at(i).ModuleHandleRVA)));
            (*g_ppModel)->setItem(i,N_IMAGE_DELAYIMPORT::ImportAddressTableRVA+1,         new QStandardItem(XBinary::valueToHex(listDelayImport.at(i).ImportAddressTableRVA)));
            (*g_ppModel)->setItem(i,N_IMAGE_DELAYIMPORT::ImportNameTableRVA+1,            new QStandardItem(XBinary::valueToHex(listDelayImport.at(i).ImportNameTableRVA)));
            (*g_ppModel)->setItem(i,N_IMAGE_DELAYIMPORT::BoundImportAddressTableRVA+1,    new QStandardItem(XBinary::valueToHex(listDelayImport.at(i).BoundImportAddressTableRVA)));
            (*g_ppModel)->setItem(i,N_IMAGE_DELAYIMPORT::UnloadInformationTableRVA+1,     new QStandardItem(XBinary::valueToHex(listDelayImport.at(i).UnloadInformationTableRVA)));
            (*g_ppModel)->setItem(i,N_IMAGE_DELAYIMPORT::TimeDateStamp+1,                 new QStandardItem(XBinary::valueToHex(listDelayImport.at(i).TimeDateStamp)));
            (*g_ppModel)->setItem(i,N_IMAGE_DELAYIMPORT::TimeDateStamp+2,                 new QStandardItem(sLibraryName));

            incValue();
        }
    }
    else if(g_nType==SPE::TYPE_EXPORT_FUNCTION)
    {
        QList<QString> listLabels;
        // No need number
        listLabels.append(getStructList(N_IMAGE_EXPORT_FUNCTION::records,N_IMAGE_EXPORT_FUNCTION::__data_size));
        listLabels.append("");

        bool bFilter=g_varInfo.toBool();

        XPE::EXPORT_HEADER eh=g_pPE->getExport(bFilter);

        int nNumberOfPositions=eh.listPositions.count();

        *g_ppModel=new QStandardItemModel(nNumberOfPositions,listLabels.count());

        setMaximum(nNumberOfPositions);

        setHeader(*g_ppModel,&listLabels);

        XBinary::_MEMORY_MAP memoryMap=g_pPE->getMemoryMap();

        for(int i=0; (i<nNumberOfPositions)&&(isRun()); i++)
        {
            QStandardItem *pItem=new QStandardItem;
            pItem->setText(XBinary::valueToHex(eh.listPositions.at(i).nOrdinal));

            pItem->setData(memoryMap.nModuleAddress+eh.listPositions.at(i).nRVA,Qt::UserRole+FW_DEF::SECTION_DATA_ADDRESS);
            pItem->setData(1,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE);
            pItem->setData(g_pPE->addressToOffset(&memoryMap,memoryMap.nModuleAddress+eh.listPositions.at(i).nRVA),Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET);

            (*g_ppModel)->setItem(i,N_IMAGE_EXPORT_FUNCTION::Ordinal,                 pItem);
            (*g_ppModel)->setItem(i,N_IMAGE_EXPORT_FUNCTION::RVA,                     new QStandardItem(XBinary::valueToHex(eh.listPositions.at(i).nRVA)));
            (*g_ppModel)->setItem(i,N_IMAGE_EXPORT_FUNCTION::Name,                    new QStandardItem(XBinary::valueToHex(eh.listPositions.at(i).nNameRVA)));
            (*g_ppModel)->setItem(i,N_IMAGE_EXPORT_FUNCTION::Name+1,                  new QStandardItem(eh.listPositions.at(i).sFunctionName));

            incValue();
        }
    }
    else if(g_nType==SPE::TYPE_BOUNDIMPORT)
    {
        QList<QString> listLabels;
        listLabels.append("");
        listLabels.append(getStructList(N_IMAGE_BOUNDIMPORT::records,N_IMAGE_BOUNDIMPORT::__data_size));
        listLabels.append("");
        listLabels.append("");

        QList<XPE::BOUND_IMPORT_POSITION> listBoundImportPositions=g_pPE->getBoundImportPositions();

        int nNumberOfPositions=listBoundImportPositions.count();

        *g_ppModel=new QStandardItemModel(nNumberOfPositions,listLabels.count());

        setMaximum(nNumberOfPositions);

        setHeader(*g_ppModel,&listLabels);

        for(int i=0; (i<nNumberOfPositions)&&(isRun()); i++)
        {
            QStandardItem *pItem=new QStandardItem;
            pItem->setData(i,Qt::DisplayRole);

            (*g_ppModel)->setItem(i,0,                                                    pItem);
            (*g_ppModel)->setItem(i,N_IMAGE_BOUNDIMPORT::TimeDateStamp+1,                 new QStandardItem(XBinary::valueToHex(listBoundImportPositions.at(i).descriptor.TimeDateStamp)));
            (*g_ppModel)->setItem(i,N_IMAGE_BOUNDIMPORT::OffsetModuleName+1,              new QStandardItem(XBinary::valueToHex(listBoundImportPositions.at(i).descriptor.OffsetModuleName)));
            (*g_ppModel)->setItem(i,N_IMAGE_BOUNDIMPORT::NumberOfModuleForwarderRefs+1,   new QStandardItem(XBinary::valueToHex(listBoundImportPositions.at(i).descriptor.NumberOfModuleForwarderRefs)));
            (*g_ppModel)->setItem(i,N_IMAGE_BOUNDIMPORT::NumberOfModuleForwarderRefs+2,   new QStandardItem(XBinary::valueToTimeString(listBoundImportPositions.at(i).descriptor.TimeDateStamp,XBinary::DT_TYPE_POSIX)));
            (*g_ppModel)->setItem(i,N_IMAGE_BOUNDIMPORT::NumberOfModuleForwarderRefs+3,   new QStandardItem(listBoundImportPositions.at(i).sName));

            incValue();
        }
    }
    else if(g_nType==SPE::TYPE_DEBUG)
    {
        QList<QString> listLabels;
        listLabels.append("");
        listLabels.append(getStructList(N_IMAGE_DEBUG::records,N_IMAGE_DEBUG::__data_size));

        QList<XPE_DEF::S_IMAGE_DEBUG_DIRECTORY> listDebug=g_pPE->getDebugList();

        int nNumberOfDebugs=listDebug.count();

        *g_ppModel=new QStandardItemModel(nNumberOfDebugs,listLabels.count());

        setMaximum(nNumberOfDebugs);

        setHeader(*g_ppModel,&listLabels);

        for(int i=0; (i<nNumberOfDebugs)&&(isRun()); i++)
        {
            QStandardItem *pItem=new QStandardItem;
            pItem->setData(i,Qt::DisplayRole);
            pItem->setData(listDebug.at(i).AddressOfRawData,Qt::UserRole+FW_DEF::SECTION_DATA_ADDRESS);
            pItem->setData(listDebug.at(i).SizeOfData,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE);
            pItem->setData(listDebug.at(i).PointerToRawData,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET);
            (*g_ppModel)->setItem(i,0,                                                pItem);
            (*g_ppModel)->setItem(i,N_IMAGE_DEBUG::Characteristics+1,                 new QStandardItem(XBinary::valueToHex(listDebug.at(i).Characteristics)));
            (*g_ppModel)->setItem(i,N_IMAGE_DEBUG::TimeDateStamp+1,                   new QStandardItem(XBinary::valueToHex(listDebug.at(i).TimeDateStamp)));
            (*g_ppModel)->setItem(i,N_IMAGE_DEBUG::MajorVersion+1,                    new QStandardItem(XBinary::valueToHex(listDebug.at(i).MajorVersion)));
            (*g_ppModel)->setItem(i,N_IMAGE_DEBUG::MinorVersion+1,                    new QStandardItem(XBinary::valueToHex(listDebug.at(i).MinorVersion)));
            (*g_ppModel)->setItem(i,N_IMAGE_DEBUG::Type+1,                            new QStandardItem(XBinary::valueToHex(listDebug.at(i).Type)));
            (*g_ppModel)->setItem(i,N_IMAGE_DEBUG::SizeOfData+1,                      new QStandardItem(XBinary::valueToHex(listDebug.at(i).SizeOfData)));
            (*g_ppModel)->setItem(i,N_IMAGE_DEBUG::AddressOfRawData+1,                new QStandardItem(XBinary::valueToHex(listDebug.at(i).AddressOfRawData)));
            (*g_ppModel)->setItem(i,N_IMAGE_DEBUG::PointerToRawData+1,                new QStandardItem(XBinary::valueToHex(listDebug.at(i).PointerToRawData)));

            incValue();
        }
    }
    else if(g_nType==SPE::TYPE_DELAYIMPORT_FUNCTION)
    {
        QList<QString> listLabels;
        listLabels.append("");
        listLabels.append(getStructList(N_IMAGE_DELAYIMPORT_FUNCTION::records32,N_IMAGE_DELAYIMPORT_FUNCTION::__data_size));
        listLabels.append("Name");

        bool bIs64=g_pPE->is64();
        QList<XPE::DELAYIMPORT_POSITION> listDIP=g_pPE->getDelayImportPositions(g_nNumber);

        int nNumberOfDIPs=listDIP.count();

        *g_ppModel=new QStandardItemModel(nNumberOfDIPs,listLabels.count());

        setMaximum(nNumberOfDIPs);

        setHeader(*g_ppModel,&listLabels);

        for(int i=0; (i<nNumberOfDIPs)&&(isRun()); i++)
        {
            QStandardItem *pItem=new QStandardItem;
            pItem->setData(i,Qt::DisplayRole);

            (*g_ppModel)->setItem(i,0,pItem);

            if(bIs64)
            {
                (*g_ppModel)->setItem(i,N_IMAGE_DELAYIMPORT_FUNCTION::AddressThunk+1,     new QStandardItem(XBinary::valueToHex((quint64)listDIP.at(i).nAddressThunkRVA)));
                (*g_ppModel)->setItem(i,N_IMAGE_DELAYIMPORT_FUNCTION::BoundThunk+1,       new QStandardItem(XBinary::valueToHex((quint64)listDIP.at(i).nBoundThunkRVA)));
            }
            else
            {
                (*g_ppModel)->setItem(i,N_IMAGE_DELAYIMPORT_FUNCTION::AddressThunk+1,     new QStandardItem(XBinary::valueToHex((quint32)listDIP.at(i).nAddressThunkRVA)));
                (*g_ppModel)->setItem(i,N_IMAGE_DELAYIMPORT_FUNCTION::BoundThunk+1,       new QStandardItem(XBinary::valueToHex((quint32)listDIP.at(i).nBoundThunkRVA)));
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

                (*g_ppModel)->setItem(i,N_IMAGE_DELAYIMPORT_FUNCTION::Ordinal+1,   new QStandardItem(sOrdinal));
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

                (*g_ppModel)->setItem(i,N_IMAGE_DELAYIMPORT_FUNCTION::NameThunk+1,    new QStandardItem(sThunk));
                (*g_ppModel)->setItem(i,N_IMAGE_DELAYIMPORT_FUNCTION::Hint+1,         new QStandardItem(XBinary::valueToHex(listDIP.at(i).nHint)));
                (*g_ppModel)->setItem(i,N_IMAGE_DELAYIMPORT_FUNCTION::Hint+2,         new QStandardItem(listDIP.at(i).sName));
            }

            incValue();
        }
    }
    else if(g_nType==SPE::TYPE_RESOURCES)
    {
        XPE::RESOURCE_HEADER rh=g_pPE->getResourceHeader();

        *g_ppModel=new QStandardItemModel;

        int nNumberOfPositions=rh.listPositions.count();

        if(nNumberOfPositions)
        {
            QStandardItem *pRoot=new QStandardItem();
            pRoot->setText(tr("Resources"));

            (*g_ppModel)->appendRow(pRoot);

            for(int i=0; i<nNumberOfPositions; i++)
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

                int nNumberOfPositions=pos.listPositions.count();

                for(int j=0; (j<nNumberOfPositions)&&(isRun()); j++)
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

                    int nNumberOfSubPositions=subpos.listPositions.count();

                    for(int k=0; k<nNumberOfSubPositions; k++)
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
                        pRecord->setData(record3.dataEntry.Size,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE);
                        pRecord->setData(record3.dataEntry.OffsetToData,Qt::UserRole+FW_DEF::SECTION_DATA_ADDRESS);

                        if(g_pPE->isImage())
                        {
                            pRecord->setData(record3.dataEntry.OffsetToData,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET);
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
    XBinary::MODE mode=g_pPE->getMode();

    if(g_nType==SPE::TYPE_SECTIONS)
    {
        pTableView->setColumnWidth(0,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT16,mode));
        pTableView->setColumnWidth(1,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT32,mode));
        pTableView->setColumnWidth(2,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT32,mode));
        pTableView->setColumnWidth(3,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT32,mode));
        pTableView->setColumnWidth(4,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT32,mode));
        pTableView->setColumnWidth(5,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT32,mode));
        pTableView->setColumnWidth(6,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT32,mode));
        pTableView->setColumnWidth(7,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT32,mode));
        pTableView->setColumnWidth(8,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT32,mode));
        pTableView->setColumnWidth(9,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT32,mode));
        pTableView->setColumnWidth(10,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT32,mode));
    }
    else if(g_nType==SPE::TYPE_RELOCS)
    {
        pTableView->setColumnWidth(0,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT16,mode));
        pTableView->setColumnWidth(1,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT32,mode));
        pTableView->setColumnWidth(2,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT32,mode));
        pTableView->setColumnWidth(3,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT32,mode));
        pTableView->setColumnWidth(4,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_STRINGMID,mode));
    }
    else if(g_nType==SPE::TYPE_RELOCS_POSITION)
    {
        pTableView->setColumnWidth(0,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT16,mode));
        pTableView->setColumnWidth(1,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT32,mode));
        pTableView->setColumnWidth(2,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINTMODE,mode));
        pTableView->setColumnWidth(3,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINTMODE,mode));
    }
    else if(g_nType==SPE::TYPE_IMPORT)
    {
        pTableView->setColumnWidth(0,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT16,mode));
        pTableView->setColumnWidth(1,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT32,mode));
        pTableView->setColumnWidth(2,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT32,mode));
        pTableView->setColumnWidth(3,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT32,mode));
        pTableView->setColumnWidth(4,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT32,mode));
        pTableView->setColumnWidth(5,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT32,mode));
        pTableView->setColumnWidth(6,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT32,mode));
        pTableView->setColumnWidth(7,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_STRINGMID,mode));
    }
    else if(g_nType==SPE::TYPE_IMPORT_FUNCTION)
    {
        pTableView->setColumnWidth(0,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT16,mode));
        pTableView->setColumnWidth(1,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINTMODE,mode));
        pTableView->setColumnWidth(2,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINTMODE,mode));
        pTableView->setColumnWidth(3,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT16,mode));
        pTableView->setColumnWidth(4,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_STRINGLONG,mode));
    }
    else if(g_nType==SPE::TYPE_EXCEPTION)
    {
        pTableView->setColumnWidth(0,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT16,mode));
        pTableView->setColumnWidth(1,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT32,mode));
        pTableView->setColumnWidth(2,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT32,mode));
        pTableView->setColumnWidth(3,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT32,mode));
        pTableView->setColumnWidth(4,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_STRINGMID,mode));
    }
    else if(g_nType==SPE::TYPE_DELAYIMPORT)
    {
        pTableView->setColumnWidth(0,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT16,mode));
        pTableView->setColumnWidth(1,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT32,mode));
        pTableView->setColumnWidth(2,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT32,mode));
        pTableView->setColumnWidth(3,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT32,mode));
        pTableView->setColumnWidth(4,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT32,mode));
        pTableView->setColumnWidth(5,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT32,mode));
        pTableView->setColumnWidth(6,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT32,mode));
        pTableView->setColumnWidth(7,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT32,mode));
        pTableView->setColumnWidth(8,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT32,mode));
        pTableView->setColumnWidth(9,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_STRINGMID,mode));
    }
    else if(g_nType==SPE::TYPE_EXPORT_FUNCTION)
    {
        pTableView->setColumnWidth(0,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT16,mode));
        pTableView->setColumnWidth(1,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT32,mode));
        pTableView->setColumnWidth(2,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT32,mode));
        pTableView->setColumnWidth(3,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_STRINGLONG,mode));
    }
    else if(g_nType==SPE::TYPE_BOUNDIMPORT)
    {
        pTableView->setColumnWidth(0,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT16,mode));
        pTableView->setColumnWidth(1,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT32,mode));
        pTableView->setColumnWidth(2,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT32,mode));
        pTableView->setColumnWidth(3,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT32,mode));
        pTableView->setColumnWidth(4,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINTMODE,mode));
        pTableView->setColumnWidth(5,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_STRINGLONG,mode));
    }
    else if(g_nType==SPE::TYPE_DEBUG)
    {
        pTableView->setColumnWidth(0,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT16,mode));
        pTableView->setColumnWidth(1,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT32,mode));
        pTableView->setColumnWidth(2,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT32,mode));
        pTableView->setColumnWidth(3,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT32,mode));
        pTableView->setColumnWidth(4,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT32,mode));
        pTableView->setColumnWidth(5,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT32,mode));
        pTableView->setColumnWidth(6,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT32,mode));
        pTableView->setColumnWidth(7,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT32,mode));
        pTableView->setColumnWidth(8,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT32,mode));
    }
    else if(g_nType==SPE::TYPE_DELAYIMPORT_FUNCTION)
    {
        pTableView->setColumnWidth(0,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT16,mode));
        pTableView->setColumnWidth(1,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINTMODE,mode));
        pTableView->setColumnWidth(2,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINTMODE,mode));
        pTableView->setColumnWidth(3,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINTMODE,mode));
        pTableView->setColumnWidth(4,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINTMODE,mode));
        pTableView->setColumnWidth(5,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_UINT16,mode));
        pTableView->setColumnWidth(6,FormatWidget::getColumnWidth(pWidget,FormatWidget::CW_STRINGMID,mode));
    }
}

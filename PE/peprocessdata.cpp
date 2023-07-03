/* Copyright (c) 2017-2023 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "peprocessdata.h"

PEProcessData::PEProcessData(int nType, QStandardItemModel **ppModel, XPE *pPE, qint32 nNumber, qint64 nOffset, qint64 nSize, QVariant varInfo) : ProcessData()
{
    this->g_nType = nType;
    this->g_ppModel = ppModel;
    this->g_pPE = pPE;
    this->g_nNumber = nNumber;
    this->g_nOffset = nOffset;
    this->g_nSize = nSize;
    this->g_varInfo = varInfo;
}

void PEProcessData::_process()
{
    bool bIs64 = g_pPE->is64();

    if (g_nType == SPE::TYPE_SECTIONS) {
        QList<QString> listLabels;
        listLabels.append("#");
        listLabels.append(getStructList(N_IMAGE_SECTION_HEADER::records, N_IMAGE_SECTION_HEADER::__data_size));

        QList<XPE_DEF::IMAGE_SECTION_HEADER> listSections = g_pPE->getSectionHeaders();

        qint32 nNumberOfRecords = listSections.count();

        *g_ppModel = new QStandardItemModel(nNumberOfRecords, listLabels.count());

        setMaximum(nNumberOfRecords);

        setTableHeader(*g_ppModel, &listLabels);

        for (qint32 i = 0; (i < nNumberOfRecords) && (isRun()); i++) {
            QStandardItem *pItemNumber = new QStandardItem;
            pItemNumber->setData(i, Qt::DisplayRole);

            pItemNumber->setData(listSections.at(i).VirtualAddress, Qt::UserRole + FW_DEF::SECTION_DATA_ADDRESS);

            if (g_pPE->isImage()) {
                pItemNumber->setData(listSections.at(i).Misc.VirtualSize, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE);
                pItemNumber->setData(listSections.at(i).VirtualAddress, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET);
            } else {
                pItemNumber->setData(listSections.at(i).SizeOfRawData, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE);
                pItemNumber->setData(listSections.at(i).PointerToRawData, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET);
            }

            (*g_ppModel)->setItem(i, 0, pItemNumber);

            QStandardItem *pItemName = new QStandardItem();
            QString sName = QString((char *)listSections.at(i).Name);
            sName.resize(qMin(sName.length(), XPE_DEF::S_IMAGE_SIZEOF_SHORT_NAME));
            pItemName->setText(sName);

            pItemNumber->setData(QString("%1_%2_%3.bin").arg(tr("Section"), QString::number(i), XBinary::convertFileNameSymbols(sName)),
                                 Qt::UserRole + FW_DEF::SECTION_DATA_NAME);

            (*g_ppModel)->setItem(i, N_IMAGE_SECTION_HEADER::Name + 1, pItemName);

            (*g_ppModel)->setItem(i, N_IMAGE_SECTION_HEADER::VirtualSize + 1, new QStandardItem(XBinary::valueToHex(listSections.at(i).Misc.VirtualSize)));
            (*g_ppModel)->setItem(i, N_IMAGE_SECTION_HEADER::VirtualAddress + 1, new QStandardItem(XBinary::valueToHex(listSections.at(i).VirtualAddress)));
            (*g_ppModel)->setItem(i, N_IMAGE_SECTION_HEADER::SizeOfRawData + 1, new QStandardItem(XBinary::valueToHex(listSections.at(i).SizeOfRawData)));
            (*g_ppModel)->setItem(i, N_IMAGE_SECTION_HEADER::PointerToRawData + 1, new QStandardItem(XBinary::valueToHex(listSections.at(i).PointerToRawData)));
            (*g_ppModel)->setItem(i, N_IMAGE_SECTION_HEADER::PointerToRelocations + 1, new QStandardItem(XBinary::valueToHex(listSections.at(i).PointerToRelocations)));
            (*g_ppModel)->setItem(i, N_IMAGE_SECTION_HEADER::PointerToLinenumbers + 1, new QStandardItem(XBinary::valueToHex(listSections.at(i).PointerToLinenumbers)));
            (*g_ppModel)->setItem(i, N_IMAGE_SECTION_HEADER::NumberOfRelocations + 1, new QStandardItem(XBinary::valueToHex(listSections.at(i).NumberOfRelocations)));
            (*g_ppModel)->setItem(i, N_IMAGE_SECTION_HEADER::NumberOfLinenumbers + 1, new QStandardItem(XBinary::valueToHex(listSections.at(i).NumberOfLinenumbers)));
            (*g_ppModel)->setItem(i, N_IMAGE_SECTION_HEADER::Characteristics + 1, new QStandardItem(XBinary::valueToHex(listSections.at(i).Characteristics)));

            incValue();
        }
    } else if (g_nType == SPE::TYPE_SECTIONS_INFO) {
        *g_ppModel = new QStandardItemModel;

        QList<QString> listLabels;
        listLabels.append("#");
        listLabels.append(tr("Name"));
        listLabels.append(tr("Relative address"));
        listLabels.append(tr("Virtual size"));
        listLabels.append(tr("File offset"));
        listLabels.append(tr("Size"));
        listLabels.append(tr("Flags"));
        listLabels.append(tr("Info"));

        setTreeHeader(*g_ppModel, &listLabels);

        QList<XPE_DEF::IMAGE_SECTION_HEADER> listSectionHeaders = g_pPE->getSectionHeaders();
        QList<XPE::SECTION_RECORD> listSectionRecords = g_pPE->getSectionRecords(&listSectionHeaders);
        XBinary::_MEMORY_MAP memoryMap = g_pPE->getMemoryMap();

        QList<QString> listStrDb = XInfoDB::loadStrDB(getOptions()->getInfoPath(), XInfoDB::STRDB_PESECTIONS);

        qint32 nNumberOfSections = listSectionRecords.count();

        setMaximum(nNumberOfSections + 2);

        QStandardItem *pItemNumberHeader = nullptr;
        QStandardItem *pItemOverlay = nullptr;

        QList<QStandardItem *> listItemNameSections;

        {
            pItemNumberHeader = new QStandardItem("");

            QList<QStandardItem *> listItems;

            listItems.append(pItemNumberHeader);
            listItems.append(new QStandardItem(""));
            listItems.append(new QStandardItem(XBinary::valueToHex((quint32)0)));
            listItems.append(new QStandardItem(XBinary::valueToHex(S_ALIGN_UP32(g_pPE->getOptionalHeader_SizeOfHeaders(), g_pPE->getOptionalHeader_SectionAlignment()))));
            listItems.append(new QStandardItem(XBinary::valueToHex((quint32)0)));
            listItems.append(new QStandardItem(XBinary::valueToHex(g_pPE->getOptionalHeader_SizeOfHeaders())));
            listItems.append(new QStandardItem(""));
            listItems.append(new QStandardItem(tr("Header")));  // Info

            (*g_ppModel)->appendRow(listItems);

            incValue();
        }

        for (qint32 i = 0; i < nNumberOfSections; i++) {
            XInfoDB::STRRECORD strRecord = XInfoDB::handleStringDB(&listStrDb, listSectionRecords.at(i).sName, true);
            Qt::GlobalColor globalColor = XFormats::typeToColor(strRecord.sType);

            QColor colText;

            if (globalColor == Qt::transparent) {
                colText = QApplication::palette().text().color();
            } else {
                colText = QColor(globalColor);
            }

            strRecord.sType = XFormats::translateType(strRecord.sType);

            QStandardItem *pItemNumber = new QStandardItem(QString::number(i));

            QStandardItem *pItemName = new QStandardItem(listSectionRecords.at(i).sName);
            pItemName->setData(colText, Qt::ForegroundRole);

            QList<QStandardItem *> listItems;
            listItems.append(pItemNumber);
            listItems.append(pItemName);
            listItems.append(new QStandardItem(XBinary::valueToHex(listSectionHeaders.at(i).VirtualAddress)));
            listItems.append(new QStandardItem(XBinary::valueToHex(listSectionHeaders.at(i).Misc.VirtualSize)));
            listItems.append(new QStandardItem(XBinary::valueToHex(listSectionHeaders.at(i).PointerToRawData)));
            listItems.append(new QStandardItem(XBinary::valueToHex(listSectionHeaders.at(i).SizeOfRawData)));
            listItems.append(new QStandardItem(XPE::sectionCharacteristicToString(listSectionHeaders.at(i).Characteristics)));
            listItems.append(new QStandardItem(strRecord.sDescription));  // Info

            (*g_ppModel)->appendRow(listItems);

            listItemNameSections.append(pItemNumber);

            incValue();
        }

        if (g_pPE->isOverlayPresent(&memoryMap)) {
            pItemOverlay = new QStandardItem("");

            QList<QStandardItem *> listItems;
            listItems.append(pItemOverlay);
            listItems.append(new QStandardItem(""));
            listItems.append(new QStandardItem(""));
            listItems.append(new QStandardItem(""));
            listItems.append(new QStandardItem(XBinary::valueToHex((quint32)(g_pPE->getOverlayOffset(&memoryMap)))));
            listItems.append(new QStandardItem(XBinary::valueToHex((quint32)(g_pPE->getOverlaySize(&memoryMap)))));
            listItems.append(new QStandardItem(""));
            listItems.append(new QStandardItem(tr("Overlay")));  // Info

            (*g_ppModel)->appendRow(listItems);

            incValue();
        }

        QMap<qint64, QString> mapRegionAddresses;
        QMap<qint64, QString> mapRegionSizes;

        {
            QString sRecord = tr("Entry point");

            mapRegionAddresses.insert(g_pPE->getOptionalHeader_AddressOfEntryPoint(), sRecord);
            mapRegionSizes.insert(0, sRecord);
        }

        if (g_pPE->isExportPresent()) {
            QString sRecord = tr("Export");

            XPE_DEF::IMAGE_DATA_DIRECTORY dd = g_pPE->getOptionalHeader_DataDirectory(XPE_DEF::S_IMAGE_DIRECTORY_ENTRY_EXPORT);

            mapRegionAddresses.insert(dd.VirtualAddress, sRecord);
            mapRegionSizes.insert(dd.Size, sRecord);
        }

        if (g_pPE->isImportPresent()) {
            QString sRecord = tr("Import");

            XPE_DEF::IMAGE_DATA_DIRECTORY dd = g_pPE->getOptionalHeader_DataDirectory(XPE_DEF::S_IMAGE_DIRECTORY_ENTRY_IMPORT);

            mapRegionAddresses.insert(dd.VirtualAddress, sRecord);
            mapRegionSizes.insert(dd.Size, sRecord);
        }

        if (g_pPE->isResourcesPresent()) {
            QString sRecord = tr("Resources");

            XPE_DEF::IMAGE_DATA_DIRECTORY dd = g_pPE->getOptionalHeader_DataDirectory(XPE_DEF::S_IMAGE_DIRECTORY_ENTRY_RESOURCE);

            mapRegionAddresses.insert(dd.VirtualAddress, sRecord);
            mapRegionSizes.insert(dd.Size, sRecord);
        }

        if (g_pPE->isExceptionPresent()) {
            QString sRecord = tr("Exceptions");

            XPE_DEF::IMAGE_DATA_DIRECTORY dd = g_pPE->getOptionalHeader_DataDirectory(XPE_DEF::S_IMAGE_DIRECTORY_ENTRY_EXCEPTION);

            mapRegionAddresses.insert(dd.VirtualAddress, sRecord);
            mapRegionSizes.insert(dd.Size, sRecord);
        }

        if (g_pPE->isRelocsPresent()) {
            QString sRecord = tr("Relocs");

            XPE_DEF::IMAGE_DATA_DIRECTORY dd = g_pPE->getOptionalHeader_DataDirectory(XPE_DEF::S_IMAGE_DIRECTORY_ENTRY_BASERELOC);

            mapRegionAddresses.insert(dd.VirtualAddress, sRecord);
            mapRegionSizes.insert(dd.Size, sRecord);
        }

        if (g_pPE->isDebugPresent()) {
            QString sRecord = tr("Debug");

            XPE_DEF::IMAGE_DATA_DIRECTORY dd = g_pPE->getOptionalHeader_DataDirectory(XPE_DEF::S_IMAGE_DIRECTORY_ENTRY_DEBUG);

            mapRegionAddresses.insert(dd.VirtualAddress, sRecord);
            mapRegionSizes.insert(dd.Size, sRecord);
        }

        if (g_pPE->isTLSPresent()) {
            QString sRecord = QString("TLS");

            XPE_DEF::IMAGE_DATA_DIRECTORY dd = g_pPE->getOptionalHeader_DataDirectory(XPE_DEF::S_IMAGE_DIRECTORY_ENTRY_TLS);

            mapRegionAddresses.insert(dd.VirtualAddress, sRecord);
            mapRegionSizes.insert(dd.Size, sRecord);
        }

        if (g_pPE->isLoadConfigPresent()) {
            QString sRecord = tr("Load config");

            XPE_DEF::IMAGE_DATA_DIRECTORY dd = g_pPE->getOptionalHeader_DataDirectory(XPE_DEF::S_IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG);

            mapRegionAddresses.insert(dd.VirtualAddress, sRecord);
            mapRegionSizes.insert(dd.Size, sRecord);
        }

        if (g_pPE->isBoundImportPresent()) {
            QString sRecord = tr("Bound import");

            XPE_DEF::IMAGE_DATA_DIRECTORY dd = g_pPE->getOptionalHeader_DataDirectory(XPE_DEF::S_IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT);

            mapRegionAddresses.insert(dd.VirtualAddress, sRecord);
            mapRegionSizes.insert(dd.Size, sRecord);
        }

        if (g_pPE->isIATPresent()) {
            QString sRecord = QString("IAT");

            XPE_DEF::IMAGE_DATA_DIRECTORY dd = g_pPE->getOptionalHeader_DataDirectory(XPE_DEF::S_IMAGE_DIRECTORY_ENTRY_IAT);

            mapRegionAddresses.insert(dd.VirtualAddress, sRecord);
            mapRegionSizes.insert(dd.Size, sRecord);
        }

        if (g_pPE->isDelayImportPresent()) {
            QString sRecord = tr("Delay import");

            XPE_DEF::IMAGE_DATA_DIRECTORY dd = g_pPE->getOptionalHeader_DataDirectory(XPE_DEF::S_IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT);

            mapRegionAddresses.insert(dd.VirtualAddress, sRecord);
            mapRegionSizes.insert(dd.Size, sRecord);
        }

        if (g_pPE->isNETPresent()) {
            QString sRecord = QString(".NET");

            XPE_DEF::IMAGE_DATA_DIRECTORY dd = g_pPE->getOptionalHeader_DataDirectory(XPE_DEF::S_IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR);

            mapRegionAddresses.insert(dd.VirtualAddress, sRecord);
            mapRegionSizes.insert(dd.Size, sRecord);
        }

        QMapIterator<qint64, QString> iter(mapRegionAddresses);

        while (iter.hasNext()) {
            iter.next();

            qint64 nRelAddress = iter.key();
            qint64 nOffset = g_pPE->relAddressToOffset(&memoryMap, nRelAddress);
            QString sOffset;

            if (nOffset != -1) {
                sOffset = XBinary::valueToHex((quint32)(nOffset));
            }

            QString sName = iter.value();

            qint32 nSection = XBinary::relAddressToLoadSection(&memoryMap, nRelAddress);
            bool bIsHeader = false;

            if (nSection == -1) {
                bIsHeader = XBinary::isRelAddressInHeader(&memoryMap, nRelAddress);
            }

            if ((nSection != -1) || (bIsHeader)) {
                QList<QStandardItem *> listItems;
                listItems.append(new QStandardItem(""));
                listItems.append(new QStandardItem(sName));
                listItems.append(new QStandardItem(XBinary::valueToHex((quint32)(nRelAddress))));
                listItems.append(new QStandardItem(XBinary::valueToHex((quint32)(mapRegionSizes.key(sName)))));
                listItems.append(new QStandardItem(sOffset));
                listItems.append(new QStandardItem(XBinary::valueToHex((quint32)(mapRegionSizes.key(sName)))));
                listItems.append(new QStandardItem(""));
                listItems.append(new QStandardItem(""));  // Info

                if (nSection != -1) {
                    listItemNameSections.at(nSection)->appendRow(listItems);
                } else if (bIsHeader) {
                    pItemNumberHeader->appendRow(listItems);
                }
            }
        }
    } else if (g_nType == SPE::TYPE_RELOCS) {
        QList<QString> listLabels;
        listLabels.append("#");
        listLabels.append(getStructList(N_IMAGE_RELOCS::records, N_IMAGE_RELOCS::__data_size));
        listLabels.append("");
        listLabels.append("");

        QList<XPE::RELOCS_HEADER> listRelocsHeaders = g_pPE->getRelocsHeaders();

        qint32 nNumberOfRecords = listRelocsHeaders.count();

        *g_ppModel = new QStandardItemModel(nNumberOfRecords, listLabels.count());

        setMaximum(nNumberOfRecords);

        setTableHeader(*g_ppModel, &listLabels);

        for (qint32 i = 0; (i < nNumberOfRecords) && (isRun()); i++) {
            QStandardItem *pItem = new QStandardItem;
            pItem->setData(i, Qt::DisplayRole);
            pItem->setData(listRelocsHeaders.at(i).nOffset, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET);
            pItem->setData(listRelocsHeaders.at(i).nOffset, Qt::UserRole + FW_DEF::SECTION_DATA_HEADEROFFSET);
            (*g_ppModel)->setItem(i, 0, pItem);
            (*g_ppModel)->setItem(i, N_IMAGE_RELOCS::VirtualAddress + 1, new QStandardItem(XBinary::valueToHex(listRelocsHeaders.at(i).baseRelocation.VirtualAddress)));
            (*g_ppModel)->setItem(i, N_IMAGE_RELOCS::SizeOfBlock + 1, new QStandardItem(XBinary::valueToHex(listRelocsHeaders.at(i).baseRelocation.SizeOfBlock)));
            (*g_ppModel)->setItem(i, N_IMAGE_RELOCS::SizeOfBlock + 2, new QStandardItem(QString::number(listRelocsHeaders.at(i).nCount)));
            (*g_ppModel)
                ->setItem(i, N_IMAGE_RELOCS::SizeOfBlock + 3,
                          new QStandardItem(g_pPE->getMemoryRecordInfoByRelAddress(listRelocsHeaders.at(i).baseRelocation.VirtualAddress)));  // Comment

            incValue();
        }
    } else if (g_nType == SPE::TYPE_RELOCS_POSITION) {
        QList<QString> listLabels;
        listLabels.append("#");
        listLabels.append(getStructList(N_IMAGE_RELOCS_POSITION::records, N_IMAGE_RELOCS_POSITION::__data_size));
        listLabels.append(tr("Type"));
        listLabels.append(tr("Address"));

        QList<XPE::RELOCS_POSITION> listRelocsPositions = g_pPE->getRelocsPositions(g_nOffset);

        int nNumberOfRelocs = listRelocsPositions.count();

        *g_ppModel = new QStandardItemModel(nNumberOfRelocs, listLabels.count());

        setMaximum(nNumberOfRelocs);

        setTableHeader(*g_ppModel, &listLabels);

        QMap<quint64, QString> mapTypes = g_pPE->getImageRelBasedS();

        for (qint32 i = 0; (i < nNumberOfRelocs) && (isRun()); i++) {
            QStandardItem *pItem = new QStandardItem;
            pItem->setData(i, Qt::DisplayRole);
            (*g_ppModel)->setItem(i, 0, pItem);
            (*g_ppModel)->setItem(i, N_IMAGE_RELOCS_POSITION::TypeOffset + 1, new QStandardItem(XBinary::valueToHex(listRelocsPositions.at(i).nTypeOffset)));
            (*g_ppModel)->setItem(i, N_IMAGE_RELOCS_POSITION::TypeOffset + 2, new QStandardItem(mapTypes.value(listRelocsPositions.at(i).nType)));
            (*g_ppModel)->setItem(i, N_IMAGE_RELOCS_POSITION::TypeOffset + 3, new QStandardItem(XBinary::valueToHex((quint32)listRelocsPositions.at(i).nAddress)));

            incValue();
        }
    } else if (g_nType == SPE::TYPE_IMPORT) {
        QList<QString> listLabels;
        listLabels.append("#");
        listLabels.append(getStructList(N_IMAGE_IMPORT::records, N_IMAGE_IMPORT::__data_size));
        listLabels.append(tr("Hash"));
        listLabels.append(tr("Name"));

        XBinary::_MEMORY_MAP memoryMap = g_pPE->getMemoryMap();

        QList<XPE::IMPORT_HEADER> listImport = g_pPE->getImports(&memoryMap);

        QList<quint32> listImportPositionHashes = g_pPE->getImportPositionHashes(&listImport);

        QList<XPE::IMAGE_IMPORT_DESCRIPTOR_EX> listID = g_pPE->getImportDescriptorsEx(&memoryMap);

        int nNumberOfIDs = listID.count();

        *g_ppModel = new QStandardItemModel(nNumberOfIDs, listLabels.count());

        setMaximum(nNumberOfIDs);

        setTableHeader(*g_ppModel, &listLabels);

        for (qint32 i = 0; (i < nNumberOfIDs) && (isRun()); i++) {
            QStandardItem *pItem = new QStandardItem;
            pItem->setData(i, Qt::DisplayRole);
            (*g_ppModel)->setItem(i, 0, pItem);
            (*g_ppModel)->setItem(i, N_IMAGE_IMPORT::OriginalFirstThunk + 1, new QStandardItem(XBinary::valueToHex(listID.at(i).OriginalFirstThunk)));
            (*g_ppModel)->setItem(i, N_IMAGE_IMPORT::TimeDateStamp + 1, new QStandardItem(XBinary::valueToHex(listID.at(i).TimeDateStamp)));
            (*g_ppModel)->setItem(i, N_IMAGE_IMPORT::ForwarderChain + 1, new QStandardItem(XBinary::valueToHex(listID.at(i).ForwarderChain)));
            (*g_ppModel)->setItem(i, N_IMAGE_IMPORT::Name + 1, new QStandardItem(XBinary::valueToHex(listID.at(i).Name)));
            (*g_ppModel)->setItem(i, N_IMAGE_IMPORT::FirstThunk + 1, new QStandardItem(XBinary::valueToHex(listID.at(i).FirstThunk)));
            (*g_ppModel)->setItem(i, N_IMAGE_IMPORT::FirstThunk + 2, new QStandardItem(XBinary::valueToHex(listImportPositionHashes.at(i))));
            (*g_ppModel)->setItem(i, N_IMAGE_IMPORT::FirstThunk + 3, new QStandardItem(listID.at(i).sLibrary));

            incValue();
        }
    } else if (g_nType == SPE::TYPE_IMPORT_FUNCTION) {
        QList<QString> listLabels;
        listLabels.append("#");
        listLabels.append(getStructList(N_IMAGE_IMPORT_FUNCTION::records32, N_IMAGE_IMPORT_FUNCTION::__data_size));
        listLabels.append(tr("Name"));

        QList<XPE::IMPORT_POSITION> listImportPositions = g_pPE->getImportPositions(g_nNumber);

        int nNumberOfIPs = listImportPositions.count();

        *g_ppModel = new QStandardItemModel(nNumberOfIPs, listLabels.count());

        setMaximum(nNumberOfIPs);

        setTableHeader(*g_ppModel, &listLabels);

        for (qint32 i = 0; (i < nNumberOfIPs) && (isRun()); i++) {
            QStandardItem *pItem = new QStandardItem;
            pItem->setData(i, Qt::DisplayRole);

            (*g_ppModel)->setItem(i, 0, pItem);

            if (listImportPositions.at(i).nOrdinal) {
                QString sOrdinal;

                if (bIs64) {
                    sOrdinal = XBinary::valueToHex((quint64)listImportPositions.at(i).nOrdinal);
                } else {
                    sOrdinal = XBinary::valueToHex((quint32)listImportPositions.at(i).nOrdinal);
                }

                (*g_ppModel)->setItem(i, N_IMAGE_IMPORT_FUNCTION::Ordinal + 1, new QStandardItem(sOrdinal));
            } else {
                QString sThunk;

                if (bIs64) {
                    sThunk = XBinary::valueToHex((quint64)listImportPositions.at(i).nThunkValue);
                } else {
                    sThunk = XBinary::valueToHex((quint32)listImportPositions.at(i).nThunkValue);
                }

                (*g_ppModel)->setItem(i, N_IMAGE_IMPORT_FUNCTION::Thunk + 1, new QStandardItem(sThunk));
                (*g_ppModel)->setItem(i, N_IMAGE_IMPORT_FUNCTION::Hint + 1, new QStandardItem(XBinary::valueToHex(listImportPositions.at(i).nHint)));
                (*g_ppModel)->setItem(i, N_IMAGE_IMPORT_FUNCTION::Hint + 2, new QStandardItem(listImportPositions.at(i).sName));
            }

            incValue();
        }
    } else if (g_nType == SPE::TYPE_EXCEPTION) {
        QList<QString> listLabels;
        listLabels.append("#");
        listLabels.append(getStructList(N_IMAGE_EXCEPTIONS::records, N_IMAGE_EXCEPTIONS::__data_size));
        listLabels.append("");

        QList<XPE_DEF::S_IMAGE_RUNTIME_FUNCTION_ENTRY> listRFE = g_pPE->getExceptionsList();

        int nNumberOfRFEs = listRFE.count();

        *g_ppModel = new QStandardItemModel(nNumberOfRFEs, listLabels.count());

        setMaximum(nNumberOfRFEs);

        setTableHeader(*g_ppModel, &listLabels);

        XBinary::_MEMORY_MAP memoryMap = g_pPE->getMemoryMap();

        for (qint32 i = 0; (i < nNumberOfRFEs) && (isRun()); i++) {
            QStandardItem *pItem = new QStandardItem;
            pItem->setData(i, Qt::DisplayRole);

            pItem->setData(listRFE.at(i).BeginAddress, Qt::UserRole + FW_DEF::SECTION_DATA_ADDRESS);
            pItem->setData(listRFE.at(i).EndAddress - listRFE.at(i).BeginAddress, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE);
            pItem->setData(g_pPE->addressToOffset(&memoryMap, memoryMap.nModuleAddress + listRFE.at(i).BeginAddress), Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET);

            (*g_ppModel)->setItem(i, 0, pItem);
            (*g_ppModel)->setItem(i, N_IMAGE_EXCEPTIONS::BeginAddress + 1, new QStandardItem(XBinary::valueToHex(listRFE.at(i).BeginAddress)));
            (*g_ppModel)->setItem(i, N_IMAGE_EXCEPTIONS::EndAddress + 1, new QStandardItem(XBinary::valueToHex(listRFE.at(i).EndAddress)));
            (*g_ppModel)->setItem(i, N_IMAGE_EXCEPTIONS::UnwindInfoAddress + 1, new QStandardItem(XBinary::valueToHex(listRFE.at(i).UnwindInfoAddress)));
            (*g_ppModel)
                ->setItem(i, N_IMAGE_EXCEPTIONS::UnwindInfoAddress + 2,
                          new QStandardItem(g_pPE->getMemoryRecordInfoByRelAddress(listRFE.at(i).BeginAddress)));  // Comment

            incValue();
        }
    } else if (g_nType == SPE::TYPE_DELAYIMPORT) {
        QList<QString> listLabels;
        listLabels.append("#");
        listLabels.append(getStructList(N_IMAGE_DELAYIMPORT::records, N_IMAGE_DELAYIMPORT::__data_size));
        listLabels.append("");

        QList<XPE_DEF::S_IMAGE_DELAYLOAD_DESCRIPTOR> listDelayImport = g_pPE->getDelayImportsList();

        int nNumberOfDelayImports = listDelayImport.count();

        *g_ppModel = new QStandardItemModel(nNumberOfDelayImports, listLabels.count());

        setMaximum(nNumberOfDelayImports);

        setTableHeader(*g_ppModel, &listLabels);

        XBinary::_MEMORY_MAP memoryMap = g_pPE->getMemoryMap();

        for (qint32 i = 0; (i < nNumberOfDelayImports) && (isRun()); i++) {
            QString sLibraryName = g_pPE->read_ansiString(g_pPE->relAddressToOffset(&memoryMap, listDelayImport.at(i).DllNameRVA));

            QStandardItem *pItem = new QStandardItem;
            pItem->setData(i, Qt::DisplayRole);

            (*g_ppModel)->setItem(i, 0, pItem);
            (*g_ppModel)->setItem(i, N_IMAGE_DELAYIMPORT::AllAttributes + 1, new QStandardItem(XBinary::valueToHex(listDelayImport.at(i).AllAttributes)));
            (*g_ppModel)->setItem(i, N_IMAGE_DELAYIMPORT::DllNameRVA + 1, new QStandardItem(XBinary::valueToHex(listDelayImport.at(i).DllNameRVA)));
            (*g_ppModel)->setItem(i, N_IMAGE_DELAYIMPORT::ModuleHandleRVA + 1, new QStandardItem(XBinary::valueToHex(listDelayImport.at(i).ModuleHandleRVA)));
            (*g_ppModel)->setItem(i, N_IMAGE_DELAYIMPORT::ImportAddressTableRVA + 1, new QStandardItem(XBinary::valueToHex(listDelayImport.at(i).ImportAddressTableRVA)));
            (*g_ppModel)->setItem(i, N_IMAGE_DELAYIMPORT::ImportNameTableRVA + 1, new QStandardItem(XBinary::valueToHex(listDelayImport.at(i).ImportNameTableRVA)));
            (*g_ppModel)
                ->setItem(i, N_IMAGE_DELAYIMPORT::BoundImportAddressTableRVA + 1,
                          new QStandardItem(XBinary::valueToHex(listDelayImport.at(i).BoundImportAddressTableRVA)));
            (*g_ppModel)
                ->setItem(i, N_IMAGE_DELAYIMPORT::UnloadInformationTableRVA + 1, new QStandardItem(XBinary::valueToHex(listDelayImport.at(i).UnloadInformationTableRVA)));
            (*g_ppModel)->setItem(i, N_IMAGE_DELAYIMPORT::TimeDateStamp + 1, new QStandardItem(XBinary::valueToHex(listDelayImport.at(i).TimeDateStamp)));
            (*g_ppModel)->setItem(i, N_IMAGE_DELAYIMPORT::TimeDateStamp + 2, new QStandardItem(sLibraryName));

            incValue();
        }
    } else if (g_nType == SPE::TYPE_EXPORT_FUNCTION) {
        QList<QString> listLabels;
        // No need number
        listLabels.append(getStructList(N_IMAGE_EXPORT_FUNCTION::records, N_IMAGE_EXPORT_FUNCTION::__data_size));
        listLabels.append("");

        bool bFilter = g_varInfo.toBool();

        XPE::EXPORT_HEADER eh = g_pPE->getExport(bFilter);

        int nNumberOfPositions = eh.listPositions.count();

        *g_ppModel = new QStandardItemModel(nNumberOfPositions, listLabels.count());

        setMaximum(nNumberOfPositions);

        setTableHeader(*g_ppModel, &listLabels);

        XBinary::_MEMORY_MAP memoryMap = g_pPE->getMemoryMap();

        for (qint32 i = 0; (i < nNumberOfPositions) && (isRun()); i++) {
            QStandardItem *pItem = new QStandardItem;
            pItem->setText(XBinary::valueToHex(eh.listPositions.at(i).nOrdinal));

            pItem->setData(memoryMap.nModuleAddress + eh.listPositions.at(i).nRVA, Qt::UserRole + FW_DEF::SECTION_DATA_ADDRESS);
            pItem->setData(1, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE);
            pItem->setData(g_pPE->addressToOffset(&memoryMap, memoryMap.nModuleAddress + eh.listPositions.at(i).nRVA), Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET);

            (*g_ppModel)->setItem(i, N_IMAGE_EXPORT_FUNCTION::Ordinal, pItem);
            (*g_ppModel)->setItem(i, N_IMAGE_EXPORT_FUNCTION::RVA, new QStandardItem(XBinary::valueToHex(eh.listPositions.at(i).nRVA)));
            (*g_ppModel)->setItem(i, N_IMAGE_EXPORT_FUNCTION::Name, new QStandardItem(XBinary::valueToHex(eh.listPositions.at(i).nNameRVA)));
            (*g_ppModel)->setItem(i, N_IMAGE_EXPORT_FUNCTION::Name + 1, new QStandardItem(eh.listPositions.at(i).sFunctionName));

            incValue();
        }
    } else if (g_nType == SPE::TYPE_BOUNDIMPORT) {
        QList<QString> listLabels;
        listLabels.append("#");
        listLabels.append(getStructList(N_IMAGE_BOUNDIMPORT::records, N_IMAGE_BOUNDIMPORT::__data_size));
        listLabels.append("");
        listLabels.append("");

        QList<XPE::BOUND_IMPORT_POSITION> listBoundImportPositions = g_pPE->getBoundImportPositions();

        int nNumberOfPositions = listBoundImportPositions.count();

        *g_ppModel = new QStandardItemModel(nNumberOfPositions, listLabels.count());

        setMaximum(nNumberOfPositions);

        setTableHeader(*g_ppModel, &listLabels);

        for (qint32 i = 0; (i < nNumberOfPositions) && (isRun()); i++) {
            QStandardItem *pItem = new QStandardItem;
            pItem->setData(i, Qt::DisplayRole);

            (*g_ppModel)->setItem(i, 0, pItem);
            (*g_ppModel)
                ->setItem(i, N_IMAGE_BOUNDIMPORT::TimeDateStamp + 1, new QStandardItem(XBinary::valueToHex(listBoundImportPositions.at(i).descriptor.TimeDateStamp)));
            (*g_ppModel)
                ->setItem(i, N_IMAGE_BOUNDIMPORT::OffsetModuleName + 1,
                          new QStandardItem(XBinary::valueToHex(listBoundImportPositions.at(i).descriptor.OffsetModuleName)));
            (*g_ppModel)
                ->setItem(i, N_IMAGE_BOUNDIMPORT::NumberOfModuleForwarderRefs + 1,
                          new QStandardItem(XBinary::valueToHex(listBoundImportPositions.at(i).descriptor.NumberOfModuleForwarderRefs)));
            (*g_ppModel)
                ->setItem(i, N_IMAGE_BOUNDIMPORT::NumberOfModuleForwarderRefs + 2,
                          new QStandardItem(XBinary::valueToTimeString(listBoundImportPositions.at(i).descriptor.TimeDateStamp, XBinary::DT_TYPE_POSIX)));
            (*g_ppModel)->setItem(i, N_IMAGE_BOUNDIMPORT::NumberOfModuleForwarderRefs + 3, new QStandardItem(listBoundImportPositions.at(i).sName));

            incValue();
        }
    } else if (g_nType == SPE::TYPE_DEBUG) {
        QList<QString> listLabels;
        listLabels.append("#");
        listLabels.append(getStructList(N_IMAGE_DEBUG::records, N_IMAGE_DEBUG::__data_size));

        QList<XPE_DEF::S_IMAGE_DEBUG_DIRECTORY> listDebug = g_pPE->getDebugList();

        int nNumberOfDebugs = listDebug.count();

        *g_ppModel = new QStandardItemModel(nNumberOfDebugs, listLabels.count());

        setMaximum(nNumberOfDebugs);

        setTableHeader(*g_ppModel, &listLabels);

        for (qint32 i = 0; (i < nNumberOfDebugs) && (isRun()); i++) {
            QStandardItem *pItem = new QStandardItem;
            pItem->setData(i, Qt::DisplayRole);
            pItem->setData(listDebug.at(i).AddressOfRawData, Qt::UserRole + FW_DEF::SECTION_DATA_ADDRESS);
            pItem->setData(listDebug.at(i).SizeOfData, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE);
            pItem->setData(listDebug.at(i).PointerToRawData, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET);
            (*g_ppModel)->setItem(i, 0, pItem);
            (*g_ppModel)->setItem(i, N_IMAGE_DEBUG::Characteristics + 1, new QStandardItem(XBinary::valueToHex(listDebug.at(i).Characteristics)));
            (*g_ppModel)->setItem(i, N_IMAGE_DEBUG::TimeDateStamp + 1, new QStandardItem(XBinary::valueToHex(listDebug.at(i).TimeDateStamp)));
            (*g_ppModel)->setItem(i, N_IMAGE_DEBUG::MajorVersion + 1, new QStandardItem(XBinary::valueToHex(listDebug.at(i).MajorVersion)));
            (*g_ppModel)->setItem(i, N_IMAGE_DEBUG::MinorVersion + 1, new QStandardItem(XBinary::valueToHex(listDebug.at(i).MinorVersion)));
            (*g_ppModel)->setItem(i, N_IMAGE_DEBUG::Type + 1, new QStandardItem(XBinary::valueToHex(listDebug.at(i).Type)));
            (*g_ppModel)->setItem(i, N_IMAGE_DEBUG::SizeOfData + 1, new QStandardItem(XBinary::valueToHex(listDebug.at(i).SizeOfData)));
            (*g_ppModel)->setItem(i, N_IMAGE_DEBUG::AddressOfRawData + 1, new QStandardItem(XBinary::valueToHex(listDebug.at(i).AddressOfRawData)));
            (*g_ppModel)->setItem(i, N_IMAGE_DEBUG::PointerToRawData + 1, new QStandardItem(XBinary::valueToHex(listDebug.at(i).PointerToRawData)));

            incValue();
        }
    } else if (g_nType == SPE::TYPE_TLSCALLBACKS) {
        QList<QString> listLabels;
        listLabels.append("#");
        listLabels.append(tr("Address"));
        listLabels.append("");

        QList<XADDR> listCallbacks = g_pPE->getTLS_CallbacksList();

        int nNumberOfRecords = listCallbacks.count();

        *g_ppModel = new QStandardItemModel(nNumberOfRecords, listLabels.count());

        setMaximum(nNumberOfRecords);

        setTableHeader(*g_ppModel, &listLabels);

        for (qint32 i = 0; (i < nNumberOfRecords) && (isRun()); i++) {
            XADDR nAddress = listCallbacks.at(i);

            QStandardItem *pItem = new QStandardItem;
            pItem->setData(i, Qt::DisplayRole);
            pItem->setData(nAddress, Qt::UserRole + FW_DEF::SECTION_DATA_ADDRESS);
            //                        pItem->setData(listCallbacks.at(i).SizeOfData,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE);
            //                        pItem->setData(listCallbacks.at(i).PointerToRawData,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET);
            (*g_ppModel)->setItem(i, 0, pItem);
            if (bIs64) {
                (*g_ppModel)->setItem(i, 1, new QStandardItem(XBinary::valueToHex((quint64)nAddress)));
            } else {
                (*g_ppModel)->setItem(i, 1, new QStandardItem(XBinary::valueToHex((quint32)nAddress)));
            }

            (*g_ppModel)->setItem(i, 2, new QStandardItem(g_pPE->getMemoryRecordInfoByAddress(nAddress)));

            incValue();
        }
    } else if (g_nType == SPE::TYPE_DELAYIMPORT_FUNCTION) {
        QList<QString> listLabels;
        listLabels.append("#");
        listLabels.append(getStructList(N_IMAGE_DELAYIMPORT_FUNCTION::records32, N_IMAGE_DELAYIMPORT_FUNCTION::__data_size));
        listLabels.append(tr("Name"));

        QList<XPE::DELAYIMPORT_POSITION> listDIP = g_pPE->getDelayImportPositions(g_nNumber);

        int nNumberOfDIPs = listDIP.count();

        *g_ppModel = new QStandardItemModel(nNumberOfDIPs, listLabels.count());

        setMaximum(nNumberOfDIPs);

        setTableHeader(*g_ppModel, &listLabels);

        for (qint32 i = 0; (i < nNumberOfDIPs) && (isRun()); i++) {
            QStandardItem *pItem = new QStandardItem;
            pItem->setData(i, Qt::DisplayRole);

            (*g_ppModel)->setItem(i, 0, pItem);

            if (bIs64) {
                (*g_ppModel)->setItem(i, N_IMAGE_DELAYIMPORT_FUNCTION::AddressThunk + 1, new QStandardItem(XBinary::valueToHex((quint64)listDIP.at(i).nAddressThunkRVA)));
                (*g_ppModel)->setItem(i, N_IMAGE_DELAYIMPORT_FUNCTION::BoundThunk + 1, new QStandardItem(XBinary::valueToHex((quint64)listDIP.at(i).nBoundThunkRVA)));
            } else {
                (*g_ppModel)->setItem(i, N_IMAGE_DELAYIMPORT_FUNCTION::AddressThunk + 1, new QStandardItem(XBinary::valueToHex((quint32)listDIP.at(i).nAddressThunkRVA)));
                (*g_ppModel)->setItem(i, N_IMAGE_DELAYIMPORT_FUNCTION::BoundThunk + 1, new QStandardItem(XBinary::valueToHex((quint32)listDIP.at(i).nBoundThunkRVA)));
            }

            if (listDIP.at(i).nOrdinal) {
                QString sOrdinal;

                if (bIs64) {
                    sOrdinal = XBinary::valueToHex((quint64)listDIP.at(i).nOrdinal);
                } else {
                    sOrdinal = XBinary::valueToHex((quint32)listDIP.at(i).nOrdinal);
                }

                (*g_ppModel)->setItem(i, N_IMAGE_DELAYIMPORT_FUNCTION::Ordinal + 1, new QStandardItem(sOrdinal));
            } else {
                QString sThunk;

                if (bIs64) {
                    sThunk = XBinary::valueToHex((quint64)listDIP.at(i).nNameThunkValue);
                } else {
                    sThunk = XBinary::valueToHex((quint32)listDIP.at(i).nNameThunkValue);
                }

                (*g_ppModel)->setItem(i, N_IMAGE_DELAYIMPORT_FUNCTION::NameThunk + 1, new QStandardItem(sThunk));
                (*g_ppModel)->setItem(i, N_IMAGE_DELAYIMPORT_FUNCTION::Hint + 1, new QStandardItem(XBinary::valueToHex(listDIP.at(i).nHint)));
                (*g_ppModel)->setItem(i, N_IMAGE_DELAYIMPORT_FUNCTION::Hint + 2, new QStandardItem(listDIP.at(i).sName));
            }

            incValue();
        }
    } else if (g_nType == SPE::TYPE_RESOURCES) {
        if (g_varInfo.toBool()) {
            XPE::RESOURCE_HEADER rh = g_pPE->getResourceHeader();

            *g_ppModel = new QStandardItemModel;

            int nNumberOfPositions = rh.listPositions.count();

            if (nNumberOfPositions) {
                QStandardItem *pRoot = new QStandardItem();
                pRoot->setText(tr("Resources"));

                (*g_ppModel)->appendRow(pRoot);

                for (qint32 i = 0; i < nNumberOfPositions; i++) {
                    XPE::RESOURCE_POSITION pos = rh.listPositions.at(i);
                    QStandardItem *pPos = new QStandardItem;
                    QString sPosText = XPE::resourceIdNameToString(pos.rin, 0);

                    pPos->setText(sPosText);

                    pRoot->appendRow(pPos);

                    int nNumberOfPositions = pos.listPositions.count();

                    for (qint32 j = 0; (j < nNumberOfPositions) && (isRun()); j++) {
                        XPE::RESOURCE_POSITION subpos = rh.listPositions.at(i).listPositions.at(j);
                        QStandardItem *pSubPos = new QStandardItem;
                        QString sSubPosText = XPE::resourceIdNameToString(pos.rin, 1);

                        pSubPos->setText(sSubPosText);

                        pPos->appendRow(pSubPos);

                        int nNumberOfSubPositions = subpos.listPositions.count();

                        for (qint32 k = 0; (k < nNumberOfSubPositions) && (isRun()); k++) {
                            XPE::RESOURCE_POSITION record1 = rh.listPositions.at(i);
                            XPE::RESOURCE_POSITION record2 = rh.listPositions.at(i).listPositions.at(j);
                            XPE::RESOURCE_POSITION record3 = rh.listPositions.at(i).listPositions.at(j).listPositions.at(k);
                            QStandardItem *pRecord = new QStandardItem;

                            QString sRecordText1 = XPE::resourceIdNameToString(record1.rin, -1);
                            QString sRecordText2 = XPE::resourceIdNameToString(record2.rin, -1);
                            QString sRecordText3 = XPE::resourceIdNameToString(record3.rin, -1);

                            pRecord->setText(sRecordText3);

                            pRecord->setData(sRecordText1, Qt::UserRole + FW_DEF::SECTION_DATA_VALUE1);
                            pRecord->setData(sRecordText2, Qt::UserRole + FW_DEF::SECTION_DATA_VALUE2);
                            pRecord->setData(sRecordText3, Qt::UserRole + FW_DEF::SECTION_DATA_VALUE3);
                            pRecord->setData(record3.dataEntry.Size, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE);
                            pRecord->setData(record3.dataEntry.OffsetToData, Qt::UserRole + FW_DEF::SECTION_DATA_ADDRESS);

                            if (g_pPE->isImage()) {
                                pRecord->setData(record3.dataEntry.OffsetToData, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET);
                            } else {
                                pRecord->setData(record3.nDataOffset, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET);
                            }

                            pSubPos->appendRow(pRecord);
                        }
                    }

                    incValue();
                }
            }
        } else {
            QList<QString> listLabels;
            listLabels.append("#");
            listLabels.append("");
            listLabels.append("");
            listLabels.append("");
            listLabels.append(tr("Address"));
            listLabels.append(tr("Offset"));
            listLabels.append(tr("Size"));
            listLabels.append("");

            QList<XPE::RESOURCE_RECORD> listResources = g_pPE->getResources();

            qint32 nNumberOfRecords = listResources.count();

            *g_ppModel = new QStandardItemModel(nNumberOfRecords, listLabels.count());

            setMaximum(nNumberOfRecords);

            setTableHeader(*g_ppModel, &listLabels);

            for (qint32 i = 0; (i < nNumberOfRecords) && (isRun()); i++) {
                QStandardItem *pItemNumber = new QStandardItem;
                pItemNumber->setData(i, Qt::DisplayRole);

                pItemNumber->setData(listResources.at(i).nAddress, Qt::UserRole + FW_DEF::SECTION_DATA_ADDRESS);
                pItemNumber->setData(listResources.at(i).nOffset, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET);
                pItemNumber->setData(listResources.at(i).nSize, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE);

                QString sResID1 = XPE::resourceIdNameToString(listResources.at(i).irin[0], 0);
                QString sResID2 = XPE::resourceIdNameToString(listResources.at(i).irin[1], 1);
                QString sResID3 = XPE::resourceIdNameToString(listResources.at(i).irin[2], 2);

                //                pItemNumber->setData(QString("%1_%2_%3.bin").arg(sResID1, sResID2, sResID3), Qt::UserRole + FW_DEF::SECTION_DATA_NAME);

                QString sResName = XBinary::convertFileNameSymbols(g_pPE->resourceRecordToString(listResources.at(i)));

                pItemNumber->setData(sResName, Qt::UserRole + FW_DEF::SECTION_DATA_NAME);

                (*g_ppModel)->setItem(i, 0, pItemNumber);

                //                (*g_ppModel)->setItem(i,1,pItemName);
                (*g_ppModel)->setItem(i, 1, new QStandardItem(sResID1));
                (*g_ppModel)->setItem(i, 2, new QStandardItem(sResID2));
                (*g_ppModel)->setItem(i, 3, new QStandardItem(sResID3));

                (*g_ppModel)->setItem(i, 4, new QStandardItem(XBinary::valueToHexEx(listResources.at(i).nAddress)));
                (*g_ppModel)->setItem(i, 5, new QStandardItem(XBinary::valueToHexEx(listResources.at(i).nOffset)));
                (*g_ppModel)->setItem(i, 6, new QStandardItem(XBinary::valueToHexEx(listResources.at(i).nSize)));

                SpecAbstract::SCAN_OPTIONS options = {};
                options.varInfo = listResources.at(i).irin[0].nID;
                options.initFilePart = XBinary::FILEPART_RESOURCE;

                SpecAbstract::SCAN_RESULT scanResult =
                    StaticScan::processSubdevice(g_pPE->getDevice(), listResources.at(i).nOffset, listResources.at(i).nSize, &options, getPdStruct());

                (*g_ppModel)->setItem(i, 7, new QStandardItem(SpecAbstract::createShortResultString(scanResult)));

                incValue();
            }
        }
    } else if (g_nType == SPE::TYPE_RESOURCES_STRINGTABLE) {
        QList<QString> listLabels;
        listLabels.append("#");
        listLabels.append(QString("Id"));
        listLabels.append(tr("Language"));
        listLabels.append(tr("String"));

        QList<XPE::RESOURCE_STRINGTABLE_RECORD> listSTR = g_pPE->getResourceStringTableRecords();

        int nNumberOfRecords = listSTR.count();

        *g_ppModel = new QStandardItemModel(nNumberOfRecords, listLabels.count());

        setMaximum(nNumberOfRecords);

        setTableHeader(*g_ppModel, &listLabels);

        for (qint32 i = 0; (i < nNumberOfRecords) && (isRun()); i++) {
            QStandardItem *pItem = new QStandardItem;
            pItem->setData(i, Qt::DisplayRole);

            pItem->setData(listSTR.at(i).nAddress, Qt::UserRole + FW_DEF::SECTION_DATA_ADDRESS);
            pItem->setData(listSTR.at(i).nOffset, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET);
            pItem->setData(listSTR.at(i).nSize, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE);

            (*g_ppModel)->setItem(i, 0, pItem);
            (*g_ppModel)->setItem(i, 1, new QStandardItem(QString::number(listSTR.at(i).nID)));
            (*g_ppModel)->setItem(i, 2, new QStandardItem(QString::number(listSTR.at(i).nLanguage)));
            (*g_ppModel)->setItem(i, 3, new QStandardItem(listSTR.at(i).sString));

            incValue();
        }
    } else if (g_nType == SPE::TYPE_CERTIFICATE) {
        //        g_pPE->getCertInfo();

        *g_ppModel = new QStandardItemModel;

        XPE_DEF::IMAGE_DATA_DIRECTORY ddSecurity = g_pPE->getOptionalHeader_DataDirectory(XPE_DEF::S_IMAGE_DIRECTORY_ENTRY_SECURITY);

        QList<XPE::CERT> listCert = g_pPE->getCertList(ddSecurity.VirtualAddress, ddSecurity.Size);

        if (listCert.count()) {
            int nNumberOfCerts = listCert.count();

            for (qint32 i = 0; (i < nNumberOfCerts) && (isRun()); i++) {
                QStandardItem *pRoot = new QStandardItem;

                pRoot->setText(tr("Certificate"));

                pRoot->setData(listCert.at(i).record.dwLength, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE);
                pRoot->setData(listCert.at(i).nOffset, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET);

                int nNumberOfRecords = listCert.at(i).certRecord.listRecords.count();

                for (qint32 j = 0; (j < nNumberOfRecords) && (isRun()); j++) {
                    QStandardItem *pRecord = new QStandardItem;

                    handleCertRecord(pRecord, listCert.at(i).certRecord.listRecords.at(j));

                    pRoot->appendRow(pRecord);
                }

                (*g_ppModel)->appendRow(pRoot);
            }
        }
    } else if (g_nType == SPE::TYPE_CERTIFICATE_CHECK) {
        *g_ppModel = new QStandardItemModel;

        QTemporaryDir tempDir;
        if (tempDir.isValid()) {
            QString sFileName = tempDir.path() + QDir::separator() + "CERT.DAT";

            if (g_pPE->dumpToFile(sFileName, (qint64)0, g_pPE->getSize())) {
                XPE::XCERT_INFO xsertInfo = XPE::getCertInfo(sFileName);

                (*g_ppModel)->appendRow(new QStandardItem(QString("%1: %2").arg(tr("Status"), xsertInfo.sStatus)));

                if (xsertInfo.sProgramName != "") (*g_ppModel)->appendRow(new QStandardItem(QString("%1: %2").arg(tr("Program name"), xsertInfo.sProgramName)));
                if (xsertInfo.sPublisher != "") (*g_ppModel)->appendRow(new QStandardItem(QString("%1: %2").arg(tr("Publisher"), xsertInfo.sPublisher)));
                if (xsertInfo.sMoreInfo != "") (*g_ppModel)->appendRow(new QStandardItem(QString("%1: %2").arg(tr("More info"), xsertInfo.sMoreInfo)));
                if (xsertInfo.sSerialNumber != "") (*g_ppModel)->appendRow(new QStandardItem(QString("%1: %2").arg(tr("Serial number"), xsertInfo.sSerialNumber)));
                if (xsertInfo.sIssuer != "") (*g_ppModel)->appendRow(new QStandardItem(QString("%1: %2").arg(tr("Issuer"), xsertInfo.sIssuer)));
                if (xsertInfo.sSubject != "") (*g_ppModel)->appendRow(new QStandardItem(QString("%1: %2").arg(tr("Subject"), xsertInfo.sSubject)));
                if (xsertInfo.sAlgorithm != "") (*g_ppModel)->appendRow(new QStandardItem(QString("%1: %2").arg(tr("Algorithm"), xsertInfo.sAlgorithm)));
                if (xsertInfo.sTSSerialNumber != "") (*g_ppModel)->appendRow(new QStandardItem(QString("TS %1: %2").arg(tr("Serial number"), xsertInfo.sTSSerialNumber)));
                if (xsertInfo.sTSIssuer != "") (*g_ppModel)->appendRow(new QStandardItem(QString("TS %1: %2").arg(tr("Issuer"), xsertInfo.sTSIssuer)));
                if (xsertInfo.sTSSubject != "") (*g_ppModel)->appendRow(new QStandardItem(QString("TS %1: %2").arg(tr("Subject"), xsertInfo.sTSSubject)));
            }
        }
    } else if (g_nType == SPE::TYPE_RICH) {
        QList<QString> listLabels;
        listLabels.append("#");
        listLabels.append("Id");
        listLabels.append(tr("Version"));
        listLabels.append(tr("Count"));
        listLabels.append("");

        QList<XMSDOS::MS_RICH_RECORD> listRichSignatures = g_pPE->getRichSignatureRecords();

        qint32 nNumberOfRecords = listRichSignatures.count();

        *g_ppModel = new QStandardItemModel(nNumberOfRecords, listLabels.count());

        setMaximum(nNumberOfRecords);

        setTableHeader(*g_ppModel, &listLabels);

        for (qint32 i = 0; (i < nNumberOfRecords) && (isRun()); i++) {
            QStandardItem *pItemNumber = new QStandardItem;
            pItemNumber->setData(i, Qt::DisplayRole);

            (*g_ppModel)->setItem(i, 0, pItemNumber);

            //                (*g_ppModel)->setItem(i,1,pItemName);
            (*g_ppModel)->setItem(i, 1, new QStandardItem(XBinary::valueToHex(listRichSignatures.at(i).nId)));
            (*g_ppModel)->setItem(i, 2, new QStandardItem(QString::number(listRichSignatures.at(i).nVersion)));
            (*g_ppModel)->setItem(i, 3, new QStandardItem(QString::number(listRichSignatures.at(i).nCount)));
            (*g_ppModel)
                ->setItem(i, 4,
                          new QStandardItem(SpecAbstract::getMsRichString(listRichSignatures.at(i).nId, listRichSignatures.at(i).nVersion,
                                                                          listRichSignatures.at(i).nCount, getPdStruct())));

            incValue();
        }
    }

    adjustModel(*g_ppModel);
}

void PEProcessData::ajustTableView(QWidget *pWidget, QTableView *pTableView)
{
    Q_UNUSED(pWidget)

    XBinary::MODE mode = g_pPE->getMode();

    if (g_nType == SPE::TYPE_SECTIONS) {
        XOptions::setTableViewHeaderWidth(pTableView, 0, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT16, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 1, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 2, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 3, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 4, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 5, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 6, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 7, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 8, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 9, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 10, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
    } else if (g_nType == SPE::TYPE_RELOCS) {
        XOptions::setTableViewHeaderWidth(pTableView, 0, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT16, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 1, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 2, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 3, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 4, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_STRINGMID, mode));
    } else if (g_nType == SPE::TYPE_RELOCS_POSITION) {
        XOptions::setTableViewHeaderWidth(pTableView, 0, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT16, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 1, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 2, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINTMODE, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 3, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINTMODE, mode));
    } else if (g_nType == SPE::TYPE_IMPORT) {
        XOptions::setTableViewHeaderWidth(pTableView, 0, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT16, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 1, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 2, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 3, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 4, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 5, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 6, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 7, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_STRINGMID, mode));
    } else if (g_nType == SPE::TYPE_IMPORT_FUNCTION) {
        XOptions::setTableViewHeaderWidth(pTableView, 0, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT16, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 1, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINTMODE, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 2, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINTMODE, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 3, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT16, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 4, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_STRINGLONG, mode));
    } else if (g_nType == SPE::TYPE_EXCEPTION) {
        XOptions::setTableViewHeaderWidth(pTableView, 0, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT16, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 1, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 2, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 3, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 4, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_STRINGMID, mode));
    } else if (g_nType == SPE::TYPE_DELAYIMPORT) {
        XOptions::setTableViewHeaderWidth(pTableView, 0, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT16, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 1, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 2, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 3, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 4, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 5, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 6, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 7, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 8, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 9, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_STRINGMID, mode));
    } else if (g_nType == SPE::TYPE_EXPORT_FUNCTION) {
        XOptions::setTableViewHeaderWidth(pTableView, 0, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT16, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 1, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 2, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 3, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_STRINGLONG, mode));
    } else if (g_nType == SPE::TYPE_BOUNDIMPORT) {
        XOptions::setTableViewHeaderWidth(pTableView, 0, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT16, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 1, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 2, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 3, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 4, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINTMODE, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 5, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_STRINGLONG, mode));
    } else if (g_nType == SPE::TYPE_DEBUG) {
        XOptions::setTableViewHeaderWidth(pTableView, 0, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT16, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 1, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 2, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 3, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 4, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 5, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 6, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 7, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 8, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
    } else if (g_nType == SPE::TYPE_DELAYIMPORT_FUNCTION) {
        XOptions::setTableViewHeaderWidth(pTableView, 0, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT16, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 1, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINTMODE, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 2, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINTMODE, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 3, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINTMODE, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 4, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINTMODE, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 5, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT16, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 6, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_STRINGMID, mode));
    } else if (g_nType == SPE::TYPE_TLSCALLBACKS) {
        XOptions::setTableViewHeaderWidth(pTableView, 0, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT16, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 1, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINTMODE, mode));
    } else if (g_nType == SPE::TYPE_RESOURCES) {
        XOptions::setTableViewHeaderWidth(pTableView, 0, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT16, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 1, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_STRINGSHORT, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 2, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_STRINGSHORT, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 3, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 4, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINTMODE, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 5, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 6, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
    } else if (g_nType == SPE::TYPE_RICH) {
        XOptions::setTableViewHeaderWidth(pTableView, 0, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 1, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 2, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 3, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
    }
}

void PEProcessData::ajustTreeView(QWidget *pWidget, QTreeView *pTreeView)
{
    Q_UNUSED(pWidget)
    XBinary::MODE mode = g_pPE->getMode();

    if (g_nType == SPE::TYPE_SECTIONS_INFO) {
        XOptions::setTreeViewHeaderWidth(pTreeView, 0, FormatWidget::getColumnWidth(pTreeView, FormatWidget::CW_UINT16, mode));
        XOptions::setTreeViewHeaderWidth(pTreeView, 1, FormatWidget::getColumnWidth(pTreeView, FormatWidget::CW_UINT32, mode));
        XOptions::setTreeViewHeaderWidth(pTreeView, 2, FormatWidget::getColumnWidth(pTreeView, FormatWidget::CW_UINT32, mode));
        XOptions::setTreeViewHeaderWidth(pTreeView, 3, FormatWidget::getColumnWidth(pTreeView, FormatWidget::CW_UINT32, mode));
        XOptions::setTreeViewHeaderWidth(pTreeView, 4, FormatWidget::getColumnWidth(pTreeView, FormatWidget::CW_UINT32, mode));
        XOptions::setTreeViewHeaderWidth(pTreeView, 5, FormatWidget::getColumnWidth(pTreeView, FormatWidget::CW_UINT32, mode));
        XOptions::setTreeViewHeaderWidth(pTreeView, 6, FormatWidget::getColumnWidth(pTreeView, FormatWidget::CW_UINT32, mode));
    }
}

void PEProcessData::handleCertRecord(QStandardItem *pParent, XPE::CERT_RECORD certRecord)
{
    QString sText = QString("%1 (%2)").arg(XBinary::valueToHex(XBinary::MODE_UNKNOWN, certRecord.certTag.nTag), XPE::certTagToString(certRecord.certTag.nTag));

    QString sValue = certRecord.varValue.toString();

    if (sValue.size()) {
        sText += QString(": %1").arg(sValue);

        if (certRecord.certTag.nTag == XPE_DEF::S_ASN1_OBJECT_ID) {
            QString sObject = XPE::objectIdToString(sValue);

            if (sObject != "") {
                sText += QString(": %1").arg(sObject);
            }
        }
    }

    pParent->setText(sText);

    pParent->setData(certRecord.certTag.nHeaderSize + certRecord.certTag.nSize, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE);
    pParent->setData(certRecord.certTag.nOffset, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET);

    int nNumberOfRecords = certRecord.listRecords.count();

    for (qint32 i = 0; (i < nNumberOfRecords) && (isRun()); i++) {
        QStandardItem *pRecord = new QStandardItem;

        handleCertRecord(pRecord, certRecord.listRecords.at(i));

        pParent->appendRow(pRecord);
    }
}

void PEProcessData::adjustModel(QStandardItemModel *pModel)
{
    if (g_nType == SPE::TYPE_SECTIONS) {
        XOptions::setModelTextAlignment(pModel, 0, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 1, Qt::AlignLeft | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 2, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 3, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 4, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 5, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 6, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 7, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 8, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 9, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 10, Qt::AlignRight | Qt::AlignVCenter);
    } else if (g_nType == SPE::TYPE_SECTIONS_INFO) {
        XOptions::setModelTextAlignment(pModel, 0, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 1, Qt::AlignLeft | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 2, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 3, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 4, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 5, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 6, Qt::AlignLeft | Qt::AlignVCenter);
    } else if (g_nType == SPE::TYPE_RELOCS) {
        XOptions::setModelTextAlignment(pModel, 0, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 1, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 2, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 3, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 4, Qt::AlignLeft | Qt::AlignVCenter);
    } else if (g_nType == SPE::TYPE_RELOCS_POSITION) {
        XOptions::setModelTextAlignment(pModel, 0, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 1, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 2, Qt::AlignLeft | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 3, Qt::AlignRight | Qt::AlignVCenter);
    } else if (g_nType == SPE::TYPE_IMPORT) {
        XOptions::setModelTextAlignment(pModel, 0, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 1, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 2, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 3, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 4, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 5, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 6, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 7, Qt::AlignLeft | Qt::AlignVCenter);
    } else if (g_nType == SPE::TYPE_IMPORT_FUNCTION) {
        XOptions::setModelTextAlignment(pModel, 0, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 1, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 2, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 3, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 4, Qt::AlignLeft | Qt::AlignVCenter);
    } else if (g_nType == SPE::TYPE_EXCEPTION) {
        XOptions::setModelTextAlignment(pModel, 0, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 1, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 2, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 3, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 4, Qt::AlignLeft | Qt::AlignVCenter);
    } else if (g_nType == SPE::TYPE_DELAYIMPORT) {
        XOptions::setModelTextAlignment(pModel, 0, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 1, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 2, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 3, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 4, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 5, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 6, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 7, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 8, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 9, Qt::AlignLeft | Qt::AlignVCenter);
    } else if (g_nType == SPE::TYPE_EXPORT_FUNCTION) {
        XOptions::setModelTextAlignment(pModel, 0, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 1, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 2, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 3, Qt::AlignLeft | Qt::AlignVCenter);
    } else if (g_nType == SPE::TYPE_BOUNDIMPORT) {
        XOptions::setModelTextAlignment(pModel, 0, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 1, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 2, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 3, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 4, Qt::AlignLeft | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 5, Qt::AlignLeft | Qt::AlignVCenter);
    } else if (g_nType == SPE::TYPE_DEBUG) {
        XOptions::setModelTextAlignment(pModel, 0, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 1, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 2, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 3, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 4, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 5, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 6, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 7, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 8, Qt::AlignRight | Qt::AlignVCenter);
    } else if (g_nType == SPE::TYPE_DELAYIMPORT_FUNCTION) {
        XOptions::setModelTextAlignment(pModel, 0, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 1, Qt::AlignLeft | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 2, Qt::AlignLeft | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 3, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 4, Qt::AlignLeft | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 5, Qt::AlignLeft | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 6, Qt::AlignLeft | Qt::AlignVCenter);
    } else if (g_nType == SPE::TYPE_TLSCALLBACKS) {
        XOptions::setModelTextAlignment(pModel, 0, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 1, Qt::AlignRight | Qt::AlignVCenter);
    } else if (g_nType == SPE::TYPE_RESOURCES) {
        if (!g_varInfo.toBool()) {
            XOptions::setModelTextAlignment(pModel, 0, Qt::AlignRight | Qt::AlignVCenter);
            XOptions::setModelTextAlignment(pModel, 1, Qt::AlignLeft | Qt::AlignVCenter);
            XOptions::setModelTextAlignment(pModel, 2, Qt::AlignLeft | Qt::AlignVCenter);
            XOptions::setModelTextAlignment(pModel, 3, Qt::AlignLeft | Qt::AlignVCenter);
            XOptions::setModelTextAlignment(pModel, 4, Qt::AlignRight | Qt::AlignVCenter);
            XOptions::setModelTextAlignment(pModel, 5, Qt::AlignRight | Qt::AlignVCenter);
            XOptions::setModelTextAlignment(pModel, 6, Qt::AlignRight | Qt::AlignVCenter);
        }
    } else if (g_nType == SPE::TYPE_RICH) {
        XOptions::setModelTextAlignment(pModel, 0, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 1, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 2, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 3, Qt::AlignRight | Qt::AlignVCenter);
    }
}

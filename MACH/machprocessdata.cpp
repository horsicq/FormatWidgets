/* Copyright (c) 2017-2022 hors<horsicq@gmail.com>
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
#include "machprocessdata.h"

MACHProcessData::MACHProcessData(int nType, QStandardItemModel **ppModel, XMACH *pXMACH, qint64 nOffset, qint64 nSize) : ProcessData()
{
    this->g_nType = nType;
    this->g_ppModel = ppModel;
    this->g_pXMACH = pXMACH;
    this->g_nOffset = nOffset;
    this->g_nSize = nSize;
}

void MACHProcessData::_process()
{
    bool bIs64 = g_pXMACH->is64();

    if (g_nType == SMACH::TYPE_mach_commands) {
        QList<QString> listLabels;
        listLabels.append("");
        listLabels.append(getStructList(N_mach_commands::records, N_mach_commands::__data_size));
        listLabels.append("");

        QList<XMACH::COMMAND_RECORD> listCommandRecords = g_pXMACH->getCommandRecords();
        QMap<quint64, QString> mapLC = g_pXMACH->getLoadCommandTypesS();

        qint32 nNumberOfRecords = listCommandRecords.count();

        *g_ppModel = new QStandardItemModel(nNumberOfRecords, listLabels.count());

        setMaximum(nNumberOfRecords);

        setHeader(*g_ppModel, &listLabels);

        for (qint32 i = 0; (i < nNumberOfRecords) && (isRun()); i++) {
            QStandardItem *pItem = new QStandardItem;
            pItem->setData(i, Qt::DisplayRole);

            pItem->setData(listCommandRecords.at(i).nStructOffset, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET);
            pItem->setData(listCommandRecords.at(i).nStructOffset, Qt::UserRole + FW_DEF::SECTION_DATA_HEADEROFFSET);
            pItem->setData(listCommandRecords.at(i).nSize, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE);
            pItem->setData(listCommandRecords.at(i).nStructOffset, Qt::UserRole + FW_DEF::SECTION_DATA_ADDRESS);

            (*g_ppModel)->setItem(i, 0, pItem);
            (*g_ppModel)->setItem(i, N_mach_commands::cmd + 1, new QStandardItem(XBinary::valueToHex((quint32)listCommandRecords.at(i).nType)));
            (*g_ppModel)->setItem(i, N_mach_commands::cmdsize + 1, new QStandardItem(XBinary::valueToHex((quint32)listCommandRecords.at(i).nSize)));
            (*g_ppModel)->setItem(i, N_mach_commands::cmdsize + 2, new QStandardItem(mapLC.value(listCommandRecords.at(i).nType)));

            incValue();
        }
    } else if ((g_nType == SMACH::TYPE_mach_libraries) || (g_nType == SMACH::TYPE_mach_weak_libraries) || (g_nType == SMACH::TYPE_mach_id_library)) {
        QList<QString> listLabels;
        listLabels.append("");
        listLabels.append(getStructList(N_mach_library::records, N_mach_library::__data_size));

        qint32 nType = XMACH_DEF::S_LC_LOAD_DYLIB;

        if (g_nType == SMACH::TYPE_mach_weak_libraries) {
            nType = XMACH_DEF::S_LC_LOAD_WEAK_DYLIB;
        } else if (g_nType == SMACH::TYPE_mach_id_library) {
            nType = XMACH_DEF::S_LC_ID_DYLIB;
        }

        QList<XMACH::LIBRARY_RECORD> listLibraries = g_pXMACH->getLibraryRecords(nType);

        qint32 nNumberOfRecords = listLibraries.count();

        *g_ppModel = new QStandardItemModel(nNumberOfRecords, listLabels.count());

        setMaximum(nNumberOfRecords);

        setHeader(*g_ppModel, &listLabels);

        for (qint32 i = 0; (i < nNumberOfRecords) && (isRun()); i++) {
            QStandardItem *pItem = new QStandardItem;
            pItem->setData(i, Qt::DisplayRole);

            pItem->setData(listLibraries.at(i).nStructOffset, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET);
            pItem->setData(listLibraries.at(i).nStructOffset, Qt::UserRole + FW_DEF::SECTION_DATA_HEADEROFFSET);
            pItem->setData(listLibraries.at(i).nStructSize, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE);
            pItem->setData(listLibraries.at(i).nStructOffset, Qt::UserRole + FW_DEF::SECTION_DATA_ADDRESS);

            (*g_ppModel)->setItem(i, 0, pItem);
            (*g_ppModel)->setItem(i, N_mach_library::timestamp + 1, new QStandardItem(XBinary::valueToHex(listLibraries.at(i).timestamp)));
            (*g_ppModel)->setItem(i, N_mach_library::current_version + 1, new QStandardItem(XBinary::valueToHex(listLibraries.at(i).current_version)));
            (*g_ppModel)->setItem(i, N_mach_library::compatibility_version + 1, new QStandardItem(XBinary::valueToHex(listLibraries.at(i).compatibility_version)));
            (*g_ppModel)->setItem(i, N_mach_library::name + 1, new QStandardItem(listLibraries.at(i).sFullName));

            incValue();
        }
    } else if ((g_nType == SMACH::TYPE_mach_IDFVMLIB) || (g_nType == SMACH::TYPE_mach_LOADFVMLIB)) {
        QList<QString> listLabels;
        listLabels.append("");
        listLabels.append(getStructList(N_mach_fmv_library::records, N_mach_fmv_library::__data_size));

        int nType = XMACH_DEF::S_LC_LOADFVMLIB;

        if (g_nType == SMACH::TYPE_mach_IDFVMLIB) {
            nType = XMACH_DEF::S_LC_IDFVMLIB;
        }

        QList<XMACH::FVM_LIBRARY_RECORD> listLibraries = g_pXMACH->getFvmLibraryRecords(nType);

        qint32 nNumberOfRecords = listLibraries.count();

        *g_ppModel = new QStandardItemModel(nNumberOfRecords, listLabels.count());

        setMaximum(nNumberOfRecords);

        setHeader(*g_ppModel, &listLabels);

        for (qint32 i = 0; (i < nNumberOfRecords) && (isRun()); i++) {
            QStandardItem *pItem = new QStandardItem;
            pItem->setData(i, Qt::DisplayRole);

            pItem->setData(listLibraries.at(i).nStructOffset, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET);
            pItem->setData(listLibraries.at(i).nStructOffset, Qt::UserRole + FW_DEF::SECTION_DATA_HEADEROFFSET);
            pItem->setData(listLibraries.at(i).nStructSize, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE);
            pItem->setData(listLibraries.at(i).nStructOffset, Qt::UserRole + FW_DEF::SECTION_DATA_ADDRESS);

            (*g_ppModel)->setItem(i, 0, pItem);
            (*g_ppModel)->setItem(i, N_mach_fmv_library::minor_version + 1, new QStandardItem(XBinary::valueToHex(listLibraries.at(i).minor_version)));
            (*g_ppModel)->setItem(i, N_mach_fmv_library::header_addr + 1, new QStandardItem(XBinary::valueToHex(listLibraries.at(i).header_addr)));
            (*g_ppModel)->setItem(i, N_mach_fmv_library::name + 1, new QStandardItem(listLibraries.at(i).sFullName));

            incValue();
        }
    } else if (g_nType == SMACH::TYPE_mach_segments) {
        QList<QString> listLabels;
        listLabels.append("");

        if (bIs64) {
            listLabels.append(getStructList(N_mach_segments::records64, N_mach_segments::__data_size));
        } else {
            listLabels.append(getStructList(N_mach_segments::records32, N_mach_segments::__data_size));
        }

        QList<XMACH::SEGMENT_RECORD> listSegmentRecords = g_pXMACH->getSegmentRecords();

        qint32 nNumberOfRecords = listSegmentRecords.count();

        *g_ppModel = new QStandardItemModel(nNumberOfRecords, listLabels.count());

        setMaximum(nNumberOfRecords);

        setHeader(*g_ppModel, &listLabels);

        for (qint32 i = 0; (i < nNumberOfRecords) && (isRun()); i++) {
            QStandardItem *pItem = new QStandardItem;
            pItem->setData(i, Qt::DisplayRole);
            pItem->setData(listSegmentRecords.at(i).nStructOffset, Qt::UserRole + FW_DEF::SECTION_DATA_HEADEROFFSET);

            if (bIs64) {
                if (g_pXMACH->isImage()) {
                    pItem->setData(listSegmentRecords.at(i).s.segment64.vmaddr, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET);
                    pItem->setData(listSegmentRecords.at(i).s.segment64.vmsize, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE);
                } else {
                    pItem->setData(listSegmentRecords.at(i).s.segment64.fileoff, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET);
                    pItem->setData(listSegmentRecords.at(i).s.segment64.filesize, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE);
                }

                pItem->setData(listSegmentRecords.at(i).s.segment64.vmaddr, Qt::UserRole + FW_DEF::SECTION_DATA_ADDRESS);

                (*g_ppModel)->setItem(i, 0, pItem);
                (*g_ppModel)->setItem(i, N_mach_segments::segname + 1, new QStandardItem(listSegmentRecords.at(i).s.segment64.segname));

                (*g_ppModel)->setItem(i, N_mach_segments::vmaddr + 1, new QStandardItem(XBinary::valueToHex(listSegmentRecords.at(i).s.segment64.vmaddr)));
                (*g_ppModel)->setItem(i, N_mach_segments::vmsize + 1, new QStandardItem(XBinary::valueToHex(listSegmentRecords.at(i).s.segment64.vmsize)));
                (*g_ppModel)->setItem(i, N_mach_segments::fileoff + 1, new QStandardItem(XBinary::valueToHex(listSegmentRecords.at(i).s.segment64.fileoff)));
                (*g_ppModel)->setItem(i, N_mach_segments::filesize + 1, new QStandardItem(XBinary::valueToHex(listSegmentRecords.at(i).s.segment64.filesize)));
                (*g_ppModel)->setItem(i, N_mach_segments::maxprot + 1, new QStandardItem(XBinary::valueToHex(listSegmentRecords.at(i).s.segment64.maxprot)));
                (*g_ppModel)->setItem(i, N_mach_segments::initprot + 1, new QStandardItem(XBinary::valueToHex(listSegmentRecords.at(i).s.segment64.initprot)));
                (*g_ppModel)->setItem(i, N_mach_segments::nsects + 1, new QStandardItem(XBinary::valueToHex(listSegmentRecords.at(i).s.segment64.nsects)));
                (*g_ppModel)->setItem(i, N_mach_segments::flags + 1, new QStandardItem(XBinary::valueToHex(listSegmentRecords.at(i).s.segment64.flags)));
            } else {
                if (g_pXMACH->isImage()) {
                    pItem->setData(listSegmentRecords.at(i).s.segment32.vmaddr, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET);
                    pItem->setData(listSegmentRecords.at(i).s.segment32.vmsize, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE);
                } else {
                    pItem->setData(listSegmentRecords.at(i).s.segment32.fileoff, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET);
                    pItem->setData(listSegmentRecords.at(i).s.segment32.filesize, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE);
                }

                pItem->setData(listSegmentRecords.at(i).s.segment32.vmaddr, Qt::UserRole + FW_DEF::SECTION_DATA_ADDRESS);

                (*g_ppModel)->setItem(i, 0, pItem);
                (*g_ppModel)->setItem(i, N_mach_segments::segname + 1, new QStandardItem(listSegmentRecords.at(i).s.segment32.segname));

                (*g_ppModel)->setItem(i, N_mach_segments::vmaddr + 1, new QStandardItem(XBinary::valueToHex(listSegmentRecords.at(i).s.segment32.vmaddr)));
                (*g_ppModel)->setItem(i, N_mach_segments::vmsize + 1, new QStandardItem(XBinary::valueToHex(listSegmentRecords.at(i).s.segment32.vmsize)));
                (*g_ppModel)->setItem(i, N_mach_segments::fileoff + 1, new QStandardItem(XBinary::valueToHex(listSegmentRecords.at(i).s.segment32.fileoff)));
                (*g_ppModel)->setItem(i, N_mach_segments::filesize + 1, new QStandardItem(XBinary::valueToHex(listSegmentRecords.at(i).s.segment32.filesize)));
                (*g_ppModel)->setItem(i, N_mach_segments::maxprot + 1, new QStandardItem(XBinary::valueToHex(listSegmentRecords.at(i).s.segment32.maxprot)));
                (*g_ppModel)->setItem(i, N_mach_segments::initprot + 1, new QStandardItem(XBinary::valueToHex(listSegmentRecords.at(i).s.segment32.initprot)));
                (*g_ppModel)->setItem(i, N_mach_segments::nsects + 1, new QStandardItem(XBinary::valueToHex(listSegmentRecords.at(i).s.segment32.nsects)));
                (*g_ppModel)->setItem(i, N_mach_segments::flags + 1, new QStandardItem(XBinary::valueToHex(listSegmentRecords.at(i).s.segment32.flags)));
            }

            incValue();
        }
    } else if (g_nType == SMACH::TYPE_mach_sections) {
        QList<QString> listLabels;
        listLabels.append("");

        if (bIs64) {
            listLabels.append(getStructList(N_mach_sections64::records, N_mach_sections64::__data_size));
        } else {
            listLabels.append(getStructList(N_mach_sections32::records, N_mach_sections32::__data_size));
        }

        QList<XMACH::SECTION_RECORD> listSectionRecords = g_pXMACH->getSectionRecords();

        qint32 nNumberOfRecords = listSectionRecords.count();

        *g_ppModel = new QStandardItemModel(nNumberOfRecords, listLabels.count());

        setMaximum(nNumberOfRecords);

        setHeader(*g_ppModel, &listLabels);

        for (qint32 i = 0; (i < nNumberOfRecords) && (isRun()); i++) {
            QStandardItem *pItem = new QStandardItem;
            pItem->setData(i, Qt::DisplayRole);
            pItem->setData(listSectionRecords.at(i).nStructOffset, Qt::UserRole + FW_DEF::SECTION_DATA_HEADEROFFSET);

            if (bIs64) {
                if (g_pXMACH->isImage()) {
                    pItem->setData(listSectionRecords.at(i).s.section64.addr, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET);
                } else {
                    pItem->setData(listSectionRecords.at(i).s.section64.offset, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET);
                }

                pItem->setData(listSectionRecords.at(i).s.section64.size, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE);
                pItem->setData(listSectionRecords.at(i).s.section64.addr, Qt::UserRole + FW_DEF::SECTION_DATA_ADDRESS);

                (*g_ppModel)->setItem(i, 0, pItem);

                (*g_ppModel)->setItem(i, N_mach_sections64::segname + 1, new QStandardItem(listSectionRecords.at(i).s.section64.segname));
                (*g_ppModel)->setItem(i, N_mach_sections64::sectname + 1, new QStandardItem(listSectionRecords.at(i).s.section64.sectname));
                (*g_ppModel)->setItem(i, N_mach_sections64::addr + 1, new QStandardItem(XBinary::valueToHex(listSectionRecords.at(i).s.section64.addr)));
                (*g_ppModel)->setItem(i, N_mach_sections64::size + 1, new QStandardItem(XBinary::valueToHex(listSectionRecords.at(i).s.section64.size)));
                (*g_ppModel)->setItem(i, N_mach_sections64::offset + 1, new QStandardItem(XBinary::valueToHex(listSectionRecords.at(i).s.section64.offset)));
                (*g_ppModel)->setItem(i, N_mach_sections64::align + 1, new QStandardItem(XBinary::valueToHex(listSectionRecords.at(i).s.section64.align)));
                (*g_ppModel)->setItem(i, N_mach_sections64::reloff + 1, new QStandardItem(XBinary::valueToHex(listSectionRecords.at(i).s.section64.reloff)));
                (*g_ppModel)->setItem(i, N_mach_sections64::nreloc + 1, new QStandardItem(XBinary::valueToHex(listSectionRecords.at(i).s.section64.nreloc)));
                (*g_ppModel)->setItem(i, N_mach_sections64::flags + 1, new QStandardItem(XBinary::valueToHex(listSectionRecords.at(i).s.section64.flags)));
                (*g_ppModel)->setItem(i, N_mach_sections64::reserved1 + 1, new QStandardItem(XBinary::valueToHex(listSectionRecords.at(i).s.section64.reserved1)));
                (*g_ppModel)->setItem(i, N_mach_sections64::reserved2 + 1, new QStandardItem(XBinary::valueToHex(listSectionRecords.at(i).s.section64.reserved2)));
                (*g_ppModel)->setItem(i, N_mach_sections64::reserved3 + 1, new QStandardItem(XBinary::valueToHex(listSectionRecords.at(i).s.section64.reserved3)));
            } else {
                if (g_pXMACH->isImage()) {
                    pItem->setData(listSectionRecords.at(i).s.section32.addr, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET);
                } else {
                    pItem->setData(listSectionRecords.at(i).s.section32.offset, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET);
                }

                pItem->setData(listSectionRecords.at(i).s.section32.size, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE);
                pItem->setData(listSectionRecords.at(i).s.section32.addr, Qt::UserRole + FW_DEF::SECTION_DATA_ADDRESS);

                (*g_ppModel)->setItem(i, 0, pItem);

                (*g_ppModel)->setItem(i, N_mach_sections32::segname + 1, new QStandardItem(listSectionRecords.at(i).s.section32.segname));
                (*g_ppModel)->setItem(i, N_mach_sections32::sectname + 1, new QStandardItem(listSectionRecords.at(i).s.section32.sectname));
                (*g_ppModel)->setItem(i, N_mach_sections32::addr + 1, new QStandardItem(XBinary::valueToHex(listSectionRecords.at(i).s.section32.addr)));
                (*g_ppModel)->setItem(i, N_mach_sections32::size + 1, new QStandardItem(XBinary::valueToHex(listSectionRecords.at(i).s.section32.size)));
                (*g_ppModel)->setItem(i, N_mach_sections32::offset + 1, new QStandardItem(XBinary::valueToHex(listSectionRecords.at(i).s.section32.offset)));
                (*g_ppModel)->setItem(i, N_mach_sections32::align + 1, new QStandardItem(XBinary::valueToHex(listSectionRecords.at(i).s.section32.align)));
                (*g_ppModel)->setItem(i, N_mach_sections32::reloff + 1, new QStandardItem(XBinary::valueToHex(listSectionRecords.at(i).s.section32.reloff)));
                (*g_ppModel)->setItem(i, N_mach_sections32::nreloc + 1, new QStandardItem(XBinary::valueToHex(listSectionRecords.at(i).s.section32.nreloc)));
                (*g_ppModel)->setItem(i, N_mach_sections32::flags + 1, new QStandardItem(XBinary::valueToHex(listSectionRecords.at(i).s.section32.flags)));
                (*g_ppModel)->setItem(i, N_mach_sections32::reserved1 + 1, new QStandardItem(XBinary::valueToHex(listSectionRecords.at(i).s.section32.reserved1)));
                (*g_ppModel)->setItem(i, N_mach_sections32::reserved2 + 1, new QStandardItem(XBinary::valueToHex(listSectionRecords.at(i).s.section32.reserved2)));
            }

            incValue();
        }
    } else if (g_nType == SMACH::TYPE_SYMBOLTABLE) {
        QList<QString> listLabels;
        listLabels.append("");

        if (bIs64) {
            listLabels.append(getStructList(N_mach_nlist::records64, N_mach_nlist::__data_size));
        } else {
            listLabels.append(getStructList(N_mach_nlist::records32, N_mach_nlist::__data_size));
        }

        listLabels.append(tr("Name"));

        XBinary::OFFSETSIZE osStringTable = g_pXMACH->getStringTableOffsetSize();

        QList<XMACH::NLIST_RECORD> listRecords = g_pXMACH->getNlistRecords();

        qint32 nNumberOfRecords = listRecords.count();

        *g_ppModel = new QStandardItemModel(nNumberOfRecords, listLabels.count());

        setMaximum(nNumberOfRecords);

        setHeader(*g_ppModel, &listLabels);

        for (qint32 i = 0; (i < nNumberOfRecords) && (isRun()); i++) {
            QStandardItem *pItem = new QStandardItem;
            pItem->setData(i, Qt::DisplayRole);
            pItem->setData(listRecords.at(i).nStructOffset, Qt::UserRole + FW_DEF::SECTION_DATA_HEADEROFFSET);

            (*g_ppModel)->setItem(i, 0, pItem);

            if (listRecords.at(i).bIs64) {
                (*g_ppModel)->setItem(i, N_mach_nlist::n_strx + 1, new QStandardItem(XBinary::valueToHex(listRecords.at(i).s.nlist64.n_strx)));
                (*g_ppModel)->setItem(i, N_mach_nlist::n_type + 1, new QStandardItem(XBinary::valueToHex(listRecords.at(i).s.nlist64.n_type)));
                (*g_ppModel)->setItem(i, N_mach_nlist::n_sect + 1, new QStandardItem(XBinary::valueToHex(listRecords.at(i).s.nlist64.n_sect)));
                (*g_ppModel)->setItem(i, N_mach_nlist::n_desc + 1, new QStandardItem(XBinary::valueToHex(listRecords.at(i).s.nlist64.n_desc)));
                (*g_ppModel)->setItem(i, N_mach_nlist::n_value + 1, new QStandardItem(XBinary::valueToHex(listRecords.at(i).s.nlist64.n_value)));

                QString sName = g_pXMACH->getStringFromIndex(osStringTable.nOffset, osStringTable.nSize, listRecords.at(i).s.nlist64.n_strx);

                (*g_ppModel)->setItem(i, N_mach_nlist::n_value + 2, new QStandardItem(sName));
            } else {
                (*g_ppModel)->setItem(i, N_mach_nlist::n_strx + 1, new QStandardItem(XBinary::valueToHex(listRecords.at(i).s.nlist32.n_strx)));
                (*g_ppModel)->setItem(i, N_mach_nlist::n_type + 1, new QStandardItem(XBinary::valueToHex(listRecords.at(i).s.nlist32.n_type)));
                (*g_ppModel)->setItem(i, N_mach_nlist::n_sect + 1, new QStandardItem(XBinary::valueToHex(listRecords.at(i).s.nlist32.n_sect)));
                (*g_ppModel)->setItem(i, N_mach_nlist::n_desc + 1, new QStandardItem(XBinary::valueToHex(listRecords.at(i).s.nlist32.n_desc)));
                (*g_ppModel)->setItem(i, N_mach_nlist::n_value + 1, new QStandardItem(XBinary::valueToHex(listRecords.at(i).s.nlist32.n_value)));

                QString sName = g_pXMACH->getStringFromIndex(osStringTable.nOffset, osStringTable.nSize, listRecords.at(i).s.nlist32.n_strx);

                (*g_ppModel)->setItem(i, N_mach_nlist::n_value + 2, new QStandardItem(sName));
            }

            incValue();
        }
    } else if (g_nType == SMACH::TYPE_FUNCTIONS) {
        QList<QString> listLabels;
        listLabels.append("");
        listLabels.append(tr("Offset"));
        listLabels.append(tr("Address"));
        listLabels.append(tr("Name"));

        QList<XMACH::FUNCTION_RECORD> listRecords = g_pXMACH->getFunctionRecords(g_nOffset, g_nSize);
        QList<XMACH::NLIST_RECORD> listNlistRecords = g_pXMACH->getNlistRecords();
        XBinary::OFFSETSIZE osStringTable = g_pXMACH->getStringTableOffsetSize();

        qint32 nNumberOfRecords = listRecords.count();

        *g_ppModel = new QStandardItemModel(nNumberOfRecords, listLabels.count());

        setMaximum(nNumberOfRecords);

        setHeader(*g_ppModel, &listLabels);

        for (qint32 i = 0; (i < nNumberOfRecords) && (isRun()); i++) {
            QStandardItem *pItem = new QStandardItem;
            pItem->setData(i, Qt::DisplayRole);

            pItem->setData(listRecords.at(i).nDataOffset, Qt::UserRole + FW_DEF::SECTION_DATA_HEADEROFFSET);
            pItem->setData(listRecords.at(i).nFunctionOffset, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET);
            pItem->setData(0, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE);
            pItem->setData(listRecords.at(i).nFunctionAddress, Qt::UserRole + FW_DEF::SECTION_DATA_ADDRESS);

            (*g_ppModel)->setItem(i, 0, pItem);

            XMACH::NLIST_RECORD nlist = XMACH::searchNlistRecordByValue(&listNlistRecords, listRecords.at(i).nFunctionAddress, true);

            if (bIs64) {
                QString sName = g_pXMACH->getStringFromIndex(osStringTable.nOffset, osStringTable.nSize, nlist.s.nlist64.n_strx);

                (*g_ppModel)->setItem(i, 1, new QStandardItem(XBinary::valueToHex((quint64)listRecords.at(i).nFunctionOffset)));
                (*g_ppModel)->setItem(i, 2, new QStandardItem(XBinary::valueToHex((quint64)listRecords.at(i).nFunctionAddress)));
                (*g_ppModel)->setItem(i, 3, new QStandardItem(sName));
            } else {
                QString sName = g_pXMACH->getStringFromIndex(osStringTable.nOffset, osStringTable.nSize, nlist.s.nlist32.n_strx);

                (*g_ppModel)->setItem(i, 1, new QStandardItem(XBinary::valueToHex((quint32)listRecords.at(i).nFunctionOffset)));
                (*g_ppModel)->setItem(i, 2, new QStandardItem(XBinary::valueToHex((quint32)listRecords.at(i).nFunctionAddress)));
                (*g_ppModel)->setItem(i, 3, new QStandardItem(sName));
            }

            incValue();
        }
    } else if (g_nType == SMACH::TYPE_DICE) {
        QList<QString> listLabels;
        listLabels.append("");
        listLabels.append(getStructList(N_mach_data_in_code_entry::records, N_mach_data_in_code_entry::__data_size));

        QList<XMACH::DICE_RECORD> listRecords = g_pXMACH->getDiceRecords(g_nOffset, g_nSize);

        int nNumberOfRecords = listRecords.count();

        *g_ppModel = new QStandardItemModel(nNumberOfRecords, listLabels.count());

        setMaximum(nNumberOfRecords);

        setHeader(*g_ppModel, &listLabels);

        for (qint32 i = 0; (i < nNumberOfRecords) && (isRun()); i++) {
            QStandardItem *pItem = new QStandardItem;
            pItem->setData(i, Qt::DisplayRole);
            pItem->setData(listRecords.at(i).nStructOffset, Qt::UserRole + FW_DEF::SECTION_DATA_HEADEROFFSET);
            pItem->setData(listRecords.at(i).dice.offset, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET);
            pItem->setData(listRecords.at(i).dice.length, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE);
            (*g_ppModel)->setItem(i, 0, pItem);
            (*g_ppModel)->setItem(i, N_mach_data_in_code_entry::offset + 1, new QStandardItem(XBinary::valueToHex(listRecords.at(i).dice.offset)));
            (*g_ppModel)->setItem(i, N_mach_data_in_code_entry::length + 1, new QStandardItem(XBinary::valueToHex(listRecords.at(i).dice.length)));
            (*g_ppModel)->setItem(i, N_mach_data_in_code_entry::kind + 1, new QStandardItem(XBinary::valueToHex(listRecords.at(i).dice.kind)));

            incValue();
        }
    } else if ((g_nType == SMACH::TYPE_DYLD_INFO_rebase) || (g_nType == SMACH::TYPE_DYLD_INFO_bind) || (g_nType == SMACH::TYPE_DYLD_INFO_export) ||
               (g_nType == SMACH::TYPE_DYLD_INFO_lazy_bind) || (g_nType == SMACH::TYPE_DYLD_INFO_weak_bind)) {
        QList<QString> listLabels;
        listLabels.append("");
        listLabels.append(tr("Offset"));
        listLabels.append(tr("Opcode"));

        XMACH::OPCODE_TYPE opcodeType = XMACH::OPCODE_TYPE_UNKNOWN;

        if (g_nType == SMACH::TYPE_DYLD_INFO_rebase)
            opcodeType = XMACH::OPCODE_TYPE_REBASE;
        else if (g_nType == SMACH::TYPE_DYLD_INFO_bind)
            opcodeType = XMACH::OPCODE_TYPE_BIND;
        else if (g_nType == SMACH::TYPE_DYLD_INFO_export)
            opcodeType = XMACH::OPCODE_TYPE_EXPORT;
        else if (g_nType == SMACH::TYPE_DYLD_INFO_lazy_bind)
            opcodeType = XMACH::OPCODE_TYPE_LAZY_BIND;
        else if (g_nType == SMACH::TYPE_DYLD_INFO_weak_bind)
            opcodeType = XMACH::OPCODE_TYPE_WEAK_BIND;

        QList<XBinary::OPCODE> listRecords = g_pXMACH->getOpcodes(g_nOffset, g_nOffset, g_nSize, opcodeType);

        int nNumberOfRecords = listRecords.count();

        *g_ppModel = new QStandardItemModel(nNumberOfRecords, listLabels.count());

        setMaximum(nNumberOfRecords);

        setHeader(*g_ppModel, &listLabels);

        for (qint32 i = 0; (i < nNumberOfRecords) && (isRun()); i++) {
            QStandardItem *pItem = new QStandardItem;
            pItem->setData(i, Qt::DisplayRole);
            pItem->setData(listRecords.at(i).nAddress - g_nOffset, Qt::UserRole + FW_DEF::SECTION_DATA_HEADEROFFSET);
            pItem->setData(listRecords.at(i).nAddress - g_nOffset, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET);
            pItem->setData(listRecords.at(i).nSize, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE);
            (*g_ppModel)->setItem(i, 0, pItem);
            (*g_ppModel)->setItem(i, 1, new QStandardItem(XBinary::valueToHex((qint32)listRecords.at(i).nAddress)));
            (*g_ppModel)->setItem(i, 2, new QStandardItem(listRecords.at(i).sName));

            incValue();
        }
    } else if (g_nType == SMACH::TYPE_DYSYMTAB_toc) {
        QList<QString> listLabels;
        listLabels.append("");
        listLabels.append(getStructList(N_mach_table_of_contents::records, N_mach_table_of_contents::__data_size));
        listLabels.append(tr("Name"));

        QList<XMACH::TOC_RECORD> listRecords = g_pXMACH->get_toc_list();
        QList<XMACH::NLIST_RECORD> listNlistRecords = g_pXMACH->getNlistRecords();
        XBinary::OFFSETSIZE osStringTable = g_pXMACH->getStringTableOffsetSize();

        int nNumberOfRecords = listRecords.count();

        *g_ppModel = new QStandardItemModel(nNumberOfRecords, listLabels.count());

        setMaximum(nNumberOfRecords);

        setHeader(*g_ppModel, &listLabels);

        for (qint32 i = 0; (i < nNumberOfRecords) && (isRun()); i++) {
            QStandardItem *pItem = new QStandardItem;
            pItem->setData(i, Qt::DisplayRole);
            pItem->setData(listRecords.at(i).nStructOffset, Qt::UserRole + FW_DEF::SECTION_DATA_HEADEROFFSET);
            (*g_ppModel)->setItem(i, 0, pItem);
            (*g_ppModel)->setItem(i, N_mach_table_of_contents::symbol_index + 1, new QStandardItem(XBinary::valueToHex(listRecords.at(i).toc.symbol_index)));
            (*g_ppModel)->setItem(i, N_mach_table_of_contents::module_index + 1, new QStandardItem(XBinary::valueToHex(listRecords.at(i).toc.module_index)));
            (*g_ppModel)
                ->setItem(i, N_mach_table_of_contents::module_index + 2,
                          new QStandardItem(g_pXMACH->getIndexSymbolName(listRecords.at(i).toc.symbol_index, &listNlistRecords, osStringTable.nOffset, osStringTable.nSize)));

            incValue();
        }
    } else if (g_nType == SMACH::TYPE_DYSYMTAB_modtab) {
        QList<QString> listLabels;
        listLabels.append("");

        if (bIs64) {
            listLabels.append(getStructList(N_mach_modtab64::records, N_mach_modtab64::__data_size));
        } else {
            listLabels.append(getStructList(N_mach_modtab32::records, N_mach_modtab64::__data_size));
        }

        QList<XMACH::MODTAB_RECORD> listRecords = g_pXMACH->get_modtab_list();

        int nNumberOfRecords = listRecords.count();

        *g_ppModel = new QStandardItemModel(nNumberOfRecords, listLabels.count());

        setMaximum(nNumberOfRecords);

        setHeader(*g_ppModel, &listLabels);

        for (qint32 i = 0; (i < nNumberOfRecords) && (isRun()); i++) {
            QStandardItem *pItem = new QStandardItem;
            pItem->setData(i, Qt::DisplayRole);
            pItem->setData(listRecords.at(i).nStructOffset, Qt::UserRole + FW_DEF::SECTION_DATA_HEADEROFFSET);
            (*g_ppModel)->setItem(i, 0, pItem);

            if (listRecords.at(i).bIs64) {
                (*g_ppModel)->setItem(i, N_mach_modtab64::module_name + 1, new QStandardItem(XBinary::valueToHex(listRecords.at(i).s.module64.module_name)));
                (*g_ppModel)->setItem(i, N_mach_modtab64::iextdefsym + 1, new QStandardItem(XBinary::valueToHex(listRecords.at(i).s.module64.iextdefsym)));
                (*g_ppModel)->setItem(i, N_mach_modtab64::nextdefsym + 1, new QStandardItem(XBinary::valueToHex(listRecords.at(i).s.module64.nextdefsym)));
                (*g_ppModel)->setItem(i, N_mach_modtab64::irefsym + 1, new QStandardItem(XBinary::valueToHex(listRecords.at(i).s.module64.irefsym)));
                (*g_ppModel)->setItem(i, N_mach_modtab64::nrefsym + 1, new QStandardItem(XBinary::valueToHex(listRecords.at(i).s.module64.nrefsym)));
                (*g_ppModel)->setItem(i, N_mach_modtab64::ilocalsym + 1, new QStandardItem(XBinary::valueToHex(listRecords.at(i).s.module64.ilocalsym)));
                (*g_ppModel)->setItem(i, N_mach_modtab64::nlocalsym + 1, new QStandardItem(XBinary::valueToHex(listRecords.at(i).s.module64.nlocalsym)));
                (*g_ppModel)->setItem(i, N_mach_modtab64::iextrel + 1, new QStandardItem(XBinary::valueToHex(listRecords.at(i).s.module64.iextrel)));
                (*g_ppModel)->setItem(i, N_mach_modtab64::nextrel + 1, new QStandardItem(XBinary::valueToHex(listRecords.at(i).s.module64.nextrel)));
                (*g_ppModel)->setItem(i, N_mach_modtab64::iinit_iterm + 1, new QStandardItem(XBinary::valueToHex(listRecords.at(i).s.module64.iinit_iterm)));
                (*g_ppModel)->setItem(i, N_mach_modtab64::ninit_nterm + 1, new QStandardItem(XBinary::valueToHex(listRecords.at(i).s.module64.ninit_nterm)));
                (*g_ppModel)
                    ->setItem(i, N_mach_modtab64::objc_module_info_size + 1, new QStandardItem(XBinary::valueToHex(listRecords.at(i).s.module64.objc_module_info_size)));
                (*g_ppModel)
                    ->setItem(i, N_mach_modtab64::objc_module_info_addr + 1, new QStandardItem(XBinary::valueToHex(listRecords.at(i).s.module64.objc_module_info_addr)));
            } else {
                (*g_ppModel)->setItem(i, N_mach_modtab32::module_name + 1, new QStandardItem(XBinary::valueToHex(listRecords.at(i).s.module32.module_name)));
                (*g_ppModel)->setItem(i, N_mach_modtab32::iextdefsym + 1, new QStandardItem(XBinary::valueToHex(listRecords.at(i).s.module32.iextdefsym)));
                (*g_ppModel)->setItem(i, N_mach_modtab32::nextdefsym + 1, new QStandardItem(XBinary::valueToHex(listRecords.at(i).s.module32.nextdefsym)));
                (*g_ppModel)->setItem(i, N_mach_modtab32::irefsym + 1, new QStandardItem(XBinary::valueToHex(listRecords.at(i).s.module32.irefsym)));
                (*g_ppModel)->setItem(i, N_mach_modtab32::nrefsym + 1, new QStandardItem(XBinary::valueToHex(listRecords.at(i).s.module32.nrefsym)));
                (*g_ppModel)->setItem(i, N_mach_modtab32::ilocalsym + 1, new QStandardItem(XBinary::valueToHex(listRecords.at(i).s.module32.ilocalsym)));
                (*g_ppModel)->setItem(i, N_mach_modtab32::nlocalsym + 1, new QStandardItem(XBinary::valueToHex(listRecords.at(i).s.module32.nlocalsym)));
                (*g_ppModel)->setItem(i, N_mach_modtab32::iextrel + 1, new QStandardItem(XBinary::valueToHex(listRecords.at(i).s.module32.iextrel)));
                (*g_ppModel)->setItem(i, N_mach_modtab32::nextrel + 1, new QStandardItem(XBinary::valueToHex(listRecords.at(i).s.module32.nextrel)));
                (*g_ppModel)->setItem(i, N_mach_modtab32::iinit_iterm + 1, new QStandardItem(XBinary::valueToHex(listRecords.at(i).s.module32.iinit_iterm)));
                (*g_ppModel)->setItem(i, N_mach_modtab32::ninit_nterm + 1, new QStandardItem(XBinary::valueToHex(listRecords.at(i).s.module32.ninit_nterm)));
                (*g_ppModel)
                    ->setItem(i, N_mach_modtab32::objc_module_info_addr + 1, new QStandardItem(XBinary::valueToHex(listRecords.at(i).s.module32.objc_module_info_addr)));
                (*g_ppModel)
                    ->setItem(i, N_mach_modtab32::objc_module_info_size + 1, new QStandardItem(XBinary::valueToHex(listRecords.at(i).s.module32.objc_module_info_size)));
            }

            incValue();
        }
    } else if (g_nType == SMACH::TYPE_DYSYMTAB_extrefsyms) {
        QList<QString> listLabels;
        listLabels.append("");
        listLabels.append(getStructList(N_mach_refsyms::records, N_mach_refsyms::__data_size));
        listLabels.append(tr("Name"));

        QList<XMACH::REFERENCE_RECORD> listRecords = g_pXMACH->get_extrefsyms_list();
        QList<XMACH::NLIST_RECORD> listNlistRecords = g_pXMACH->getNlistRecords();
        XBinary::OFFSETSIZE osStringTable = g_pXMACH->getStringTableOffsetSize();

        int nNumberOfRecords = listRecords.count();

        *g_ppModel = new QStandardItemModel(nNumberOfRecords, listLabels.count());

        setMaximum(nNumberOfRecords);

        setHeader(*g_ppModel, &listLabels);

        for (qint32 i = 0; (i < nNumberOfRecords) && (isRun()); i++) {
            QStandardItem *pItem = new QStandardItem;
            pItem->setData(i, Qt::DisplayRole);
            pItem->setData(listRecords.at(i).nStructOffset, Qt::UserRole + FW_DEF::SECTION_DATA_HEADEROFFSET);
            (*g_ppModel)->setItem(i, 0, pItem);
            (*g_ppModel)->setItem(i, N_mach_refsyms::isym + 1, new QStandardItem(XBinary::valueToHex(listRecords.at(i).reference.s._value.isym)));
            (*g_ppModel)->setItem(i, N_mach_refsyms::flags + 1, new QStandardItem(XBinary::valueToHex((quint8)listRecords.at(i).reference.s._value.flags)));

            QString sName = g_pXMACH->getIndexSymbolName(listRecords.at(i).reference.s._value.isym, &listNlistRecords, osStringTable.nOffset, osStringTable.nSize);

            (*g_ppModel)->setItem(i, 3, new QStandardItem(sName));

            incValue();
        }
    } else if (g_nType == SMACH::TYPE_DYSYMTAB_indirectsyms) {
        QList<QString> listLabels;
        listLabels.append("");
        listLabels.append(tr("Value"));
        listLabels.append(tr("Name"));

        QList<XMACH::VALUE32_RECORD> listRecords = g_pXMACH->get_indirectsyms_list();
        QList<XMACH::NLIST_RECORD> listNlistRecords = g_pXMACH->getNlistRecords();
        XBinary::OFFSETSIZE osStringTable = g_pXMACH->getStringTableOffsetSize();

        int nNumberOfRecords = listRecords.count();

        *g_ppModel = new QStandardItemModel(nNumberOfRecords, listLabels.count());

        setMaximum(nNumberOfRecords);

        setHeader(*g_ppModel, &listLabels);

        for (qint32 i = 0; (i < nNumberOfRecords) && (isRun()); i++) {
            QStandardItem *pItem = new QStandardItem;
            pItem->setData(i, Qt::DisplayRole);
            pItem->setData(listRecords.at(i).nStructOffset, Qt::UserRole + FW_DEF::SECTION_DATA_HEADEROFFSET);
            (*g_ppModel)->setItem(i, 0, pItem);
            (*g_ppModel)->setItem(i, 1, new QStandardItem(XBinary::valueToHex(listRecords.at(i).nValue)));

            QString sName = g_pXMACH->getIndexSymbolName(listRecords.at(i).nValue, &listNlistRecords, osStringTable.nOffset, osStringTable.nSize);

            (*g_ppModel)->setItem(i, 2, new QStandardItem(sName));

            incValue();
        }
    } else if ((g_nType == SMACH::TYPE_DYSYMTAB_extrel) || (g_nType == SMACH::TYPE_DYSYMTAB_locrel)) {
        QList<QString> listLabels;
        listLabels.append("");
        listLabels.append(getStructList(N_mach_relocs_E::records, N_mach_relocs_E::__data_size));

        QList<XMACH::RELOC_RECORD> listRecords;

        if (g_nType == SMACH::TYPE_DYSYMTAB_extrel) {
            listRecords = g_pXMACH->get_extrel_list();
        } else if (g_nType == SMACH::TYPE_DYSYMTAB_locrel) {
            listRecords = g_pXMACH->get_locrel_list();
        }

        int nNumberOfRecords = listRecords.count();

        *g_ppModel = new QStandardItemModel(nNumberOfRecords, listLabels.count());

        setMaximum(nNumberOfRecords);

        setHeader(*g_ppModel, &listLabels);

        for (qint32 i = 0; (i < nNumberOfRecords) && (isRun()); i++) {
            QStandardItem *pItem = new QStandardItem;
            pItem->setData(i, Qt::DisplayRole);
            pItem->setData(listRecords.at(i).nStructOffset, Qt::UserRole + FW_DEF::SECTION_DATA_HEADEROFFSET);
            (*g_ppModel)->setItem(i, 0, pItem);
            (*g_ppModel)->setItem(i, N_mach_relocs_E::r_address + 1, new QStandardItem(XBinary::valueToHex(listRecords.at(i).reloc.r_address)));
            (*g_ppModel)->setItem(i, N_mach_relocs_E::r_symbolnum + 1, new QStandardItem(XBinary::valueToHex(listRecords.at(i).reloc.s._value.r_symbolnum)));
            (*g_ppModel)->setItem(i, N_mach_relocs_E::r_pcrel + 1, new QStandardItem(XBinary::valueToHex((quint8)listRecords.at(i).reloc.s._value.r_pcrel)));
            (*g_ppModel)->setItem(i, N_mach_relocs_E::r_length + 1, new QStandardItem(XBinary::valueToHex((quint8)listRecords.at(i).reloc.s._value.r_length)));
            (*g_ppModel)->setItem(i, N_mach_relocs_E::r_extern + 1, new QStandardItem(XBinary::valueToHex((quint8)listRecords.at(i).reloc.s._value.r_extern)));
            (*g_ppModel)->setItem(i, N_mach_relocs_E::r_type + 1, new QStandardItem(XBinary::valueToHex((quint8)listRecords.at(i).reloc.s._value.r_type)));

            incValue();
        }
    }
}

void MACHProcessData::ajustTableView(QWidget *pWidget, QTableView *pTableView)
{
    XBinary::MODE mode = g_pXMACH->getMode();

    if (g_nType == SMACH::TYPE_mach_commands) {
        pTableView->setColumnWidth(0, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT16, mode));
        pTableView->setColumnWidth(1, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(2, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(3, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_STRINGMID, mode));
    } else if ((g_nType == SMACH::TYPE_mach_id_library) || (g_nType == SMACH::TYPE_mach_weak_libraries) || (g_nType == SMACH::TYPE_mach_libraries)) {
        pTableView->setColumnWidth(0, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT16, mode));
        pTableView->setColumnWidth(1, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(2, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(3, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(4, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_STRINGLONG, mode));
    } else if ((g_nType == SMACH::TYPE_mach_LOADFVMLIB) || (g_nType == SMACH::TYPE_mach_IDFVMLIB)) {
        pTableView->setColumnWidth(0, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT16, mode));
        pTableView->setColumnWidth(1, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(2, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(3, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_STRINGLONG, mode));
    } else if (g_nType == SMACH::TYPE_mach_segments) {
        pTableView->setColumnWidth(0, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT16, mode));
        pTableView->setColumnWidth(1, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_STRINGSHORT, mode));
        pTableView->setColumnWidth(2, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINTMODE, mode));
        pTableView->setColumnWidth(3, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINTMODE, mode));
        pTableView->setColumnWidth(4, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINTMODE, mode));
        pTableView->setColumnWidth(5, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINTMODE, mode));
        pTableView->setColumnWidth(6, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(7, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(8, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(9, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
    } else if (g_nType == SMACH::TYPE_mach_sections) {
        pTableView->setColumnWidth(0, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT16, mode));
        pTableView->setColumnWidth(1, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_STRINGSHORT, mode));
        pTableView->setColumnWidth(2, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_STRINGSHORT, mode));
        pTableView->setColumnWidth(3, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINTMODE, mode));
        pTableView->setColumnWidth(4, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINTMODE, mode));
        pTableView->setColumnWidth(5, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(6, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(7, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(8, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(9, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(10, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(11, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(12, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
    } else if (g_nType == SMACH::TYPE_SYMBOLTABLE) {
        pTableView->setColumnWidth(0, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT16, mode));
        pTableView->setColumnWidth(1, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(2, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT16, mode));
        pTableView->setColumnWidth(3, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT16, mode));
        pTableView->setColumnWidth(4, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(5, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINTMODE, mode));
        pTableView->setColumnWidth(6, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_STRINGLONG, mode));
    } else if (g_nType == SMACH::TYPE_FUNCTIONS) {
        pTableView->setColumnWidth(0, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT16, mode));
        pTableView->setColumnWidth(1, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINTMODE, mode));
        pTableView->setColumnWidth(2, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINTMODE, mode));
        pTableView->setColumnWidth(3, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_STRINGLONG, mode));
    } else if (g_nType == SMACH::TYPE_DICE) {
        pTableView->setColumnWidth(0, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT16, mode));
        pTableView->setColumnWidth(1, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(2, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(3, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
    } else if ((g_nType == SMACH::TYPE_DYLD_INFO_bind) || (g_nType == SMACH::TYPE_DYLD_INFO_export) || (g_nType == SMACH::TYPE_DYLD_INFO_lazy_bind) ||
               (g_nType == SMACH::TYPE_DYLD_INFO_rebase) || (g_nType == SMACH::TYPE_DYLD_INFO_weak_bind)) {
        pTableView->setColumnWidth(0, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT16, mode));
        pTableView->setColumnWidth(1, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(2, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_STRINGLONG, mode));
    } else if (g_nType == SMACH::TYPE_DYSYMTAB_toc) {
        pTableView->setColumnWidth(0, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT16, mode));
        pTableView->setColumnWidth(1, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(2, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(3, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_STRINGLONG, mode));
    } else if (g_nType == SMACH::TYPE_DYSYMTAB_modtab) {
        pTableView->setColumnWidth(0, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT16, mode));
        pTableView->setColumnWidth(1, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(2, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(3, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(4, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(5, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(6, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(7, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(8, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(9, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(10, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(11, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(12, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(13, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINTMODE, mode));
    } else if (g_nType == SMACH::TYPE_DYSYMTAB_extrefsyms) {
        pTableView->setColumnWidth(0, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT16, mode));
        pTableView->setColumnWidth(1, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(2, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT8, mode));
        pTableView->setColumnWidth(3, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_STRINGLONG, mode));
    } else if (g_nType == SMACH::TYPE_DYSYMTAB_indirectsyms) {
        pTableView->setColumnWidth(0, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT16, mode));
        pTableView->setColumnWidth(1, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(2, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_STRINGLONG, mode));
    } else if ((g_nType == SMACH::TYPE_DYSYMTAB_extrel) || (g_nType == SMACH::TYPE_DYSYMTAB_locrel)) {
        pTableView->setColumnWidth(0, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT16, mode));
        pTableView->setColumnWidth(1, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(2, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(3, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT8, mode));
        pTableView->setColumnWidth(4, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT8, mode));
        pTableView->setColumnWidth(5, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT8, mode));
        pTableView->setColumnWidth(6, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT8, mode));
    }
}

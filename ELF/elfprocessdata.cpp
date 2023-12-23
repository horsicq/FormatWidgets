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
#include "elfprocessdata.h"

ELFProcessData::ELFProcessData(qint32 nType, QStandardItemModel **ppModel, XELF *pELF, qint64 nOffset, qint64 nSize, qint64 nStringTableOffset, qint64 nStringTableSize)
    : ProcessData()
{
    this->g_nType = nType;
    this->g_ppModel = ppModel;
    this->g_pELF = pELF;
    this->g_nOffset = nOffset;
    this->g_nSize = nSize;
    this->g_nStringTableOffset = nStringTableOffset;
    this->g_nStringTableSize = nStringTableSize;
}

void ELFProcessData::_process()
{
    if (g_nType == SELF::TYPE_Elf_Shdr) {
        bool bIs64 = g_pELF->is64();

        QList<QString> listLabels;
        listLabels.append("");

        if (bIs64) {
            listLabels.append(getStructList(N_Elf_Shdr::records64, N_Elf_Shdr::__data_size));
        } else {
            listLabels.append(getStructList(N_Elf_Shdr::records32, N_Elf_Shdr::__data_size));
        }

        listLabels.append(tr("Name"));
        listLabels.append(tr("Type"));

        QMap<quint64, QString> mapTypes = XELF::getSectionTypesS();

        QList<XELF_DEF::Elf64_Shdr> listSectionHeaders64;
        QList<XELF_DEF::Elf32_Shdr> listSectionHeaders32;

        qint32 nNumberOfSections = 0;

        if (bIs64) {
            listSectionHeaders64 = g_pELF->getElf64_ShdrList(-1);
            nNumberOfSections = listSectionHeaders64.count();
        } else {
            listSectionHeaders32 = g_pELF->getElf32_ShdrList(-1);
            nNumberOfSections = listSectionHeaders32.count();
        }

        *g_ppModel = new QStandardItemModel(nNumberOfSections, listLabels.count());

        setMaximum(nNumberOfSections);

        setTableHeader(*g_ppModel, &listLabels);

        quint32 nMainStringSection = g_pELF->getSectionStringTable();

        XBinary::OFFSETSIZE osStringTable = g_pELF->getSectionOffsetSize(nMainStringSection);

        for (qint32 i = 0; (i < nNumberOfSections) && (isRun()); i++) {
            if (bIs64) {
                QStandardItem *pItem = new QStandardItem(QString::number(i));

                if (g_pELF->isImage()) {
                    pItem->setData(listSectionHeaders64.at(i).sh_addr, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET);
                } else {
                    pItem->setData(listSectionHeaders64.at(i).sh_offset, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET);
                }

                pItem->setData(listSectionHeaders64.at(i).sh_size, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE);
                pItem->setData(listSectionHeaders64.at(i).sh_addr, Qt::UserRole + FW_DEF::SECTION_DATA_ADDRESS);

                pItem->setData(osStringTable.nOffset, Qt::UserRole + FW_DEF::SECTION_DATA_STRINGTABLEOFFSET);
                pItem->setData(osStringTable.nSize, Qt::UserRole + FW_DEF::SECTION_DATA_STRINGTABLESIZE);

                (*g_ppModel)->setItem(i, 0, pItem);
                (*g_ppModel)->setItem(i, N_Elf_Shdr::sh_name + 1, new QStandardItem(XBinary::valueToHex(listSectionHeaders64.at(i).sh_name)));
                (*g_ppModel)->setItem(i, N_Elf_Shdr::sh_type + 1, new QStandardItem(XBinary::valueToHex(listSectionHeaders64.at(i).sh_type)));
                (*g_ppModel)->setItem(i, N_Elf_Shdr::sh_flags + 1, new QStandardItem(XBinary::valueToHex(listSectionHeaders64.at(i).sh_flags)));
                (*g_ppModel)->setItem(i, N_Elf_Shdr::sh_addr + 1, new QStandardItem(XBinary::valueToHex(listSectionHeaders64.at(i).sh_addr)));
                (*g_ppModel)->setItem(i, N_Elf_Shdr::sh_offset + 1, new QStandardItem(XBinary::valueToHex(listSectionHeaders64.at(i).sh_offset)));
                (*g_ppModel)->setItem(i, N_Elf_Shdr::sh_size + 1, new QStandardItem(XBinary::valueToHex(listSectionHeaders64.at(i).sh_size)));
                (*g_ppModel)->setItem(i, N_Elf_Shdr::sh_link + 1, new QStandardItem(XBinary::valueToHex(listSectionHeaders64.at(i).sh_link)));
                (*g_ppModel)->setItem(i, N_Elf_Shdr::sh_info + 1, new QStandardItem(XBinary::valueToHex(listSectionHeaders64.at(i).sh_info)));
                (*g_ppModel)->setItem(i, N_Elf_Shdr::sh_addralign + 1, new QStandardItem(XBinary::valueToHex(listSectionHeaders64.at(i).sh_addralign)));
                (*g_ppModel)->setItem(i, N_Elf_Shdr::sh_entsize + 1, new QStandardItem(XBinary::valueToHex(listSectionHeaders64.at(i).sh_entsize)));
                (*g_ppModel)
                    ->setItem(i, N_Elf_Shdr::sh_entsize + 2,
                              new QStandardItem(g_pELF->getStringFromIndex(osStringTable.nOffset, osStringTable.nSize, listSectionHeaders64.at(i).sh_name)));
                (*g_ppModel)->setItem(i, N_Elf_Shdr::sh_entsize + 3, new QStandardItem(mapTypes.value(listSectionHeaders64.at(i).sh_type)));
            } else {
                QStandardItem *pItem = new QStandardItem(QString::number(i));

                if (g_pELF->isImage()) {
                    pItem->setData(listSectionHeaders32.at(i).sh_addr, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET);
                } else {
                    pItem->setData(listSectionHeaders32.at(i).sh_offset, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET);
                }

                pItem->setData(listSectionHeaders32.at(i).sh_size, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE);
                pItem->setData(listSectionHeaders32.at(i).sh_addr, Qt::UserRole + FW_DEF::SECTION_DATA_ADDRESS);

                pItem->setData(g_nStringTableOffset, Qt::UserRole + FW_DEF::SECTION_DATA_STRINGTABLEOFFSET);
                pItem->setData(g_nStringTableSize, Qt::UserRole + FW_DEF::SECTION_DATA_STRINGTABLESIZE);

                (*g_ppModel)->setItem(i, 0, pItem);
                (*g_ppModel)->setItem(i, N_Elf_Shdr::sh_name + 1, new QStandardItem(XBinary::valueToHex(listSectionHeaders32.at(i).sh_name)));
                (*g_ppModel)->setItem(i, N_Elf_Shdr::sh_type + 1, new QStandardItem(XBinary::valueToHex(listSectionHeaders32.at(i).sh_type)));
                (*g_ppModel)->setItem(i, N_Elf_Shdr::sh_flags + 1, new QStandardItem(XBinary::valueToHex(listSectionHeaders32.at(i).sh_flags)));
                (*g_ppModel)->setItem(i, N_Elf_Shdr::sh_addr + 1, new QStandardItem(XBinary::valueToHex(listSectionHeaders32.at(i).sh_addr)));
                (*g_ppModel)->setItem(i, N_Elf_Shdr::sh_offset + 1, new QStandardItem(XBinary::valueToHex(listSectionHeaders32.at(i).sh_offset)));
                (*g_ppModel)->setItem(i, N_Elf_Shdr::sh_size + 1, new QStandardItem(XBinary::valueToHex(listSectionHeaders32.at(i).sh_size)));
                (*g_ppModel)->setItem(i, N_Elf_Shdr::sh_link + 1, new QStandardItem(XBinary::valueToHex(listSectionHeaders32.at(i).sh_link)));
                (*g_ppModel)->setItem(i, N_Elf_Shdr::sh_info + 1, new QStandardItem(XBinary::valueToHex(listSectionHeaders32.at(i).sh_info)));
                (*g_ppModel)->setItem(i, N_Elf_Shdr::sh_addralign + 1, new QStandardItem(XBinary::valueToHex(listSectionHeaders32.at(i).sh_addralign)));
                (*g_ppModel)->setItem(i, N_Elf_Shdr::sh_entsize + 1, new QStandardItem(XBinary::valueToHex(listSectionHeaders32.at(i).sh_entsize)));
                (*g_ppModel)
                    ->setItem(i, N_Elf_Shdr::sh_entsize + 2,
                              new QStandardItem(g_pELF->getStringFromIndex(osStringTable.nOffset, osStringTable.nSize, listSectionHeaders32.at(i).sh_name)));
                (*g_ppModel)->setItem(i, N_Elf_Shdr::sh_entsize + 3, new QStandardItem(mapTypes.value(listSectionHeaders32.at(i).sh_type)));
            }

            incValue();
        }
    } else if (g_nType == SELF::TYPE_Elf_Phdr) {
        bool bIs64 = g_pELF->is64();

        QList<QString> listLabels;
        listLabels.append("");

        if (bIs64) {
            listLabels.append(getStructList(N_Elf_Phdr64::records, N_Elf_Phdr64::__data_size));
        } else {
            listLabels.append(getStructList(N_Elf_Phdr32::records, N_Elf_Phdr32::__data_size));
        }

        listLabels.append(tr("Type"));

        QMap<quint64, QString> mapProgramTypes = XELF::getProgramTypesS();

        QList<XELF_DEF::Elf64_Phdr> listPrograms64;
        QList<XELF_DEF::Elf32_Phdr> listPrograms32;

        qint32 nNumberOfPrograms = 0;

        if (bIs64) {
            listPrograms64 = g_pELF->getElf64_PhdrList(1000);
            nNumberOfPrograms = listPrograms64.count();
        } else {
            listPrograms32 = g_pELF->getElf32_PhdrList(1000);
            nNumberOfPrograms = listPrograms32.count();
        }

        *g_ppModel = new QStandardItemModel(nNumberOfPrograms, listLabels.count());

        setMaximum(nNumberOfPrograms);

        setTableHeader(*g_ppModel, &listLabels);

        for (qint32 i = 0; (i < nNumberOfPrograms) && (isRun()); i++) {
            if (bIs64) {
                QStandardItem *pItem = new QStandardItem(QString::number(i));

                if (g_pELF->isImage()) {
                    pItem->setData(listPrograms64.at(i).p_memsz, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE);
                    pItem->setData(listPrograms64.at(i).p_vaddr, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET);
                } else {
                    pItem->setData(listPrograms64.at(i).p_filesz, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE);
                    pItem->setData(listPrograms64.at(i).p_offset, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET);
                }

                pItem->setData(listPrograms64.at(i).p_vaddr, Qt::UserRole + FW_DEF::SECTION_DATA_ADDRESS);

                (*g_ppModel)->setItem(i, 0, pItem);
                (*g_ppModel)->setItem(i, N_Elf_Phdr64::p_type + 1, new QStandardItem(XBinary::valueToHex(listPrograms64.at(i).p_type)));
                (*g_ppModel)->setItem(i, N_Elf_Phdr64::p_flags + 1, new QStandardItem(XBinary::valueToHex(listPrograms64.at(i).p_flags)));
                (*g_ppModel)->setItem(i, N_Elf_Phdr64::p_offset + 1, new QStandardItem(XBinary::valueToHex(listPrograms64.at(i).p_offset)));
                (*g_ppModel)->setItem(i, N_Elf_Phdr64::p_vaddr + 1, new QStandardItem(XBinary::valueToHex(listPrograms64.at(i).p_vaddr)));
                (*g_ppModel)->setItem(i, N_Elf_Phdr64::p_paddr + 1, new QStandardItem(XBinary::valueToHex(listPrograms64.at(i).p_paddr)));
                (*g_ppModel)->setItem(i, N_Elf_Phdr64::p_filesz + 1, new QStandardItem(XBinary::valueToHex(listPrograms64.at(i).p_filesz)));
                (*g_ppModel)->setItem(i, N_Elf_Phdr64::p_memsz + 1, new QStandardItem(XBinary::valueToHex(listPrograms64.at(i).p_memsz)));
                (*g_ppModel)->setItem(i, N_Elf_Phdr64::p_align + 1, new QStandardItem(XBinary::valueToHex(listPrograms64.at(i).p_align)));
                (*g_ppModel)->setItem(i, N_Elf_Phdr64::p_align + 2, new QStandardItem(mapProgramTypes.value(listPrograms64.at(i).p_type)));
            } else {
                QStandardItem *pItem = new QStandardItem(QString::number(i));

                if (g_pELF->isImage()) {
                    pItem->setData(listPrograms32.at(i).p_memsz, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE);
                    pItem->setData(listPrograms32.at(i).p_vaddr, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET);
                } else {
                    pItem->setData(listPrograms32.at(i).p_filesz, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE);
                    pItem->setData(listPrograms32.at(i).p_offset, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET);
                }

                pItem->setData(listPrograms32.at(i).p_vaddr, Qt::UserRole + FW_DEF::SECTION_DATA_ADDRESS);

                (*g_ppModel)->setItem(i, 0, pItem);
                (*g_ppModel)->setItem(i, N_Elf_Phdr32::p_type + 1, new QStandardItem(XBinary::valueToHex(listPrograms32.at(i).p_type)));
                (*g_ppModel)->setItem(i, N_Elf_Phdr32::p_offset + 1, new QStandardItem(XBinary::valueToHex(listPrograms32.at(i).p_offset)));
                (*g_ppModel)->setItem(i, N_Elf_Phdr32::p_vaddr + 1, new QStandardItem(XBinary::valueToHex(listPrograms32.at(i).p_vaddr)));
                (*g_ppModel)->setItem(i, N_Elf_Phdr32::p_paddr + 1, new QStandardItem(XBinary::valueToHex(listPrograms32.at(i).p_paddr)));
                (*g_ppModel)->setItem(i, N_Elf_Phdr32::p_filesz + 1, new QStandardItem(XBinary::valueToHex(listPrograms32.at(i).p_filesz)));
                (*g_ppModel)->setItem(i, N_Elf_Phdr32::p_memsz + 1, new QStandardItem(XBinary::valueToHex(listPrograms32.at(i).p_memsz)));
                (*g_ppModel)->setItem(i, N_Elf_Phdr32::p_flags + 1, new QStandardItem(XBinary::valueToHex(listPrograms32.at(i).p_flags)));
                (*g_ppModel)->setItem(i, N_Elf_Phdr32::p_align + 1, new QStandardItem(XBinary::valueToHex(listPrograms32.at(i).p_align)));
                (*g_ppModel)->setItem(i, N_Elf_Phdr32::p_align + 2, new QStandardItem(mapProgramTypes.value(listPrograms32.at(i).p_type)));
            }
        }
    } else if (g_nType == SELF::TYPE_SYMBOLTABLE) {
        QMap<quint64, QString> mapBinds = g_pELF->getStBindsS();
        QMap<quint64, QString> mapTypes = g_pELF->getStTypesS();

        if (g_pELF->is64()) {
            QList<QString> listLabels;
            listLabels.append("");
            listLabels.append(getStructList(N_Elf64_Sym::records, N_Elf64_Sym::__data_size));
            listLabels.append(tr("Name"));
            listLabels.append(tr("Bind"));
            listLabels.append(tr("Type"));

            QList<XELF_DEF::Elf64_Sym> listSymbols = g_pELF->getElf64_SymList(g_nOffset, g_nSize);

            qint32 nNumberOfSymbols = listSymbols.count();

            *g_ppModel = new QStandardItemModel(nNumberOfSymbols, listLabels.count());

            setMaximum(nNumberOfSymbols);

            setTableHeader(*g_ppModel, &listLabels);

            for (qint32 i = 0; (i < nNumberOfSymbols) && (isRun()); i++) {
                QStandardItem *pItem = new QStandardItem;
                pItem->setData(i, Qt::DisplayRole);
                pItem->setTextAlignment(Qt::AlignRight);

                pItem->setData(g_nOffset + i * sizeof(XELF_DEF::Elf64_Sym), Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET);
                pItem->setData(g_nOffset + i * sizeof(XELF_DEF::Elf64_Sym), Qt::UserRole + FW_DEF::SECTION_DATA_HEADEROFFSET);

                pItem->setData(g_nStringTableOffset, Qt::UserRole + FW_DEF::SECTION_DATA_STRINGTABLEOFFSET);
                pItem->setData(g_nStringTableSize, Qt::UserRole + FW_DEF::SECTION_DATA_STRINGTABLESIZE);

                (*g_ppModel)->setItem(i, 0, pItem);
                (*g_ppModel)->setItem(i, N_Elf64_Sym::st_name + 1, new QStandardItem(XBinary::valueToHex(listSymbols.at(i).st_name)));
                (*g_ppModel)->setItem(i, N_Elf64_Sym::st_info + 1, new QStandardItem(XBinary::valueToHex(listSymbols.at(i).st_info)));
                (*g_ppModel)->setItem(i, N_Elf64_Sym::st_other + 1, new QStandardItem(XBinary::valueToHex(listSymbols.at(i).st_other)));
                (*g_ppModel)->setItem(i, N_Elf64_Sym::st_shndx + 1, new QStandardItem(XBinary::valueToHex(listSymbols.at(i).st_shndx)));
                (*g_ppModel)->setItem(i, N_Elf64_Sym::st_value + 1, new QStandardItem(XBinary::valueToHex(listSymbols.at(i).st_value)));
                (*g_ppModel)->setItem(i, N_Elf64_Sym::st_size + 1, new QStandardItem(XBinary::valueToHex(listSymbols.at(i).st_size)));

                QString sName = g_pELF->getStringFromIndex(g_nStringTableOffset, g_nStringTableSize, listSymbols.at(i).st_name);

                (*g_ppModel)->setItem(i, N_Elf64_Sym::st_size + 2, new QStandardItem(sName));
                (*g_ppModel)->setItem(i, N_Elf64_Sym::st_size + 3, new QStandardItem(mapBinds.value(S_ELF64_ST_BIND(listSymbols.at(i).st_info))));
                (*g_ppModel)->setItem(i, N_Elf64_Sym::st_size + 4, new QStandardItem(mapTypes.value(S_ELF64_ST_TYPE(listSymbols.at(i).st_info))));

                incValue();
            }
        } else {
            QList<QString> listLabels;
            listLabels.append("");
            listLabels.append(getStructList(N_Elf32_Sym::records, N_Elf32_Sym::__data_size));
            listLabels.append(tr("Name"));
            listLabels.append(tr("Bind"));
            listLabels.append(tr("Type"));

            QList<XELF_DEF::Elf32_Sym> listSymbols = g_pELF->getElf32_SymList(g_nOffset, g_nSize);

            qint32 nNumberOfSymbols = listSymbols.count();

            *g_ppModel = new QStandardItemModel(nNumberOfSymbols, listLabels.count());

            setMaximum(nNumberOfSymbols);

            setTableHeader(*g_ppModel, &listLabels);

            for (qint32 i = 0; (i < nNumberOfSymbols) && (isRun()); i++) {
                QStandardItem *pItem = new QStandardItem;
                pItem->setData(i, Qt::DisplayRole);
                pItem->setTextAlignment(Qt::AlignRight);

                pItem->setData(g_nOffset + i * sizeof(XELF_DEF::Elf32_Sym), Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET);
                pItem->setData(g_nOffset + i * sizeof(XELF_DEF::Elf32_Sym), Qt::UserRole + FW_DEF::SECTION_DATA_HEADEROFFSET);

                pItem->setData(g_nStringTableOffset, Qt::UserRole + FW_DEF::SECTION_DATA_STRINGTABLEOFFSET);
                pItem->setData(g_nStringTableSize, Qt::UserRole + FW_DEF::SECTION_DATA_STRINGTABLESIZE);

                (*g_ppModel)->setItem(i, 0, pItem);
                (*g_ppModel)->setItem(i, N_Elf32_Sym::st_name + 1, new QStandardItem(XBinary::valueToHex(listSymbols.at(i).st_name)));
                (*g_ppModel)->setItem(i, N_Elf32_Sym::st_value + 1, new QStandardItem(XBinary::valueToHex(listSymbols.at(i).st_value)));
                (*g_ppModel)->setItem(i, N_Elf32_Sym::st_size + 1, new QStandardItem(XBinary::valueToHex(listSymbols.at(i).st_size)));
                (*g_ppModel)->setItem(i, N_Elf32_Sym::st_info + 1, new QStandardItem(XBinary::valueToHex(listSymbols.at(i).st_info)));
                (*g_ppModel)->setItem(i, N_Elf32_Sym::st_other + 1, new QStandardItem(XBinary::valueToHex(listSymbols.at(i).st_other)));
                (*g_ppModel)->setItem(i, N_Elf32_Sym::st_shndx + 1, new QStandardItem(XBinary::valueToHex(listSymbols.at(i).st_shndx)));

                QString sName = g_pELF->getStringFromIndex(g_nStringTableOffset, g_nStringTableSize, listSymbols.at(i).st_name);

                (*g_ppModel)->setItem(i, N_Elf32_Sym::st_shndx + 2, new QStandardItem(sName));
                (*g_ppModel)->setItem(i, N_Elf64_Sym::st_size + 3, new QStandardItem(mapBinds.value(S_ELF32_ST_BIND(listSymbols.at(i).st_info))));
                (*g_ppModel)->setItem(i, N_Elf64_Sym::st_size + 4, new QStandardItem(mapTypes.value(S_ELF32_ST_TYPE(listSymbols.at(i).st_info))));

                incValue();
            }
        }
    } else if (g_nType == SELF::TYPE_Elf_DynamicArrayTags) {
        bool bIs64 = g_pELF->is64();

        QList<QString> listLabels;
        listLabels.append("");

        if (bIs64) {
            listLabels.append(getStructList(N_Elf_DynamicArrayTags::records64, N_Elf_DynamicArrayTags::__data_size));
        } else {
            listLabels.append(getStructList(N_Elf_DynamicArrayTags::records32, N_Elf_DynamicArrayTags::__data_size));
        }

        listLabels.append(tr("Type"));

        QList<XELF::TAG_STRUCT> listTagStructs = g_pELF->_getTagStructs(g_nOffset, g_nSize, bIs64, g_pELF->isBigEndian());

        qint32 nNumberOfTags = listTagStructs.count();

        *g_ppModel = new QStandardItemModel(nNumberOfTags, listLabels.count());

        setMaximum(nNumberOfTags);

        setTableHeader(*g_ppModel, &listLabels);

        QMap<quint64, QString> mapTags = g_pELF->getDynamicTagsS();

        for (qint32 i = 0; (i < nNumberOfTags) && (isRun()); i++) {
            QStandardItem *pItem = new QStandardItem(QString::number(i));

            pItem->setData(listTagStructs.at(i).nOffset, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET);
            pItem->setData(listTagStructs.at(i).nOffset, Qt::UserRole + FW_DEF::SECTION_DATA_HEADEROFFSET);

            pItem->setData(g_nStringTableOffset, Qt::UserRole + FW_DEF::SECTION_DATA_STRINGTABLEOFFSET);
            pItem->setData(g_nStringTableSize, Qt::UserRole + FW_DEF::SECTION_DATA_STRINGTABLESIZE);

            (*g_ppModel)->setItem(i, 0, pItem);

            if (bIs64) {
                (*g_ppModel)->setItem(i, N_Elf_DynamicArrayTags::d_tag + 1, new QStandardItem(XBinary::valueToHex((quint64)listTagStructs.at(i).nTag)));
                (*g_ppModel)->setItem(i, N_Elf_DynamicArrayTags::d_value + 1, new QStandardItem(XBinary::valueToHex((quint64)listTagStructs.at(i).nValue)));
            } else {
                (*g_ppModel)->setItem(i, N_Elf_DynamicArrayTags::d_tag + 1, new QStandardItem(XBinary::valueToHex((quint32)listTagStructs.at(i).nTag)));
                (*g_ppModel)->setItem(i, N_Elf_DynamicArrayTags::d_value + 1, new QStandardItem(XBinary::valueToHex((quint32)listTagStructs.at(i).nValue)));
            }

            (*g_ppModel)->setItem(i, N_Elf_DynamicArrayTags::d_value + 2, new QStandardItem(mapTags.value(listTagStructs.at(i).nTag)));

            incValue();
        }
    } else if (g_nType == SELF::TYPE_NOTES) {
        QList<QString> listLabels;
        listLabels.append("");

        listLabels.append(getStructList(N_ELF_NOTES::records, N_ELF_NOTES::__data_size));

        QList<XELF::NOTE> listNotes = g_pELF->_getNotes(g_nOffset, g_nSize, g_pELF->isBigEndian());

        qint32 nNumberOfNotes = listNotes.count();

        *g_ppModel = new QStandardItemModel(nNumberOfNotes, listLabels.count());

        setMaximum(nNumberOfNotes);

        setTableHeader(*g_ppModel, &listLabels);

        for (qint32 i = 0; (i < nNumberOfNotes) && (isRun()); i++) {
            QStandardItem *pItem = new QStandardItem(QString::number(i));

            pItem->setData(listNotes.at(i).nOffset, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET);
            pItem->setData(listNotes.at(i).nOffset, Qt::UserRole + FW_DEF::SECTION_DATA_HEADEROFFSET);
            pItem->setData(listNotes.at(i).nOffset, Qt::UserRole + FW_DEF::SECTION_DATA_ADDRESS);
            pItem->setData(listNotes.at(i).nSize, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE);

            (*g_ppModel)->setItem(i, 0, pItem);
            (*g_ppModel)->setItem(i, N_ELF_NOTES::type + 1, new QStandardItem(XBinary::valueToHex((quint32)listNotes.at(i).nType)));
            (*g_ppModel)->setItem(i, N_ELF_NOTES::name + 1, new QStandardItem(listNotes.at(i).sName));

            incValue();
        }
    } else if (g_nType == SELF::TYPE_LIBRARIES) {
        QList<QString> listLabels;
        listLabels.append("");

        listLabels.append(getStructList(N_ELF_LIBRARIES::records, N_ELF_LIBRARIES::__data_size));

        QList<QString> listLibraries = g_pELF->getLibraries();

        qint32 nNumberOfLibraries = listLibraries.count();

        *g_ppModel = new QStandardItemModel(nNumberOfLibraries, listLabels.count());

        setMaximum(nNumberOfLibraries);

        setTableHeader(*g_ppModel, &listLabels);

        for (qint32 i = 0; (i < nNumberOfLibraries) && (isRun()); i++) {
            QStandardItem *pItem = new QStandardItem(QString::number(i));

            pItem->setData(g_nStringTableOffset, Qt::UserRole + FW_DEF::SECTION_DATA_STRINGTABLEOFFSET);
            pItem->setData(g_nStringTableSize, Qt::UserRole + FW_DEF::SECTION_DATA_STRINGTABLESIZE);

            (*g_ppModel)->setItem(i, 0, pItem);
            (*g_ppModel)->setItem(i, N_ELF_LIBRARIES::library_name + 1, new QStandardItem(listLibraries.at(i)));

            incValue();
        }
    } else if (g_nType == SELF::TYPE_Elf_Rela) {
        bool bIs64 = g_pELF->is64();

        QList<QString> listLabels;
        listLabels.append("");

        if (bIs64) {
            listLabels.append(getStructList(N_Elf_Rela::records64, N_Elf_Rela::__data_size));
        } else {
            listLabels.append(getStructList(N_Elf_Rela::records32, N_Elf_Rela::__data_size));
        }

        listLabels.append("Sym");
        listLabels.append(tr("Type"));

        QList<XELF_DEF::Elf64_Rela> listRela64;
        QList<XELF_DEF::Elf32_Rela> listRela32;

        qint32 nNumberOfRels = 0;

        if (bIs64) {
            listRela64 = g_pELF->getElf64_RelaList(g_nOffset, g_nSize);
            nNumberOfRels = listRela64.count();
        } else {
            listRela32 = g_pELF->getElf32_RelaList(g_nOffset, g_nSize);
            nNumberOfRels = listRela32.count();
        }

        *g_ppModel = new QStandardItemModel(nNumberOfRels, listLabels.count());

        setMaximum(nNumberOfRels);

        setTableHeader(*g_ppModel, &listLabels);

        QMap<quint64, QString> mapTypes;

        QString sArch = g_pELF->getArch();

        // TODO More Check!
        if ((sArch == "SPARC") || (sArch == "SPARC32PLUS") || (sArch == "SPARCV9"))  // TODO Check!
        {
            mapTypes = g_pELF->getRelTypesS_SPARC();
        } else if (sArch == "386") {
            mapTypes = g_pELF->getRelTypesS_x86();
        } else if ((sArch == "AMD64") || (sArch == "X86_64")) {
            mapTypes = g_pELF->getRelTypesS_x64();
        }

        for (qint32 i = 0; (i < nNumberOfRels) && (isRun()); i++) {
            if (bIs64) {
                QStandardItem *pItem = new QStandardItem(QString::number(i));

                pItem->setData(g_nOffset + i * sizeof(XELF_DEF::Elf64_Rela), Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET);
                pItem->setData(g_nOffset + i * sizeof(XELF_DEF::Elf64_Rela), Qt::UserRole + FW_DEF::SECTION_DATA_HEADEROFFSET);

                (*g_ppModel)->setItem(i, 0, pItem);
                (*g_ppModel)->setItem(i, N_Elf_Rela::r_offset + 1, new QStandardItem(XBinary::valueToHex(listRela64.at(i).r_offset)));
                (*g_ppModel)->setItem(i, N_Elf_Rela::r_info + 1, new QStandardItem(XBinary::valueToHex(listRela64.at(i).r_info)));
                (*g_ppModel)->setItem(i, N_Elf_Rela::r_addend + 1, new QStandardItem(XBinary::valueToHex(listRela64.at(i).r_addend)));
                (*g_ppModel)->setItem(i, N_Elf_Rela::r_addend + 2, new QStandardItem(XBinary::valueToHex((quint32)S_ELF64_R_SYM(listRela64.at(i).r_info))));
                (*g_ppModel)->setItem(i, N_Elf_Rela::r_addend + 3, new QStandardItem(mapTypes.value(S_ELF64_R_TYPE(listRela64.at(i).r_info))));
            } else {
                QStandardItem *pItem = new QStandardItem(QString::number(i));

                pItem->setData(g_nOffset + i * sizeof(XELF_DEF::Elf32_Rela), Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET);
                pItem->setData(g_nOffset + i * sizeof(XELF_DEF::Elf32_Rela), Qt::UserRole + FW_DEF::SECTION_DATA_HEADEROFFSET);

                (*g_ppModel)->setItem(i, 0, pItem);
                (*g_ppModel)->setItem(i, N_Elf_Rela::r_offset + 1, new QStandardItem(XBinary::valueToHex(listRela32.at(i).r_offset)));
                (*g_ppModel)->setItem(i, N_Elf_Rela::r_info + 1, new QStandardItem(XBinary::valueToHex(listRela32.at(i).r_info)));
                (*g_ppModel)->setItem(i, N_Elf_Rela::r_addend + 1, new QStandardItem(XBinary::valueToHex(listRela32.at(i).r_addend)));
                (*g_ppModel)->setItem(i, N_Elf_Rela::r_addend + 2, new QStandardItem(XBinary::valueToHex((quint32)S_ELF32_R_SYM(listRela32.at(i).r_info))));
                (*g_ppModel)->setItem(i, N_Elf_Rela::r_addend + 3, new QStandardItem(mapTypes.value(S_ELF32_R_TYPE(listRela32.at(i).r_info))));
            }

            incValue();
        }
    } else if (g_nType == SELF::TYPE_Elf_Rel) {
        bool bIs64 = g_pELF->is64();

        QList<QString> listLabels;
        listLabels.append("");

        if (bIs64) {
            listLabels.append(getStructList(N_Elf_Rel::records64, N_Elf_Rel::__data_size));
        } else {
            listLabels.append(getStructList(N_Elf_Rel::records32, N_Elf_Rel::__data_size));
        }

        listLabels.append("Sym");
        listLabels.append(tr("Type"));

        QList<XELF_DEF::Elf64_Rel> listRel64;
        QList<XELF_DEF::Elf32_Rel> listRel32;

        qint32 nNumberOfRels = 0;

        if (bIs64) {
            listRel64 = g_pELF->getElf64_RelList(g_nOffset, g_nSize);
            nNumberOfRels = listRel64.count();
        } else {
            listRel32 = g_pELF->getElf32_RelList(g_nOffset, g_nSize);
            nNumberOfRels = listRel32.count();
        }

        *g_ppModel = new QStandardItemModel(nNumberOfRels, listLabels.count());

        setMaximum(nNumberOfRels);

        setTableHeader(*g_ppModel, &listLabels);

        QMap<quint64, QString> mapTypes;

        QString sArch = g_pELF->getArch();

        if ((sArch == "SPARC") || (sArch == "SPARC32PLUS") || (sArch == "SPARCV9")) {
            mapTypes = g_pELF->getRelTypesS_SPARC();
        } else if (sArch == "386") {
            mapTypes = g_pELF->getRelTypesS_x86();
        } else if ((sArch == "AMD64") || (sArch == "X86_64")) {
            mapTypes = g_pELF->getRelTypesS_x64();
        }

        for (qint32 i = 0; (i < nNumberOfRels) && (isRun()); i++) {
            if (bIs64) {
                QStandardItem *pItem = new QStandardItem(QString::number(i));

                pItem->setData(g_nOffset + i * sizeof(XELF_DEF::Elf64_Rel), Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET);
                pItem->setData(g_nOffset + i * sizeof(XELF_DEF::Elf64_Rel), Qt::UserRole + FW_DEF::SECTION_DATA_HEADEROFFSET);

                (*g_ppModel)->setItem(i, 0, pItem);
                (*g_ppModel)->setItem(i, N_Elf_Rel::r_offset + 1, new QStandardItem(XBinary::valueToHex(listRel64.at(i).r_offset)));
                (*g_ppModel)->setItem(i, N_Elf_Rel::r_info + 1, new QStandardItem(XBinary::valueToHex(listRel64.at(i).r_info)));
                (*g_ppModel)->setItem(i, N_Elf_Rel::r_info + 2, new QStandardItem(XBinary::valueToHex((quint32)S_ELF64_R_SYM(listRel64.at(i).r_info))));
                (*g_ppModel)->setItem(i, N_Elf_Rel::r_info + 3, new QStandardItem(mapTypes.value(S_ELF64_R_TYPE(listRel64.at(i).r_info))));
            } else {
                QStandardItem *pItem = new QStandardItem(QString::number(i));

                pItem->setData(g_nOffset + i * sizeof(XELF_DEF::Elf32_Rel), Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET);
                pItem->setData(g_nOffset + i * sizeof(XELF_DEF::Elf32_Rel), Qt::UserRole + FW_DEF::SECTION_DATA_HEADEROFFSET);

                (*g_ppModel)->setItem(i, 0, pItem);
                (*g_ppModel)->setItem(i, N_Elf_Rel::r_offset + 1, new QStandardItem(XBinary::valueToHex(listRel32.at(i).r_offset)));
                (*g_ppModel)->setItem(i, N_Elf_Rel::r_info + 1, new QStandardItem(XBinary::valueToHex(listRel32.at(i).r_info)));
                (*g_ppModel)->setItem(i, N_Elf_Rel::r_info + 2, new QStandardItem(XBinary::valueToHex((quint32)S_ELF32_R_SYM(listRel32.at(i).r_info))));
                (*g_ppModel)->setItem(i, N_Elf_Rel::r_info + 3, new QStandardItem(mapTypes.value(S_ELF32_R_TYPE(listRel32.at(i).r_info))));
            }

            incValue();
        }
    }

    adjustModel(*g_ppModel);
}

void ELFProcessData::ajustTableView(QWidget *pWidget, QTableView *pTableView)
{
    XBinary::MODE mode = g_pELF->getMode();

    if (g_nType == SELF::TYPE_SYMBOLTABLE) {
        if (g_pELF->is64()) {
            XOptions::setTableViewHeaderWidth(pTableView, 0, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT16, mode));
            XOptions::setTableViewHeaderWidth(pTableView, N_Elf64_Sym::st_name + 1, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
            XOptions::setTableViewHeaderWidth(pTableView, N_Elf64_Sym::st_info + 1, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT8, mode));
            XOptions::setTableViewHeaderWidth(pTableView, N_Elf64_Sym::st_other + 1, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT8, mode));
            XOptions::setTableViewHeaderWidth(pTableView, N_Elf64_Sym::st_shndx + 1, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT16, mode));
            XOptions::setTableViewHeaderWidth(pTableView, N_Elf64_Sym::st_value + 1, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINTMODE, mode));
            XOptions::setTableViewHeaderWidth(pTableView, N_Elf64_Sym::st_size + 1, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINTMODE, mode));
            XOptions::setTableViewHeaderWidth(pTableView, N_Elf64_Sym::st_size + 2, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_STRINGLONG, mode));
        } else {
            XOptions::setTableViewHeaderWidth(pTableView, 0, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT16, mode));
            XOptions::setTableViewHeaderWidth(pTableView, N_Elf32_Sym::st_name + 1, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
            XOptions::setTableViewHeaderWidth(pTableView, N_Elf32_Sym::st_value + 1, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINTMODE, mode));
            XOptions::setTableViewHeaderWidth(pTableView, N_Elf32_Sym::st_size + 1, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINTMODE, mode));
            XOptions::setTableViewHeaderWidth(pTableView, N_Elf32_Sym::st_info + 1, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT8, mode));
            XOptions::setTableViewHeaderWidth(pTableView, N_Elf32_Sym::st_other + 1, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT8, mode));
            XOptions::setTableViewHeaderWidth(pTableView, N_Elf32_Sym::st_shndx + 1, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT16, mode));
            XOptions::setTableViewHeaderWidth(pTableView, N_Elf32_Sym::st_shndx + 2, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_STRINGLONG, mode));
        }
    } else if (g_nType == SELF::TYPE_Elf_Shdr) {
        XOptions::setTableViewHeaderWidth(pTableView, 0, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT16, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 1, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 2, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 3, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINTMODE, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 4, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINTMODE, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 5, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINTMODE, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 6, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINTMODE, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 7, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 8, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 9, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINTMODE, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 10, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINTMODE, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 11, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_STRINGMID, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 12, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_STRINGSHORT, mode));
    } else if (g_nType == SELF::TYPE_Elf_Phdr) {
        XOptions::setTableViewHeaderWidth(pTableView, 0, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT16, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 1, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 2, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 3, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINTMODE, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 4, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINTMODE, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 5, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINTMODE, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 6, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINTMODE, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 7, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINTMODE, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 8, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINTMODE, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 9, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINTMODE, mode));
    } else if (g_nType == SELF::TYPE_Elf_DynamicArrayTags) {
        XOptions::setTableViewHeaderWidth(pTableView, 0, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT16, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 1, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINTMODE, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 2, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINTMODE, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 3, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_STRINGMID, mode));
    } else if (g_nType == SELF::TYPE_NOTES) {
        XOptions::setTableViewHeaderWidth(pTableView, 0, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT16, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 1, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 2, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_STRINGLONG, mode));
    } else if (g_nType == SELF::TYPE_LIBRARIES) {
        XOptions::setTableViewHeaderWidth(pTableView, 0, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT16, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 1, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_STRINGLONG, mode));
    } else if (g_nType == SELF::TYPE_Elf_Rela) {
        XOptions::setTableViewHeaderWidth(pTableView, 0, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT16, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 1, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINTMODE, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 2, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINTMODE, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 3, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINTMODE, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 4, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 5, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_STRINGMID, mode));
    } else if (g_nType == SELF::TYPE_Elf_Rel) {
        XOptions::setTableViewHeaderWidth(pTableView, 0, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT16, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 1, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINTMODE, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 2, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINTMODE, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 3, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 4, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_STRINGMID, mode));
    }
}

void ELFProcessData::adjustModel(QStandardItemModel *pModel)
{
    if (g_nType == SELF::TYPE_SYMBOLTABLE) {
        if (g_pELF->is64()) {
            XOptions::setModelTextAlignment(pModel, 0, Qt::AlignRight | Qt::AlignVCenter);
            XOptions::setModelTextAlignment(pModel, N_Elf64_Sym::st_name + 1, Qt::AlignRight | Qt::AlignVCenter);
            XOptions::setModelTextAlignment(pModel, N_Elf64_Sym::st_info + 1, Qt::AlignRight | Qt::AlignVCenter);
            XOptions::setModelTextAlignment(pModel, N_Elf64_Sym::st_other + 1, Qt::AlignRight | Qt::AlignVCenter);
            XOptions::setModelTextAlignment(pModel, N_Elf64_Sym::st_shndx + 1, Qt::AlignRight | Qt::AlignVCenter);
            XOptions::setModelTextAlignment(pModel, N_Elf64_Sym::st_value + 1, Qt::AlignRight | Qt::AlignVCenter);
            XOptions::setModelTextAlignment(pModel, N_Elf64_Sym::st_size + 1, Qt::AlignRight | Qt::AlignVCenter);
            XOptions::setModelTextAlignment(pModel, N_Elf64_Sym::st_size + 2, Qt::AlignLeft | Qt::AlignVCenter);
        } else {
            XOptions::setModelTextAlignment(pModel, 0, Qt::AlignRight | Qt::AlignVCenter);
            XOptions::setModelTextAlignment(pModel, N_Elf32_Sym::st_name + 1, Qt::AlignRight | Qt::AlignVCenter);
            XOptions::setModelTextAlignment(pModel, N_Elf32_Sym::st_value + 1, Qt::AlignRight | Qt::AlignVCenter);
            XOptions::setModelTextAlignment(pModel, N_Elf32_Sym::st_size + 1, Qt::AlignRight | Qt::AlignVCenter);
            XOptions::setModelTextAlignment(pModel, N_Elf32_Sym::st_info + 1, Qt::AlignRight | Qt::AlignVCenter);
            XOptions::setModelTextAlignment(pModel, N_Elf32_Sym::st_other + 1, Qt::AlignRight | Qt::AlignVCenter);
            XOptions::setModelTextAlignment(pModel, N_Elf32_Sym::st_shndx + 1, Qt::AlignRight | Qt::AlignVCenter);
            XOptions::setModelTextAlignment(pModel, N_Elf32_Sym::st_shndx + 2, Qt::AlignLeft | Qt::AlignVCenter);
        }
    } else if (g_nType == SELF::TYPE_Elf_Shdr) {
        XOptions::setModelTextAlignment(pModel, 0, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 1, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 2, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 3, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 4, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 5, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 6, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 7, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 8, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 9, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 10, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 11, Qt::AlignLeft | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 12, Qt::AlignLeft | Qt::AlignVCenter);
    } else if (g_nType == SELF::TYPE_Elf_Phdr) {
        XOptions::setModelTextAlignment(pModel, 0, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 1, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 2, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 3, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 4, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 5, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 6, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 7, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 8, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 9, Qt::AlignRight | Qt::AlignVCenter);
    } else if (g_nType == SELF::TYPE_Elf_DynamicArrayTags) {
        XOptions::setModelTextAlignment(pModel, 0, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 1, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 2, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 3, Qt::AlignLeft | Qt::AlignVCenter);
    } else if (g_nType == SELF::TYPE_NOTES) {
        XOptions::setModelTextAlignment(pModel, 0, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 1, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 2, Qt::AlignLeft | Qt::AlignVCenter);
    } else if (g_nType == SELF::TYPE_LIBRARIES) {
        XOptions::setModelTextAlignment(pModel, 0, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 1, Qt::AlignLeft | Qt::AlignVCenter);
    } else if (g_nType == SELF::TYPE_Elf_Rela) {
        XOptions::setModelTextAlignment(pModel, 0, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 1, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 2, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 3, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 4, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 5, Qt::AlignLeft | Qt::AlignVCenter);
    } else if (g_nType == SELF::TYPE_Elf_Rel) {
        XOptions::setModelTextAlignment(pModel, 0, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 1, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 2, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 3, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment(pModel, 4, Qt::AlignLeft | Qt::AlignVCenter);
    }
}

/* Copyright (c) 2017-2025 hors<horsicq@gmail.com>
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
#include "xprocessdata.h"

XProcessData::XProcessData()
{
    g_pPdStruct = nullptr;
    g_ppModel = nullptr;
    g_pCwOptions = nullptr;
    g_nFreeIndex = -1;
}

void XProcessData::setData(QStandardItemModel **ppModel, QList<XFW_DEF::HEADER_RECORD> *pListHeaderRecords, XFW_DEF::CWOPTIONS *pCwOptions, XBinary::PDSTRUCT *pPdStruct)
{
    g_ppModel = ppModel;
    g_pListHeaderRecords = pListHeaderRecords;
    g_pCwOptions = pCwOptions;
    g_pPdStruct = pPdStruct;
}

void XProcessData::process()
{
    QElapsedTimer scanTimer;
    scanTimer.start();

    g_nFreeIndex = XBinary::getFreeIndex(g_pPdStruct);
    XBinary::setPdStructInit(g_pPdStruct, g_nFreeIndex, 0);

    *g_pListHeaderRecords = XFormatWidget::getHeaderRecords(g_pCwOptions, -1);

    qint32 nHeaderSize = XFormatWidget::getHeaderSize(g_pListHeaderRecords);
    qint32 nNumberOfColumns = g_pListHeaderRecords->count();
    qint32 nNumberOfRows = 0;

    if (nHeaderSize == 0) {
        nHeaderSize = 1;
    }

    nNumberOfRows = g_pCwOptions->nDataCount;

    if (nNumberOfRows == 0) {
        nNumberOfRows = (g_pCwOptions->nDataSize) / nHeaderSize;
    }

    if (g_pCwOptions->_type == XFW_DEF::TYPE_MACH_load_command) {
        XMACH mach(g_pCwOptions->pDevice, g_pCwOptions->bIsImage, g_pCwOptions->nImageBase);

        QMap<quint64, QString> mapLC = mach.getLoadCommandTypes();

        QList<XMACH::COMMAND_RECORD> listCommands =
            mach._getCommandRecords(g_pCwOptions->nDataOffset, g_pCwOptions->nDataSize, g_pCwOptions->nDataCount, (g_pCwOptions->mode == XBinary::MODE_64),
                                    (g_pCwOptions->endian == XBinary::ENDIAN_BIG), 0, g_pPdStruct);

        nNumberOfRows = listCommands.count();
        XBinary::setPdStructTotal(g_pPdStruct, g_nFreeIndex, nNumberOfRows);

        (*g_ppModel) = new QStandardItemModel(nNumberOfRows, nNumberOfColumns);

        for (qint32 i = 0; (i < nNumberOfRows) && XBinary::isPdStructNotCanceled(g_pPdStruct); i++) {
            XFW_DEF::TYPE _type = XFormatWidget::load_commandIdToType(listCommands.at(i).nId);

            XFormatWidget::setItemToModelData((*g_ppModel), i, 0, i, 0, g_pListHeaderRecords->at(0).vtype, _type, listCommands.at(i).nStructOffset, nHeaderSize, 0, 0, 0,
                                              g_pCwOptions->demangleMode);
            XFormatWidget::setItemToModel((*g_ppModel), i, XTYPE_MACH::X_load_commands::cmd + 1, listCommands.at(i).nId,
                                          g_pListHeaderRecords->at(XTYPE_MACH::X_load_commands::cmd + 1).nSize,
                                          g_pListHeaderRecords->at(XTYPE_MACH::X_load_commands::cmd + 1).vtype, g_pCwOptions->demangleMode);
            XFormatWidget::setItemToModel((*g_ppModel), i, XTYPE_MACH::X_load_commands::cmdsize + 1, listCommands.at(i).nSize,
                                          g_pListHeaderRecords->at(XTYPE_MACH::X_load_commands::cmdsize + 1).nSize,
                                          g_pListHeaderRecords->at(XTYPE_MACH::X_load_commands::cmdsize + 1).vtype, g_pCwOptions->demangleMode);
            XFormatWidget::setItemToModel((*g_ppModel), i, XTYPE_MACH::X_load_commands::cmdsize + 2, mapLC.value(listCommands.at(i).nId),
                                          g_pListHeaderRecords->at(XTYPE_MACH::X_load_commands::cmdsize + 2).nSize,
                                          g_pListHeaderRecords->at(XTYPE_MACH::X_load_commands::cmdsize + 2).vtype, g_pCwOptions->demangleMode);

            XBinary::setPdStructCurrent(g_pPdStruct, g_nFreeIndex, i);
        }
    } else if (g_pCwOptions->_type == XFW_DEF::TYPE_MACH_functions) {
        XMACH mach(g_pCwOptions->pDevice, g_pCwOptions->bIsImage, g_pCwOptions->nImageBase);

        QList<XMACH::FUNCTION_RECORD> listFunctions = mach.getFunctionRecords(g_pCwOptions->nDataOffset, g_pCwOptions->nDataSize);

        nNumberOfRows = listFunctions.count();
        XBinary::setPdStructTotal(g_pPdStruct, g_nFreeIndex, nNumberOfRows);

        (*g_ppModel) = new QStandardItemModel(nNumberOfRows, nNumberOfColumns);

        for (qint32 i = 0; (i < nNumberOfRows) && XBinary::isPdStructNotCanceled(g_pPdStruct); i++) {
            XFormatWidget::setItemToModelData((*g_ppModel), i, 0, i, 0, g_pListHeaderRecords->at(0).vtype, XFW_DEF::TYPE_UNKNOWN, listFunctions.at(i).nDataOffset,
                                              listFunctions.at(i).nDataSize, listFunctions.at(i).nFunctionOffset, 0, 0, g_pCwOptions->demangleMode);
            XFormatWidget::setItemToModel((*g_ppModel), i, 1, listFunctions.at(i).nValue, g_pListHeaderRecords->at(1).nSize, g_pListHeaderRecords->at(1).vtype,
                                          g_pCwOptions->demangleMode);
            XFormatWidget::setItemToModel((*g_ppModel), i, 2, listFunctions.at(i).nFunctionOffset, g_pListHeaderRecords->at(2).nSize, g_pListHeaderRecords->at(2).vtype,
                                          g_pCwOptions->demangleMode);

            XBinary::setPdStructCurrent(g_pPdStruct, g_nFreeIndex, i);
        }
    } else if ((g_pCwOptions->_type == XFW_DEF::TYPE_MACH_nlist) || (g_pCwOptions->_type == XFW_DEF::TYPE_MACH_nlist_64)) {
        XMACH mach(g_pCwOptions->pDevice, g_pCwOptions->bIsImage, g_pCwOptions->nImageBase);

        XBinary::setPdStructTotal(g_pPdStruct, g_nFreeIndex, nNumberOfRows);

        QByteArray baStringTable = mach.read_array(g_pCwOptions->var1.toLongLong(), g_pCwOptions->var2.toULongLong());
        char *pBuffer = baStringTable.data();
        qint64 nBufferSize = baStringTable.size();

        (*g_ppModel) = new QStandardItemModel(nNumberOfRows, nNumberOfColumns);

        qint64 _nOffset = g_pCwOptions->nDataOffset;

        for (qint32 i = 0; (i < nNumberOfRows) && XBinary::isPdStructNotCanceled(g_pPdStruct); i++) {
            quint64 nValue = 0;
            for (qint32 j = 0; j < nNumberOfColumns - 1; j++) {
                quint32 vtype = g_pListHeaderRecords->at(j).vtype;
                QVariant var = XFormatWidget::_readVariant(&mach, _nOffset + g_pListHeaderRecords->at(j).nOffset, g_pListHeaderRecords->at(j).nSize, vtype,
                                                           (g_pCwOptions->endian == XBinary::ENDIAN_BIG));
                if (j == 0) {
                    XFormatWidget::setItemToModelData((*g_ppModel), i, j, i, 0, vtype, g_pCwOptions->_type, _nOffset, nHeaderSize, 0, 0, 0, g_pCwOptions->demangleMode);
                } else {
                    XFormatWidget::setItemToModel((*g_ppModel), i, j, var, g_pListHeaderRecords->at(j).nSize, vtype, g_pCwOptions->demangleMode);
                }

                if (j == 1) {
                    nValue = var.toULongLong();
                }
            }

            QString _sSymbolName = mach._read_ansiString_safe(pBuffer, nBufferSize, nValue, 256);

            XFormatWidget::setItemToModel((*g_ppModel), i, nNumberOfColumns - 1, _sSymbolName, g_pListHeaderRecords->at(nNumberOfColumns - 1).nSize,
                                          g_pListHeaderRecords->at(nNumberOfColumns - 1).vtype, g_pCwOptions->demangleMode);

            _nOffset += nHeaderSize;
            XBinary::setPdStructCurrent(g_pPdStruct, g_nFreeIndex, i);
        }
    } else if (g_pCwOptions->_type == XFW_DEF::TYPE_MACH_trie_export_table) {
        XMACH mach(g_pCwOptions->pDevice, g_pCwOptions->bIsImage, g_pCwOptions->nImageBase);

        QList<XMACH::EXPORT_RECORD> listExportRecords;

        mach.handleImport(g_pCwOptions->nDataOffset, 0, g_pCwOptions->nDataSize, &listExportRecords, "", g_pPdStruct);

        nNumberOfRows = listExportRecords.count();

        (*g_ppModel) = new QStandardItemModel(nNumberOfRows, nNumberOfColumns);

        for (qint32 i = 0; (i < nNumberOfRows) && XBinary::isPdStructNotCanceled(g_pPdStruct); i++) {
            XFormatWidget::setItemToModelData((*g_ppModel), i, 0, i, 0, g_pListHeaderRecords->at(0).vtype, XFW_DEF::TYPE_UNKNOWN, 0, 0, 0, 0, 0,
                                              g_pCwOptions->demangleMode);
            XFormatWidget::setItemToModel((*g_ppModel), i, 1, listExportRecords.at(i).nOffset, 0, g_pListHeaderRecords->at(1).vtype, g_pCwOptions->demangleMode);
            XFormatWidget::setItemToModel((*g_ppModel), i, 2, listExportRecords.at(i).nFlags, 0, g_pListHeaderRecords->at(2).vtype, g_pCwOptions->demangleMode);
            XFormatWidget::setItemToModel((*g_ppModel), i, 3, listExportRecords.at(i).sName, 0, g_pListHeaderRecords->at(3).vtype, g_pCwOptions->demangleMode);

            XBinary::setPdStructCurrent(g_pPdStruct, g_nFreeIndex, i);
        }
    } else if (g_pCwOptions->_type == XFW_DEF::TYPE_GENERIC_STRINGTABLE_ANSI) {
        XBinary binary(g_pCwOptions->pDevice, g_pCwOptions->bIsImage, g_pCwOptions->nImageBase);

        qint64 _nOffset = g_pCwOptions->nDataOffset;
        qint64 _nSize = g_pCwOptions->nDataSize;

        QList<XBinary::STRINGTABLE_RECORD> listSTrecords = binary.getStringTable_ANSI(_nOffset, _nSize, g_pPdStruct);

        nNumberOfRows = listSTrecords.count();

        XBinary::setPdStructTotal(g_pPdStruct, g_nFreeIndex, nNumberOfRows);
        (*g_ppModel) = new QStandardItemModel(nNumberOfRows, nNumberOfColumns);

        for (qint32 i = 0; (i < nNumberOfRows) && XBinary::isPdStructNotCanceled(g_pPdStruct); i++) {
            XFormatWidget::setItemToModelData((*g_ppModel), i, 0, i, 0, g_pListHeaderRecords->at(0).vtype, XFW_DEF::TYPE_UNKNOWN,
                                              listSTrecords.at(i).nOffsetFromStart + _nOffset, listSTrecords.at(i).nSizeInBytes, 0, 0, 0, g_pCwOptions->demangleMode);
            XFormatWidget::setItemToModel((*g_ppModel), i, 1, listSTrecords.at(i).nOffsetFromStart, 0, g_pListHeaderRecords->at(1).vtype, g_pCwOptions->demangleMode);
            XFormatWidget::setItemToModel((*g_ppModel), i, 2, listSTrecords.at(i).sString, listSTrecords.at(i).nSizeInBytes, g_pListHeaderRecords->at(2).vtype,
                                          g_pCwOptions->demangleMode);

            XBinary::setPdStructCurrent(g_pPdStruct, g_nFreeIndex, i);
        }
    } else if (g_pCwOptions->_type == XFW_DEF::TYPE_PE_IMAGE_DATA_DIRECTORY) {
        XPE xpe(g_pCwOptions->pDevice, g_pCwOptions->bIsImage, g_pCwOptions->nImageBase);
        XBinary::_MEMORY_MAP memoryMap = xpe.getMemoryMap(XBinary::MAPMODE_SECTIONS, g_pPdStruct);

        XBinary::setPdStructTotal(g_pPdStruct, g_nFreeIndex, nNumberOfRows);

        QMap<quint64, QString> mapOHDD = XPE::getImageOptionalHeaderDataDirectoryS();

        (*g_ppModel) = new QStandardItemModel(nNumberOfRows, nNumberOfColumns);

        qint64 _nOffset = g_pCwOptions->nDataOffset;

        for (qint32 i = 0; (i < nNumberOfRows) && XBinary::isPdStructNotCanceled(g_pPdStruct); i++) {
            XPE_DEF::IMAGE_DATA_DIRECTORY idd = xpe.read_IMAGE_DATA_DIRECTORY(_nOffset);

            XADDR nDataOffset = 0;
            qint64 nDataSize = idd.Size;

            if (i == XPE_DEF::S_IMAGE_DIRECTORY_ENTRY_SECURITY) {
                nDataOffset = idd.VirtualAddress;
            } else {
                nDataOffset = XBinary::relAddressToOffset(&memoryMap, idd.VirtualAddress);
            }

            if (!XBinary::isOffsetAndSizeValid(&memoryMap, nDataOffset, nDataSize)) {
                nDataOffset = 0;
                nDataSize = 0;
            }

            XFormatWidget::setItemToModelData((*g_ppModel), i, 0, i, 0, g_pListHeaderRecords->at(0).vtype, g_pCwOptions->_type, _nOffset, nHeaderSize, nDataOffset,
                                              nDataSize, 0, g_pCwOptions->demangleMode);
            XFormatWidget::setItemToModel((*g_ppModel), i, 1, idd.VirtualAddress, g_pListHeaderRecords->at(1).nSize, g_pListHeaderRecords->at(1).vtype,
                                          g_pCwOptions->demangleMode);
            XFormatWidget::setItemToModel((*g_ppModel), i, 2, idd.Size, g_pListHeaderRecords->at(2).nSize, g_pListHeaderRecords->at(2).vtype, g_pCwOptions->demangleMode);
            XFormatWidget::setItemToModel((*g_ppModel), i, 3, mapOHDD.value(i), g_pListHeaderRecords->at(3).nSize, g_pListHeaderRecords->at(3).vtype,
                                          g_pCwOptions->demangleMode);

            _nOffset += nHeaderSize;
            XBinary::setPdStructCurrent(g_pPdStruct, g_nFreeIndex, i);
        }
    } else if (g_pCwOptions->_type == XFW_DEF::TYPE_PE_IMAGE_SECTION_HEADER) {
        XPE xpe(g_pCwOptions->pDevice, g_pCwOptions->bIsImage, g_pCwOptions->nImageBase);
        XBinary::_MEMORY_MAP memoryMap = xpe.getMemoryMap(XBinary::MAPMODE_SECTIONS, g_pPdStruct);

        XBinary::setPdStructTotal(g_pPdStruct, g_nFreeIndex, nNumberOfRows);

        (*g_ppModel) = new QStandardItemModel(nNumberOfRows, nNumberOfColumns);

        qint64 _nOffset = g_pCwOptions->nDataOffset;

        for (qint32 i = 0; (i < nNumberOfRows) && XBinary::isPdStructNotCanceled(g_pPdStruct); i++) {
            XPE_DEF::IMAGE_SECTION_HEADER ish = xpe.read_IMAGE_SECTION_HEADER(_nOffset);

            XADDR nDataOffset = ish.PointerToRawData;
            qint64 nDataSize = ish.SizeOfRawData;

            if (!XBinary::isOffsetAndSizeValid(&memoryMap, nDataOffset, nDataSize)) {
                nDataOffset = 0;
                nDataSize = 0;
            }

            QString sName = QString((char *)ish.Name);
            sName.resize(qMin(sName.length(), XPE_DEF::S_IMAGE_SIZEOF_SHORT_NAME));

            XFormatWidget::setItemToModelData((*g_ppModel), i, 0, i + 1, 0, g_pListHeaderRecords->at(0).vtype, g_pCwOptions->_type, _nOffset, nHeaderSize, nDataOffset,
                                              nDataSize, 0, g_pCwOptions->demangleMode);
            XFormatWidget::setItemToModel((*g_ppModel), i, 1, sName, g_pListHeaderRecords->at(1).nSize, g_pListHeaderRecords->at(1).vtype, g_pCwOptions->demangleMode);
            XFormatWidget::setItemToModel((*g_ppModel), i, 2, ish.Misc.VirtualSize, g_pListHeaderRecords->at(2).nSize, g_pListHeaderRecords->at(2).vtype,
                                          g_pCwOptions->demangleMode);
            XFormatWidget::setItemToModel((*g_ppModel), i, 3, ish.VirtualAddress, g_pListHeaderRecords->at(3).nSize, g_pListHeaderRecords->at(3).vtype,
                                          g_pCwOptions->demangleMode);
            XFormatWidget::setItemToModel((*g_ppModel), i, 4, ish.SizeOfRawData, g_pListHeaderRecords->at(4).nSize, g_pListHeaderRecords->at(4).vtype,
                                          g_pCwOptions->demangleMode);
            XFormatWidget::setItemToModel((*g_ppModel), i, 5, ish.PointerToRawData, g_pListHeaderRecords->at(5).nSize, g_pListHeaderRecords->at(5).vtype,
                                          g_pCwOptions->demangleMode);
            XFormatWidget::setItemToModel((*g_ppModel), i, 6, ish.PointerToRelocations, g_pListHeaderRecords->at(6).nSize, g_pListHeaderRecords->at(6).vtype,
                                          g_pCwOptions->demangleMode);
            XFormatWidget::setItemToModel((*g_ppModel), i, 7, ish.PointerToLinenumbers, g_pListHeaderRecords->at(7).nSize, g_pListHeaderRecords->at(7).vtype,
                                          g_pCwOptions->demangleMode);
            XFormatWidget::setItemToModel((*g_ppModel), i, 8, ish.NumberOfRelocations, g_pListHeaderRecords->at(8).nSize, g_pListHeaderRecords->at(8).vtype,
                                          g_pCwOptions->demangleMode);
            XFormatWidget::setItemToModel((*g_ppModel), i, 9, ish.NumberOfLinenumbers, g_pListHeaderRecords->at(9).nSize, g_pListHeaderRecords->at(9).vtype,
                                          g_pCwOptions->demangleMode);
            XFormatWidget::setItemToModel((*g_ppModel), i, 10, ish.Characteristics, g_pListHeaderRecords->at(10).nSize, g_pListHeaderRecords->at(10).vtype,
                                          g_pCwOptions->demangleMode);

            _nOffset += nHeaderSize;
            XBinary::setPdStructCurrent(g_pPdStruct, g_nFreeIndex, i);
        }
    } else if ((g_pCwOptions->_type == XFW_DEF::TYPE_7ZIP_PROPERTIES) || (g_pCwOptions->_type == XFW_DEF::TYPE_MACH_bind) ||
               (g_pCwOptions->_type == XFW_DEF::TYPE_MACH_weak) || (g_pCwOptions->_type == XFW_DEF::TYPE_MACH_lazy_bind) ||
               (g_pCwOptions->_type == XFW_DEF::TYPE_MACH_rebase) || (g_pCwOptions->_type == XFW_DEF::TYPE_MACH_trie_export_commands)) {
        XBinary binary(g_pCwOptions->pDevice, g_pCwOptions->bIsImage, g_pCwOptions->nImageBase);

        QByteArray baData = binary.read_array(g_pCwOptions->nDataOffset, g_pCwOptions->nDataSize, g_pPdStruct);

        QList<XDisasmAbstract::DISASM_RESULT> listOpcodes;

        XDisasmCore disasmCore;

        if (g_pCwOptions->_type == XFW_DEF::TYPE_7ZIP_PROPERTIES) {
            disasmCore.setMode(XBinary::DM_CUSTOM_7ZIP_PROPERTIES);
        } else if ((g_pCwOptions->_type == XFW_DEF::TYPE_MACH_bind) || (g_pCwOptions->_type == XFW_DEF::TYPE_MACH_lazy_bind)) {
            disasmCore.setMode(XBinary::DM_CUSTOM_MACH_BIND);
        } else if (g_pCwOptions->_type == XFW_DEF::TYPE_MACH_weak) {
            disasmCore.setMode(XBinary::DM_CUSTOM_MACH_WEAK);
        } else if (g_pCwOptions->_type == XFW_DEF::TYPE_MACH_rebase) {
            disasmCore.setMode(XBinary::DM_CUSTOM_MACH_REBASE);
        } else if (g_pCwOptions->_type == XFW_DEF::TYPE_MACH_trie_export_commands) {
            disasmCore.setMode(XBinary::DM_CUSTOM_MACH_EXPORT);
        }

        listOpcodes = disasmCore.disAsmList(baData.data(), baData.size(), g_pCwOptions->nDataOffset, XDisasmAbstract::DISASM_OPTIONS(), 0, g_pPdStruct);

        nNumberOfRows = listOpcodes.count();

        XBinary::setPdStructTotal(g_pPdStruct, g_nFreeIndex, nNumberOfRows);

        (*g_ppModel) = new QStandardItemModel(nNumberOfRows, nNumberOfColumns);

        qint64 _nOffset = g_pCwOptions->nDataOffset;

        for (qint32 i = 0; (i < nNumberOfRows) && XBinary::isPdStructNotCanceled(g_pPdStruct); i++) {
            XFormatWidget::setItemToModelData((*g_ppModel), i, 0, i, 0, g_pListHeaderRecords->at(0).vtype, XFW_DEF::TYPE_UNKNOWN, _nOffset, listOpcodes.at(i).nSize, 0, 0,
                                              0, g_pCwOptions->demangleMode);
            XFormatWidget::setItemToModel((*g_ppModel), i, 1, _nOffset, g_pListHeaderRecords->at(1).nSize, g_pListHeaderRecords->at(1).vtype, g_pCwOptions->demangleMode);
            XFormatWidget::setItemToModel((*g_ppModel), i, 2, XDisasmAbstract::getOpcodeFullString(listOpcodes.at(i)), g_pListHeaderRecords->at(2).nSize,
                                          g_pListHeaderRecords->at(2).vtype, g_pCwOptions->demangleMode);

            _nOffset += listOpcodes.at(i).nSize;
            XBinary::setPdStructCurrent(g_pPdStruct, g_nFreeIndex, i);
        }

    } else {
        XBinary binary(g_pCwOptions->pDevice, g_pCwOptions->bIsImage, g_pCwOptions->nImageBase);
        XBinary::setPdStructTotal(g_pPdStruct, g_nFreeIndex, nNumberOfRows);

        (*g_ppModel) = new QStandardItemModel(nNumberOfRows, nNumberOfColumns);

        qint64 _nOffset = g_pCwOptions->nDataOffset;

        for (qint32 i = 0; (i < nNumberOfRows) && XBinary::isPdStructNotCanceled(g_pPdStruct); i++) {
            for (qint32 j = 0; j < nNumberOfColumns; j++) {
                QVariant var = XFormatWidget::_readVariant(&binary, _nOffset + g_pListHeaderRecords->at(j).nOffset, g_pListHeaderRecords->at(j).nSize,
                                                           g_pListHeaderRecords->at(j).vtype, (g_pCwOptions->endian == XBinary::ENDIAN_BIG));
                if (j == 0) {
                    XFormatWidget::setItemToModelData((*g_ppModel), i, j, i, 0, g_pListHeaderRecords->at(j).vtype, g_pCwOptions->_type, _nOffset, nHeaderSize, 0, 0, 0,
                                                      g_pCwOptions->demangleMode);
                } else {
                    XFormatWidget::setItemToModel((*g_ppModel), i, j, var, g_pListHeaderRecords->at(j).nSize, g_pListHeaderRecords->at(j).vtype,
                                                  g_pCwOptions->demangleMode);
                }
            }

            _nOffset += nHeaderSize;
            XBinary::setPdStructCurrent(g_pPdStruct, g_nFreeIndex, i);
        }
    }

    for (qint32 i = 0; i < nNumberOfColumns; i++) {
        //(*g_ppModel)->setHeaderData(i, Qt::Horizontal, (qint32)(Qt::AlignRight | Qt::AlignVCenter), Qt::TextAlignmentRole);
        (*g_ppModel)->setHorizontalHeaderItem(i, new QStandardItem(g_pListHeaderRecords->at(i).sName));
    }

    XBinary::setPdStructFinished(g_pPdStruct, g_nFreeIndex);

    emit completed(scanTimer.elapsed());
}

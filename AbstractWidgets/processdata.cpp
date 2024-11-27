/* Copyright (c) 2017-2024 hors<horsicq@gmail.com>
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
#include "processdata.h"

ProcessData::ProcessData()
{
    g_pPdStruct = nullptr;
    g_ppModel = nullptr;
    g_pCwOptions = nullptr;
    g_nFreeIndex = -1;
}

void ProcessData::setData(QStandardItemModel **ppModel, QList<FW_DEF::HEADER_RECORD> *pListHeaderRecords, FW_DEF::CWOPTIONS *pCwOptions, XBinary::PDSTRUCT *pPdStruct)
{
    g_ppModel = ppModel;
    g_pListHeaderRecords = pListHeaderRecords;
    g_pCwOptions = pCwOptions;
    g_pPdStruct = pPdStruct;
}

void ProcessData::process()
{
    QElapsedTimer scanTimer;
    scanTimer.start();

    g_nFreeIndex = XBinary::getFreeIndex(g_pPdStruct);
    XBinary::setPdStructInit(g_pPdStruct, g_nFreeIndex, 0);

    g_pListHeaderRecords->clear();

    {
        FW_DEF::HEADER_RECORD record={};
        record.nPosition = -1;
        record.sName = QString("#");
        record.vtype = FW_DEF::VAL_TYPE_NUMBER;

        g_pListHeaderRecords->append(record);
    }

    QList<FW_DEF::HEADER_RECORD> listHeaderRecords = FormatWidget::getHeaderRecords(g_pCwOptions);

    g_pListHeaderRecords->append(listHeaderRecords);

    if (g_pCwOptions->_type == FW_DEF::TYPE_MACH_commands) {
        FW_DEF::HEADER_RECORD record={};
        record.nPosition = -1;
        record.sName = tr("Info");
        record.vtype = FW_DEF::VAL_TYPE_INFO;

        g_pListHeaderRecords->append(record);
    }

    qint32 nHeaderSize = FormatWidget::getHeaderSize(&listHeaderRecords);
    qint32 nNumberOfColumns = g_pListHeaderRecords->count();
    qint32 nNumberOfRows = 0;

    if (g_pCwOptions->_type == FW_DEF::TYPE_MACH_commands) {
        XMACH mach(g_pCwOptions->pDevice, g_pCwOptions->bIsImage, g_pCwOptions->nImageBase);

        QMap<quint64, QString> mapLC = mach.getLoadCommandTypesS();

        QList<XMACH::COMMAND_RECORD> listCommands=mach._getCommandRecords(g_pCwOptions->nDataOffset, g_pCwOptions->nDataSize, g_pCwOptions->var1.toInt(),
                                                                         (g_pCwOptions->mode == XBinary::MODE_64), (g_pCwOptions->endian == XBinary::ENDIAN_BIG), 0, g_pPdStruct);

        nNumberOfRows = listCommands.count();
        XBinary::setPdStructTotal(g_pPdStruct, g_nFreeIndex, nNumberOfRows);

        (*g_ppModel) = new QStandardItemModel(nNumberOfRows, nNumberOfColumns);

        for (qint32 i = 0; (i < nNumberOfRows) && (!(g_pPdStruct->bIsStop)); i++) {
            {
                QStandardItem *pItem = new QStandardItem;
                pItem->setData(i, Qt::DisplayRole);
                pItem->setData(g_pCwOptions->_type, Qt::UserRole + FW_DEF::TABLEDATA_TYPE);
                pItem->setData(listCommands.at(i).nStructOffset, Qt::UserRole + FW_DEF::TABLEDATA_HEADEROFFSET);
                pItem->setData(nHeaderSize, Qt::UserRole + FW_DEF::TABLEDATA_HEADERSIZE);
                pItem->setData(listCommands.at(i).nStructOffset, Qt::UserRole + FW_DEF::TABLEDATA_DATAOFFSET);
                pItem->setData(listCommands.at(i).nSize + nHeaderSize, Qt::UserRole + FW_DEF::TABLEDATA_DATASIZE);
                (*g_ppModel)->setItem(i, 0, pItem);
            }
            {
                (*g_ppModel)->setItem(i, N_mach_commands::cmd + 1, new QStandardItem(XBinary::valueToHex((quint32)listCommands.at(i).nId)));
                (*g_ppModel)->setItem(i, N_mach_commands::cmdsize + 1, new QStandardItem(XBinary::valueToHex((quint32)listCommands.at(i).nSize)));
                (*g_ppModel)->setItem(i, N_mach_commands::cmdsize + 2, new QStandardItem(mapLC.value(listCommands.at(i).nId)));
            }

            XBinary::setPdStructCurrent(g_pPdStruct, g_nFreeIndex, i);
        }
    } else {
        XBinary binary(g_pCwOptions->pDevice, g_pCwOptions->bIsImage, g_pCwOptions->nImageBase);

        qint32 nNumberOfHeaderRecords = listHeaderRecords.count();

        nNumberOfRows = g_pCwOptions->var1.toInt();

        if (nNumberOfRows == 0) {
            nNumberOfRows = (g_pCwOptions->nDataSize) / nHeaderSize;
        }

        XBinary::setPdStructTotal(g_pPdStruct, g_nFreeIndex, nNumberOfRows);

        (*g_ppModel) = new QStandardItemModel(nNumberOfRows, nNumberOfColumns);

        qint64 _nOffset = g_pCwOptions->nDataOffset;

        for (qint32 i = 0; (i < nNumberOfRows) && (!(g_pPdStruct->bIsStop)); i++) {
            {
                QStandardItem *pItem = new QStandardItem;
                pItem->setData(i, Qt::DisplayRole);
                pItem->setData(_nOffset, Qt::UserRole + FW_DEF::TABLEDATA_HEADEROFFSET);
                pItem->setData(nHeaderSize, Qt::UserRole + FW_DEF::TABLEDATA_HEADERSIZE);
                pItem->setData(_nOffset, Qt::UserRole + FW_DEF::TABLEDATA_DATAOFFSET);
                pItem->setData(nHeaderSize, Qt::UserRole + FW_DEF::TABLEDATA_DATASIZE);
                (*g_ppModel)->setItem(i, 0, pItem);
            }

            for (qint32 j = 0; j < nNumberOfHeaderRecords; j ++) {
                QVariant var = FormatWidget::_readVariant(&binary, _nOffset + g_pListHeaderRecords->at(j).nOffset, g_pListHeaderRecords->at(j).nSize,
                                                          g_pListHeaderRecords->at(j).vtype, (g_pCwOptions->endian == XBinary::ENDIAN_BIG));
                (*g_ppModel)->setItem(i, j + 1, new QStandardItem(FormatWidget::_valueToString(var, g_pListHeaderRecords->at(j).nSize, g_pListHeaderRecords->at(j).vtype)));
            }

            _nOffset += nHeaderSize;

            XBinary::setPdStructCurrent(g_pPdStruct, g_nFreeIndex, i);
        }
    }

    for (qint32 i = 0; i < nNumberOfColumns; i++) {
        (*g_ppModel)->setHorizontalHeaderItem(i, new QStandardItem(g_pListHeaderRecords->at(i).sName));
    }

    XBinary::setPdStructFinished(g_pPdStruct, g_nFreeIndex);

    emit completed(scanTimer.elapsed());
}

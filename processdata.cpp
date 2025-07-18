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
#include "processdata.h"

ProcessData::ProcessData()
{
    g_pPdStruct = nullptr;
    g_pOptions = nullptr;
    g_nFreeIndex = -1;
}

void ProcessData::setPdStruct(XBinary::PDSTRUCT *pPdStruct)
{
    g_pPdStruct = pPdStruct;
}

void ProcessData::setOptions(XOptions *pOptions)
{
    g_pOptions = pOptions;
}

XOptions *ProcessData::getOptions()
{
    return g_pOptions;
}

void ProcessData::setMaximum(quint64 nMaximum)
{
    XBinary::setPdStructTotal(g_pPdStruct, g_nFreeIndex, nMaximum);
}

void ProcessData::incValue()
{
    XBinary::setPdStructCurrentIncrement(g_pPdStruct, g_nFreeIndex);
}

bool ProcessData::isRun()
{
    return !XBinary::isPdStructStopped(g_pPdStruct);
}

XBinary::PDSTRUCT *ProcessData::getPdStruct()
{
    return g_pPdStruct;
}

void ProcessData::ajustTableView(qint32 nType, QTableView *pTableView)
{
    Q_UNUSED(nType)

    qint32 nNumberOfColumns = pTableView->model()->columnCount();

    for (qint32 i = 0; i < nNumberOfColumns; i++) {
        XOptions::setTableViewHeaderWidth(pTableView, i, 120);  // TODO
    }
}

void ProcessData::ajustTreeView(qint32 nType, QTreeView *pTreeView)
{
    Q_UNUSED(nType)

    pTreeView->expand(pTreeView->model()->index(0, 0));
}

QList<QString> ProcessData::getStructList(const FW_DEF::HEADER_RECORD *pRecords, qint32 nRecordCount)
{
    QList<QString> listResult;

    for (qint32 i = 0; i < nRecordCount; i++) {
        listResult.append(pRecords[i].sName);
    }

    return listResult;
}

void ProcessData::setTableHeader(QStandardItemModel *pModel, QList<QString> *pListStrings)
{
    qint32 nNumberOfRecords = pListStrings->count();

    for (qint32 i = 0; i < nNumberOfRecords; i++) {
        pModel->setHeaderData(i, Qt::Horizontal, pListStrings->at(i));
    }
}

void ProcessData::setTreeHeader(QStandardItemModel *pModel, QList<QString> *pListStrings)
{
    pModel->setHorizontalHeaderLabels(*pListStrings);
}

void ProcessData::adjustModel(QStandardItemModel *pModel)
{
    Q_UNUSED(pModel)
}

void ProcessData::process()
{
    QElapsedTimer scanTimer;
    scanTimer.start();

    g_nFreeIndex = XBinary::getFreeIndex(g_pPdStruct);
    XBinary::setPdStructInit(g_pPdStruct, g_nFreeIndex, 0);

    _process();

    XBinary::setPdStructFinished(g_pPdStruct, g_nFreeIndex);

    emit completed(scanTimer.elapsed());
}

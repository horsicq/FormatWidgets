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
    return !(g_pPdStruct->bIsStop);
}

void ProcessData::setTableViewAdjust(QTableView *pTableView, qint32 nColumn, Qt::Alignment flag, qint32 nWidth)
{
    XOptions::setModelTextAlignment(pTableView->model(), nColumn, flag);

    QFont font = pTableView->font();
    font.setBold(true);

    const QFontMetricsF fm(font);

    QString sTitle = pTableView->model()->headerData(nColumn, Qt::Horizontal, Qt::DisplayRole).toString();

    qreal rWidth = fm.boundingRect(sTitle + "  ").width();

    nWidth = qMax(nWidth, (qint32)(rWidth));

    pTableView->setColumnWidth(nColumn, nWidth);
}

void ProcessData::setTreeViewAdjust(QTreeView *pTreeView, qint32 nColumn, Qt::Alignment flag, qint32 nWidth)
{
    XOptions::setModelTextAlignment(pTreeView->model(), nColumn, flag);

    pTreeView->setColumnWidth(nColumn, nWidth);
}

XBinary::PDSTRUCT *ProcessData::getPdStruct()
{
    return g_pPdStruct;
}

void ProcessData::ajustTreeView(QWidget *pWidget, QTreeView *pTreeView)
{
    Q_UNUSED(pWidget)

    pTreeView->expand(pTreeView->model()->index(0, 0));
}

QList<QString> ProcessData::getStructList(const FW_DEF::HEADER_RECORD *pRecords, int nRecordCount)
{
    QList<QString> listResult;

    for (qint32 i = 0; i < nRecordCount; i++) {
        listResult.append(pRecords[i].sName);
    }

    return listResult;
}

void ProcessData::setTableHeader(QStandardItemModel *pModel, QList<QString> *pListStrings)
{
    int nNumberOfRecords = pListStrings->count();

    for (qint32 i = 0; i < nNumberOfRecords; i++) {
        pModel->setHeaderData(i, Qt::Horizontal, pListStrings->at(i));
    }
}

void ProcessData::setTreeHeader(QStandardItemModel *pModel, QList<QString> *pListStrings)
{
    pModel->setHorizontalHeaderLabels(*pListStrings);
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

/* Copyright (c) 2017-2022 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
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
    g_bIsStop=false;
}

void ProcessData::stop()
{
    g_bIsStop=true;
}

void ProcessData::setMaximum(quint64 nMaximum)
{
    this->g_nMaximum=nMaximum;
    g_nProcent=nMaximum/100;
    g_nCurrentProcent=0;
    g_nValue=0;
}

void ProcessData::incValue()
{
    g_nValue++;

    if(g_nValue>((g_nCurrentProcent+1)*g_nProcent))
    {
        g_nCurrentProcent++;
        emit progressValue(g_nCurrentProcent);
    }
}

bool ProcessData::isRun()
{
    return !(g_bIsStop);
}

void ProcessData::ajustTreeView(QWidget *pWidget,QTreeView *pTreeView)
{
    Q_UNUSED(pWidget)

    pTreeView->expand(pTreeView->model()->index(0,0));
}

QList<QString> ProcessData::getStructList(const FW_DEF::HEADER_RECORD *pRecords,int nRecordCount)
{
    QList<QString> listResult;

    for(qint32 i=0;i<nRecordCount;i++)
    {
        listResult.append(pRecords[i].sName);
    }

    return listResult;
}

void ProcessData::setHeader(QStandardItemModel *pModel,QList<QString> *pListStrings)
{
    int nNumberOfRecords=pListStrings->count();

    for(int i=0;i<nNumberOfRecords;i++)
    {
        pModel->setHeaderData(i,Qt::Horizontal,pListStrings->at(i));
    }
}

void ProcessData::process()
{
    QElapsedTimer scanTimer;
    scanTimer.start();

    emit progressMinimum(0);
    emit progressMaximum(100);

    _process();

    g_bIsStop=false;

    emit progressValue(100);

    emit completed(scanTimer.elapsed());
}

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
#include "processdata.h"

ProcessData::ProcessData()
{
    bIsStop=false;
}

void ProcessData::stop()
{
    bIsStop=true;
}

void ProcessData::setMaximum(quint64 nMaximum)
{
    this->_nMaximum=nMaximum;
    _nProcent=nMaximum/100;
    _nCurrentProcent=0;
    _nValue=0;
}

void ProcessData::incValue()
{
    _nValue++;

    if(_nValue>((_nCurrentProcent+1)*_nProcent))
    {
        _nCurrentProcent++;
        emit progressValue(_nCurrentProcent);
    }
}

bool ProcessData::isRun()
{
    return !(bIsStop);
}

void ProcessData::ajustTreeView(QWidget *pWidget, QTreeView *pTreeView)
{
    Q_UNUSED(pWidget)

    pTreeView->expand(pTreeView->model()->index(0,0));
}

QList<QString> ProcessData::getStructList(const FW_DEF::HEADER_RECORD *pRecords, int nRecordCount)
{
    QList<QString> listResult;

    for(int i=0; i<nRecordCount; i++)
    {
        listResult.append(pRecords[i].sName);
    }

    return listResult;
}

void ProcessData::setHeader(QStandardItemModel *pModel, QList<QString> *pListStrings)
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
    emit progressMinimum(100);

    _process();

    bIsStop=false;

    emit progressValue(100);

    emit completed(scanTimer.elapsed());
}

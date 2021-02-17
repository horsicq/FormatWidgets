// copyright (c) 2019-2021 hors<horsicq@gmail.com>
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
#include "multisearch.h"

MultiSearch::MultiSearch(QObject *pParent) : QObject(pParent)
{
    g_bIsStop=false;
    g_options={};
    g_ppModel=nullptr;

    connect(&g_binary,SIGNAL(errorMessage(QString)),this,SIGNAL(errorMessage(QString)));
    connect(&g_binary,SIGNAL(searchProgressValueChanged(qint32)),this,SIGNAL(progressValueChanged(qint32)));
//    connect(&xBinary,SIGNAL(searchProgressMinimumChanged(qint32)),this,SIGNAL(progressValueMinimum(qint32)));
//    connect(&xBinary,SIGNAL(searchProgressMaximumChanged(qint32)),this,SIGNAL(progressValueMaximum(qint32)));
}

void MultiSearch::setSearchData(QIODevice *pDevice, QList<XBinary::MS_RECORD> *pListRecords, OPTIONS options, TYPE type)
{
    this->g_pDevice=pDevice;
    this->g_pListRecords=pListRecords;

    g_binary.setDevice(pDevice);

    g_options=options;
    g_type=type;
}

void MultiSearch::setModelData(QList<XBinary::MS_RECORD> *pListRecords, QStandardItemModel **ppModel, OPTIONS options, TYPE type)
{
    this->g_pListRecords=pListRecords;
    this->g_ppModel=ppModel;

    g_options=options;
    g_type=type;
}

void MultiSearch::stop()
{
    g_bIsStop=true;
    g_binary.setSearchProcessEnable(false);
}

void MultiSearch::processSearch()
{
    QElapsedTimer scanTimer;
    scanTimer.start();

    if(g_type==TYPE_STRINGS)
    {
        *g_pListRecords=g_binary.multiSearch_allStrings(0,g_pDevice->size(),N_MAX,g_options.nMinLenght,128,g_options.bAnsi,g_options.bUnicode);
    }
    else if(g_type==TYPE_SIGNATURES)
    {
        *g_pListRecords=g_binary.multiSearch_signature(0,g_pDevice->size(),N_MAX,"11+22");
    }

    emit completed(scanTimer.elapsed());
}

void MultiSearch::processModel()
{
    QElapsedTimer scanTimer;
    scanTimer.start();

    if(g_type==TYPE_STRINGS)
    {
        int nNumberOfRecords=g_pListRecords->count();
        *g_ppModel=new QStandardItemModel(nNumberOfRecords,4); // TODO Check maximum

        qint64 nBaseAddress=g_options.nBaseAddress;
        qint32 nAddressWidth=g_options.nAddressWidth;

        qint64 _nProcent=nNumberOfRecords/100;
        qint32 _nCurrentProcent=0;

        emit progressValueChanged(_nCurrentProcent); // TODO Make procents from xbinary

        (*g_ppModel)->setHeaderData(0,Qt::Horizontal,nBaseAddress?(tr("Address")):(tr("Offset")));
        (*g_ppModel)->setHeaderData(1,Qt::Horizontal,tr("Size"));
        (*g_ppModel)->setHeaderData(2,Qt::Horizontal,tr("Type"));
        (*g_ppModel)->setHeaderData(3,Qt::Horizontal,tr("String"));

        emit progressValueChanged(0);

        g_bIsStop=false;

        for(int i=0;(i<nNumberOfRecords)&&(!g_bIsStop);i++)
        {
            XBinary::MS_RECORD record=g_pListRecords->at(i);

            QStandardItem *pTypeAddress=new QStandardItem;
            pTypeAddress->setText(QString("%1").arg(record.nOffset+nBaseAddress,nAddressWidth,16,QChar('0')));
            pTypeAddress->setData(record.nOffset,Qt::UserRole+1);
            pTypeAddress->setData(record.nSize,Qt::UserRole+2);
            pTypeAddress->setTextAlignment(Qt::AlignRight);
            (*g_ppModel)->setItem(i,0,pTypeAddress);

            QStandardItem *pTypeSize=new QStandardItem;
            pTypeSize->setText(QString("%1").arg(record.nSize,8,16,QChar('0')));
            pTypeSize->setTextAlignment(Qt::AlignRight);
            (*g_ppModel)->setItem(i,1,pTypeSize);

            QStandardItem *pTypeItem=new QStandardItem;

            if(record.recordType==XBinary::MS_RECORD_TYPE_ANSI)
            {
                pTypeItem->setText("A");
            }
            else if(record.recordType==XBinary::MS_RECORD_TYPE_UNICODE)
            {
                pTypeItem->setText("U");
            }

            (*g_ppModel)->setItem(i,2,pTypeItem);
            (*g_ppModel)->setItem(i,3,new QStandardItem(record.sString));

            if(i>((_nCurrentProcent+1)*_nProcent))
            {
                _nCurrentProcent++;
                emit progressValueChanged(_nCurrentProcent);
            }
        }

        g_bIsStop=false;
    }
    else if(g_type==TYPE_SIGNATURES)
    {
        int nNumberOfRecords=g_pListRecords->count();
        *g_ppModel=new QStandardItemModel(nNumberOfRecords,2); // TODO Check maximum

        qint64 nBaseAddress=g_options.nBaseAddress;
        qint32 nAddressWidth=g_options.nAddressWidth;

        qint64 _nProcent=nNumberOfRecords/100;
        qint32 _nCurrentProcent=0;

        emit progressValueChanged(_nCurrentProcent); // TODO Make procents from xbinary

        (*g_ppModel)->setHeaderData(0,Qt::Horizontal,tr("Offset"));
        (*g_ppModel)->setHeaderData(1,Qt::Horizontal,tr("Signature"));

        emit progressValueChanged(0);

        g_bIsStop=false;

        for(int i=0;(i<nNumberOfRecords)&&(!g_bIsStop);i++)
        {
            XBinary::MS_RECORD record=g_pListRecords->at(i);

            QStandardItem *pTypeAddress=new QStandardItem;
            pTypeAddress->setText(QString("%1").arg(record.nOffset+nBaseAddress,nAddressWidth,16,QChar('0')));
            pTypeAddress->setData(record.nOffset,Qt::UserRole+1);
            pTypeAddress->setData(record.nSize,Qt::UserRole+2);
            pTypeAddress->setTextAlignment(Qt::AlignRight);
            (*g_ppModel)->setItem(i,0,pTypeAddress);

            (*g_ppModel)->setItem(i,1,new QStandardItem(record.sString));

            if(i>((_nCurrentProcent+1)*_nProcent))
            {
                _nCurrentProcent++;
                emit progressValueChanged(_nCurrentProcent);
            }
        }

        g_bIsStop=false;
    }

    emit completed(scanTimer.elapsed());
}


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
    g_pSemaphore=new QSemaphore(N_MAXNUMBEROFTHREADS);
    g_procent={};
}

MultiSearch::~MultiSearch()
{
    delete g_pSemaphore;
}

void MultiSearch::setSearchData(QIODevice *pDevice, QList<XBinary::MS_RECORD> *pListRecords, OPTIONS options, TYPE type)
{
    this->g_pDevice=pDevice;
    this->g_pListRecords=pListRecords;

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

QList<MultiSearch::SIGNATURE_RECORD> MultiSearch::loadSignaturesFromFile(QString sFileName)
{
    QList<SIGNATURE_RECORD> listResult;

    QFile file;
    file.setFileName(sFileName);

    qint32 nIndex=0;

    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        while(!in.atEnd())
        {
            QString sLine=in.readLine().trimmed();
            if(sLine!="")
            {
                SIGNATURE_RECORD record={};
                record.nNumber=nIndex++;
                record.sName=sLine.section(";",0,0);
                record.sSignature=sLine.section(";",2,-1);

                QString sEndianess=sLine.section(";",1,1);

                record.bIsBigEndian=(sEndianess=="BE");
                record.bIsLittleEndian=(sEndianess=="LE");

                listResult.append(record);
            }
        }

        file.close();
    }

    return listResult;
}

void MultiSearch::processSignature(MultiSearch::SIGNATURE_RECORD signatureRecord)
{
//#ifdef QT_DEBUG
//    QElapsedTimer timer;
//    timer.start();
//#endif
    g_pSemaphore->acquire();

    bool bLog=false;

    XBinary binary;

    binary.setReadWriteMutex(&g_mutex);
    binary.setDevice(g_pDevice);

    connect(this,SIGNAL(setSearchProcessEnable(bool)),&binary,SLOT(setSearchProcessEnable(bool)),Qt::DirectConnection);

    QList<XBinary::MS_RECORD> listResult=binary.multiSearch_signature(&(g_options.memoryMap),0,binary.getSize(),N_MAX,signatureRecord.sSignature,signatureRecord.sName);

    g_pListRecords->append(listResult);

    if(XBinary::procentSetCurrentValue(&g_procent,signatureRecord.nNumber))
    {
        emit progressValueChanged(g_procent.nCurrentProcent);
        emit progressInfo(signatureRecord.sName);
        emit progressFound(g_pListRecords->count());
    }

    g_pSemaphore->release();
//#ifdef QT_DEBUG
//    qDebug("%lld %s",timer.elapsed(),signatureRecord.sName.toLatin1().data());
//#endif
}

void MultiSearch::stop()
{
    emit setSearchProcessEnable(false);

    g_bIsStop=true;
}

void MultiSearch::processSearch()
{
    QElapsedTimer scanTimer;
    scanTimer.start();

    if(g_type==TYPE_STRINGS)
    {
        XBinary binary(g_pDevice);

        connect(&binary,SIGNAL(errorMessage(QString)),this,SIGNAL(errorMessage(QString)));
        connect(&binary,SIGNAL(searchProgressValueChanged(qint32)),this,SIGNAL(progressValueChanged(qint32)));
        connect(this,SIGNAL(setSearchProcessEnable(bool)),&binary,SLOT(setSearchProcessEnable(bool)),Qt::DirectConnection);

        *g_pListRecords=binary.multiSearch_allStrings(0,g_pDevice->size(),N_MAX,g_options.nMinLenght,128,g_options.bAnsi,g_options.bUnicode);
    }
    else if(g_type==TYPE_SIGNATURES)
    {
    #ifdef QT_DEBUG
        QElapsedTimer timer;
        timer.start();
        qDebug("Signatures start");
    #endif

        int nNumberOfSignatures=g_options.pListSignatureRecords->count();

        g_procent=XBinary::procentInit(nNumberOfSignatures,true);

        emit progressValueChanged(0);

        for(int i=0;(i<nNumberOfSignatures)&&(!g_bIsStop);i++)
        {
            SIGNATURE_RECORD signatureRecord=g_options.pListSignatureRecords->at(i);

            bool bSuccess=false;

            if((signatureRecord.bIsBigEndian)&&(g_options.bIsBigEndian))
            {
                bSuccess=true;
            }
            else if((signatureRecord.bIsLittleEndian)&&(!g_options.bIsBigEndian))
            {
                bSuccess=true;
            }
            else if((!signatureRecord.bIsLittleEndian)&&(!signatureRecord.bIsBigEndian))
            {
                bSuccess=true;
            }

            if(bSuccess)
            {
                QFuture<void> future=QtConcurrent::run(this,&MultiSearch::processSignature,signatureRecord);

                while(true)
                {
                    int nAvailable=g_pSemaphore->available();

                    if(nAvailable)
                    {
                        break;
                    }
//                    QThread::msleep(5);
                }
            }
        }

        while(true)
        {
            int nAvailable=g_pSemaphore->available();

            if(nAvailable==N_MAXNUMBEROFTHREADS)
            {
                break;
            }

            QThread::msleep(10);
        }

        QThread::msleep(100);

        emit progressValueChanged(g_procent.nMaxProcent);

    #ifdef QT_DEBUG
        qDebug("Signatures end: %lld msec",timer.elapsed());
    #endif
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

        qint64 nBaseAddress=g_options.memoryMap.nBaseAddress;

        XBinary::MODE modeAddress=XBinary::getWidthModeFromSize(nBaseAddress+g_options.memoryMap.nRawSize);

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
            pTypeAddress->setText(XBinary::valueToHex(modeAddress,record.nOffset+nBaseAddress));
            pTypeAddress->setData(record.nOffset,Qt::UserRole+1);
            pTypeAddress->setData(record.nSize,Qt::UserRole+2);
            pTypeAddress->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
            (*g_ppModel)->setItem(i,0,pTypeAddress);

            QStandardItem *pTypeSize=new QStandardItem;
            pTypeSize->setText(XBinary::valueToHex(XBinary::MODE_32,record.nSize));
            pTypeSize->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
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
            pTypeItem->setTextAlignment(Qt::AlignLeft);
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
        *g_ppModel=new QStandardItemModel(nNumberOfRecords,3); // TODO Check maximum

        XBinary::MODE modeAddress=XBinary::getWidthModeFromMemoryMap(&(g_options.memoryMap));

        qint64 _nProcent=nNumberOfRecords/100;
        qint32 _nCurrentProcent=0;

        emit progressValueChanged(_nCurrentProcent); // TODO Make procents from xbinary

        (*g_ppModel)->setHeaderData(0,Qt::Horizontal,tr("Address"));
        (*g_ppModel)->setHeaderData(1,Qt::Horizontal,tr("Offset"));
        (*g_ppModel)->setHeaderData(2,Qt::Horizontal,tr("Name"));

        emit progressValueChanged(0);

        g_bIsStop=false;

        for(int i=0;(i<nNumberOfRecords)&&(!g_bIsStop);i++)
        {
            XBinary::MS_RECORD record=g_pListRecords->at(i);

            qint64 nAddress=XBinary::offsetToAddress(&(g_options.memoryMap),record.nOffset);

            QStandardItem *pTypeAddress=new QStandardItem;
            pTypeAddress->setText(XBinary::valueToHex(modeAddress,nAddress));
            pTypeAddress->setData(record.nOffset,Qt::UserRole+1);
            pTypeAddress->setData(record.nSize,Qt::UserRole+2);
            pTypeAddress->setData(record.sString,Qt::UserRole+3);
            pTypeAddress->setData(record.sInfo,Qt::UserRole+4);
            pTypeAddress->setData(nAddress,Qt::UserRole+5);
            pTypeAddress->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
            (*g_ppModel)->setItem(i,0,pTypeAddress);

            QStandardItem *pTypeOffset=new QStandardItem;
            pTypeOffset->setText((XBinary::valueToHex(modeAddress,record.nOffset)));
            pTypeOffset->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);

            (*g_ppModel)->setItem(i,1,pTypeOffset);

            QString sName=record.sInfo;

            if(sName=="")
            {
                sName=record.sString;
            }

            (*g_ppModel)->setItem(i,2,new QStandardItem(sName));

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


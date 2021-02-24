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
#ifndef MULTISEARCH_H
#define MULTISEARCH_H

#include <QIODevice>
#include <QSemaphore>
#include <QFuture>
#include <QtConcurrent>
#include <QtConcurrentMap>
#include <QElapsedTimer>
#include <QStandardItemModel>
#include "xbinary.h"

class MultiSearch : public QObject
{
    Q_OBJECT

public:

    enum TYPE
    {
        TYPE_STRINGS=0,
        TYPE_SIGNATURES
    };

    struct SIGNATURE_RECORD
    {
        qint32 nNumber;
        QString sName;
        bool bIsBigEndian;
        bool bIsLittleEndian;
        QString sSignature;
    };

    struct OPTIONS
    {
        XBinary::_MEMORY_MAP memoryMap;
        bool bAnsi; // TODO
        bool bUnicode; // TODO
        bool bSignature;
        bool bIsBigEndian;
        qint32 nMinLenght;
        QList<SIGNATURE_RECORD> *pListSignatureRecords;
        bool bMenu_Hex;
    };

    const int N_MAX=50000;
    const int N_MAXNUMBEROFTHREADS=8;

    explicit MultiSearch(QObject *pParent=nullptr);
    ~MultiSearch();
    void setSearchData(QIODevice *pDevice,QList<XBinary::MS_RECORD> *pListRecords,OPTIONS options,TYPE type);
    void setModelData(QList<XBinary::MS_RECORD> *pListRecords,QStandardItemModel **ppModel,OPTIONS options,TYPE type);

    static QList<SIGNATURE_RECORD> loadSignaturesFromFile(QString sFileName);
    void processSignature(SIGNATURE_RECORD signatureRecord);

signals:
    void errorMessage(QString sText);
    void completed(qint64 nElapsed);
    void progressValueChanged(qint32 nValue);
    void progressInfo(QString sText);
    void progressFound(qint32 nValue);
    void setSearchProcessEnable(bool bState);

public slots:
    void stop();
    void processSearch();
    void processModel();

private:
    QIODevice *g_pDevice;
    QList<XBinary::MS_RECORD> *g_pListRecords;
    OPTIONS g_options;
    TYPE g_type;
    QStandardItemModel **g_ppModel;
    bool g_bIsStop;
    QSemaphore *g_pSemaphore;
    QMutex g_mutex;
    XBinary::PROCENT g_procent;
};

#endif // MULTISEARCH_H

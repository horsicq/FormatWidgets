/* Copyright (c) 2019-2026 hors<horsicq@gmail.com>
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
#ifndef MULTISEARCH_H
#define MULTISEARCH_H

#include <QElapsedTimer>
#include <QFuture>
#include <QIODevice>
#include <QSemaphore>
#include <QStandardItemModel>
#include <QtConcurrent>
// #include <QtConcurrentMap>

#include "xformats.h"
#include "xoptions.h"
// #include "xinfodb.h"
#include "xthreadobject.h"

class MultiSearch : public XThreadObject {
    Q_OBJECT

public:
    enum TYPE {
        TYPE_STRINGS = 0,
        TYPE_SIGNATURES,
        TYPE_VALUES,
        TYPE_STRINGS_XINFODB
    };

    struct OPTIONS {
        XBinary::_MEMORY_MAP memoryMap;
        bool bAnsi;
        // bool bUTF8;     // TODO
        bool bUnicode;
        bool bNullTerminated;
        // QString sANSICodec;
        bool bLinks;
        QString sMask;
        XBinary::ENDIAN endian;
        qint32 nMinLenght;
        QList<XBinary::SIGNATUREDB_RECORD> *pListSignatureRecords;
        QVariant varValue;
        XBinary::VT valueType;
        bool bMenu_Hex;  // TODO remove
    };

    const qint32 N_MAX = 0xFFFFFFF;
    const qint32 N_MAXNUMBEROFTHREADS = 8;

    explicit MultiSearch(QObject *pParent = nullptr);
    ~MultiSearch();

    void setSearchData(QIODevice *pDevice, QVector<XBinary::MS_RECORD> *pListRecords, OPTIONS options, TYPE type, XBinary::PDSTRUCT *pPdStruct);
    void setModelData(QVector<XBinary::MS_RECORD> *pListRecords, QStandardItemModel **ppModel, OPTIONS options, TYPE type, XBinary::PDSTRUCT *pPdStruct);

    static QList<XBinary::SIGNATUREDB_RECORD> loadSignaturesFromFile(const QString &sFileName);
    void processSignature(XBinary::SIGNATUREDB_RECORD signatureRecord);
    virtual void process();
    virtual QString getTitle();

private:
    QIODevice *m_pDevice;
    QVector<XBinary::MS_RECORD> *m_pListRecords;
    OPTIONS m_options;
    TYPE m_type;
    QStandardItemModel **m_ppModel;
    QSemaphore *m_pSemaphore;
    QMutex m_mutex;
    XBinary::PDSTRUCT *m_pPdStruct;
    qint32 m_nFreeIndex;
};

#endif  // MULTISEARCH_H

/* Copyright (c) 2019-2024 hors<horsicq@gmail.com>
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
#include "multisearch.h"

MultiSearch::MultiSearch(QObject *pParent) : QObject(pParent)
{
    g_options = {};
    g_ppModel = nullptr;
    g_nFreeIndex = -1;
    g_pPdStruct = nullptr;
    g_pSemaphore = new QSemaphore(N_MAXNUMBEROFTHREADS);
}

MultiSearch::~MultiSearch()
{
    delete g_pSemaphore;
}

void MultiSearch::setSearchData(QIODevice *pDevice, QVector<XBinary::MS_RECORD> *pListRecords, OPTIONS options, TYPE type, XBinary::PDSTRUCT *pPdStruct)
{
    this->g_pDevice = pDevice;
    this->g_pListRecords = pListRecords;

    g_options = options;
    g_type = type;
    g_pPdStruct = pPdStruct;
}

void MultiSearch::setModelData(QVector<XBinary::MS_RECORD> *pListRecords, QStandardItemModel **ppModel, OPTIONS options, TYPE type, XBinary::PDSTRUCT *pPdStruct)
{
    this->g_pListRecords = pListRecords;
    this->g_ppModel = ppModel;

    g_options = options;
    g_type = type;
    g_pPdStruct = pPdStruct;
}

QList<XBinary::SIGNATUREDB_RECORD> MultiSearch::loadSignaturesFromFile(const QString &sFileName)
{
    QList<XBinary::SIGNATUREDB_RECORD> listResult;

    QFile file;
    file.setFileName(sFileName);

    qint32 nIndex = 0;

    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString sLine = in.readLine().trimmed();
            if (sLine != "") {
                XBinary::SIGNATUREDB_RECORD record = {};
                record.nNumber = nIndex++;
                record.sName = sLine.section(";", 0, 0);
                record.sSignature = sLine.section(";", 2, -1);
                record.sPatch = sLine.section(";", 3, 3);
                record.endian = XBinary::ENDIAN_UNKNOWN;

                QString sInfo = sLine.section(";", 1, 1);

                if (sInfo.contains("BE")) {
                    record.endian = XBinary::ENDIAN_BIG;
                } else if (sInfo.contains("LE")) {
                    record.endian = XBinary::ENDIAN_LITTLE;
                }

                if (sInfo.contains("P:")) {
                    record.sPatch = sInfo.section("P:", 1, -1);
                }

                listResult.append(record);
            }
        }

        file.close();
    }

    return listResult;
}

void MultiSearch::processSignature(XBinary::SIGNATUREDB_RECORD signatureRecord)
{
    // #ifdef QT_DEBUG
    //     QElapsedTimer timer;
    //     timer.start();
    // #endif
    g_pSemaphore->acquire();

    XBinary binary;

    binary.setReadWriteMutex(&g_mutex);
    binary.setDevice(g_pDevice);

    QVector<XBinary::MS_RECORD> listResult =
        binary.multiSearch_signature(&(g_options.memoryMap), 0, binary.getSize(), N_MAX, signatureRecord.sSignature, signatureRecord.nNumber, g_pPdStruct);

    (*g_pListRecords) += listResult;

    XBinary::setPdStructStatus(g_pPdStruct, g_nFreeIndex, signatureRecord.sName);
    XBinary::setPdStructCurrent(g_pPdStruct, g_nFreeIndex, signatureRecord.nNumber);

    g_pSemaphore->release();
    // #ifdef QT_DEBUG
    //     qDebug("%lld
    //     %s",timer.elapsed(),signatureRecord.sName.toLatin1().data());
    // #endif
}

void MultiSearch::processSearch()
{
    QElapsedTimer scanTimer;
    scanTimer.start();

    g_nFreeIndex = XBinary::getFreeIndex(g_pPdStruct);
    XBinary::setPdStructInit(g_pPdStruct, g_nFreeIndex, 0);

    if ((g_type == TYPE_STRINGS) || (g_type == TYPE_STRINGS_XINFODB)) {
        XBinary binary(g_pDevice);

        connect(&binary, SIGNAL(errorMessage(QString)), this, SIGNAL(errorMessage(QString)));

        XBinary::STRINGSEARCH_OPTIONS ssOptions = {};
        ssOptions.nLimit = N_MAX;
        ssOptions.nMinLenght = g_options.nMinLenght;
        ssOptions.nMaxLenght = 256;
        ssOptions.bAnsi = g_options.bAnsi;
        // ssOptions.bUTF8 = g_options.bUTF8;
        ssOptions.bUnicode = g_options.bUnicode;
        ssOptions.bNullTerminated = g_options.bNullTerminated;
        // ssOptions.sANSICodec = g_options.sANSICodec;
        ssOptions.bLinks = g_options.bLinks;
        ssOptions.sMask = g_options.sMask;

        *g_pListRecords = binary.multiSearch_allStrings(&(g_options.memoryMap), 0, g_pDevice->size(), ssOptions, g_pPdStruct);
    } else if (g_type == TYPE_VALUES) {
        XBinary binary(g_pDevice);

        connect(&binary, SIGNAL(errorMessage(QString)), this, SIGNAL(errorMessage(QString)));

        *g_pListRecords = binary.multiSearch_value(&(g_options.memoryMap), 0, g_pDevice->size(), N_MAX, g_options.varValue, g_options.valueType,
                                                   (g_options.endian == XBinary::ENDIAN_BIG), g_pPdStruct);
    } else if (g_type == TYPE_SIGNATURES) {
#ifdef QT_DEBUG
        QElapsedTimer timer;
        timer.start();
        qDebug("Signatures start");
#endif
        g_pListRecords->clear();

        qint32 nNumberOfSignatures = g_options.pListSignatureRecords->count();

        XBinary::setPdStructTotal(g_pPdStruct, g_nFreeIndex, nNumberOfSignatures);

        for (qint32 i = 0; (i < nNumberOfSignatures) && (!(g_pPdStruct->bIsStop)); i++) {
            XBinary::SIGNATUREDB_RECORD signatureRecord = g_options.pListSignatureRecords->at(i);

            bool bSuccess = false;

            if (signatureRecord.endian == g_options.endian) {
                bSuccess = true;
            } else if (signatureRecord.endian == XBinary::ENDIAN_UNKNOWN) {
                bSuccess = true;
            }

            if (bSuccess) {
                // QFuture<void>
                // future=QtConcurrent::run(this,&MultiSearch::processSignature,signatureRecord);

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                QFuture<void> future = QtConcurrent::run(&MultiSearch::processSignature, this,
                                                         signatureRecord);  // mb TODO return
#else
                QtConcurrent::run(this, &MultiSearch::processSignature, signatureRecord);
#endif

                QThread::msleep(5);  // wait till run started TODO rewrite!

                while (true) {
                    qint32 nAvailable = g_pSemaphore->available();

                    if (nAvailable) {
                        break;
                    }
                    //                    QThread::msleep(5);
                }
            }
        }

        while (true) {
            qint32 nAvailable = g_pSemaphore->available();

            if (nAvailable == N_MAXNUMBEROFTHREADS) {
                break;
            }

            QThread::msleep(10);
        }

        QThread::msleep(100);

#ifdef QT_DEBUG
        qDebug("Signatures end: %lld msec", timer.elapsed());
#endif
    }

    XBinary::setPdStructFinished(g_pPdStruct, g_nFreeIndex);

    emit completed(scanTimer.elapsed());
}

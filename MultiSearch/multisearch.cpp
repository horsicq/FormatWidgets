/* Copyright (c) 2019-2025 hors<horsicq@gmail.com>
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

MultiSearch::MultiSearch(QObject *pParent) : XThreadObject(pParent)
{
    m_options = {};
    m_ppModel = nullptr;
    m_nFreeIndex = -1;
    m_pPdStruct = nullptr;
    m_pSemaphore = new QSemaphore(N_MAXNUMBEROFTHREADS);
}

MultiSearch::~MultiSearch()
{
    delete m_pSemaphore;
}

void MultiSearch::setSearchData(QIODevice *pDevice, QVector<XBinary::MS_RECORD> *pListRecords, OPTIONS options, TYPE type, XBinary::PDSTRUCT *pPdStruct)
{
    this->m_pDevice = pDevice;
    this->m_pListRecords = pListRecords;

    m_options = options;
    m_type = type;
    m_pPdStruct = pPdStruct;
}

void MultiSearch::setModelData(QVector<XBinary::MS_RECORD> *pListRecords, QStandardItemModel **ppModel, OPTIONS options, TYPE type, XBinary::PDSTRUCT *pPdStruct)
{
    this->m_pListRecords = pListRecords;
    this->m_ppModel = ppModel;

    m_options = options;
    m_type = type;
    m_pPdStruct = pPdStruct;
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
    m_pSemaphore->acquire();

    XBinary binary;

    binary.setReadWriteMutex(&m_mutex);
    binary.setDevice(m_pDevice);

    QVector<XBinary::MS_RECORD> listResult =
        binary.multiSearch_signature(&(m_options.memoryMap), 0, binary.getSize(), N_MAX, signatureRecord.sSignature, signatureRecord.nNumber, m_pPdStruct);

    (*m_pListRecords) += listResult;

    XBinary::setPdStructStatus(m_pPdStruct, m_nFreeIndex, signatureRecord.sName);
    XBinary::setPdStructCurrent(m_pPdStruct, m_nFreeIndex, signatureRecord.nNumber);

    m_pSemaphore->release();
    // #ifdef QT_DEBUG
    //     qDebug("%lld
    //     %s",timer.elapsed(),signatureRecord.sName.toLatin1().data());
    // #endif
}

void MultiSearch::process()
{
    m_nFreeIndex = XBinary::getFreeIndex(m_pPdStruct);
    XBinary::setPdStructInit(m_pPdStruct, m_nFreeIndex, 0);

    if ((m_type == TYPE_STRINGS) || (m_type == TYPE_STRINGS_XINFODB)) {
        XBinary binary(m_pDevice);

        connect(&binary, SIGNAL(errorMessage(QString)), this, SIGNAL(errorMessage(QString)));

        XBinary::STRINGSEARCH_OPTIONS ssOptions = {};
        ssOptions.nLimit = N_MAX;
        ssOptions.nMinLenght = m_options.nMinLenght;
        ssOptions.nMaxLenght = 256;
        ssOptions.bAnsi = m_options.bAnsi;
        // ssOptions.bUTF8 = m_options.bUTF8;
        ssOptions.bUnicode = m_options.bUnicode;
        ssOptions.bNullTerminated = m_options.bNullTerminated;
        // ssOptions.sANSICodec = m_options.sANSICodec;
        ssOptions.bLinks = m_options.bLinks;
        ssOptions.sMask = m_options.sMask;

        *m_pListRecords = binary.multiSearch_allStrings(&(m_options.memoryMap), 0, m_pDevice->size(), ssOptions, m_pPdStruct);
    } else if (m_type == TYPE_VALUES) {
        XBinary binary(m_pDevice);

        connect(&binary, SIGNAL(errorMessage(QString)), this, SIGNAL(errorMessage(QString)));

        *m_pListRecords = binary.multiSearch_value(&(m_options.memoryMap), 0, m_pDevice->size(), N_MAX, m_options.varValue, m_options.valueType,
                                                   (m_options.endian == XBinary::ENDIAN_BIG), m_pPdStruct);
    } else if (m_type == TYPE_SIGNATURES) {
#ifdef QT_DEBUG
        QElapsedTimer timer;
        timer.start();
        qDebug("Signatures start");
#endif
        m_pListRecords->clear();

        qint32 nNumberOfSignatures = m_options.pListSignatureRecords->count();

        XBinary::setPdStructTotal(m_pPdStruct, m_nFreeIndex, nNumberOfSignatures);

        for (qint32 i = 0; (i < nNumberOfSignatures) && XBinary::isPdStructNotCanceled(m_pPdStruct); i++) {
            XBinary::SIGNATUREDB_RECORD signatureRecord = m_options.pListSignatureRecords->at(i);

            bool bSuccess = false;

            if (signatureRecord.endian == m_options.endian) {
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
                    qint32 nAvailable = m_pSemaphore->available();

                    if (nAvailable) {
                        break;
                    }
                    //                    QThread::msleep(5);
                }
            }
        }

        while (true) {
            qint32 nAvailable = m_pSemaphore->available();

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

    XBinary::setPdStructFinished(m_pPdStruct, m_nFreeIndex);
}

QString MultiSearch::getTitle()
{
    QString sResult;

    if (m_type == MultiSearch::TYPE_STRINGS) {
        sResult = tr("Search strings");
    } else if (m_type == MultiSearch::TYPE_SIGNATURES) {
        sResult = tr("Search signatures");
    } else if (m_type == MultiSearch::TYPE_VALUES) {
        sResult = tr("Search values");
    }

    return sResult;
}

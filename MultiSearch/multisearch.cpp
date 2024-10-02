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

void MultiSearch::setSearchData(QIODevice *pDevice, QList<XBinary::MS_RECORD> *pListRecords, OPTIONS options, TYPE type, XBinary::PDSTRUCT *pPdStruct)
{
    this->g_pDevice = pDevice;
    this->g_pListRecords = pListRecords;

    g_options = options;
    g_type = type;
    g_pPdStruct = pPdStruct;
}

void MultiSearch::setModelData(QList<XBinary::MS_RECORD> *pListRecords, QStandardItemModel **ppModel, OPTIONS options, TYPE type, XBinary::PDSTRUCT *pPdStruct)
{
    this->g_pListRecords = pListRecords;
    this->g_ppModel = ppModel;

    g_options = options;
    g_type = type;
    g_pPdStruct = pPdStruct;
}

QList<MultiSearch::SIGNATURE_RECORD> MultiSearch::loadSignaturesFromFile(const QString &sFileName)
{
    QList<SIGNATURE_RECORD> listResult;

    QFile file;
    file.setFileName(sFileName);

    qint32 nIndex = 0;

    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString sLine = in.readLine().trimmed();
            if (sLine != "") {
                SIGNATURE_RECORD record = {};
                record.nNumber = nIndex++;
                record.sName = sLine.section(";", 0, 0);
                record.sSignature = sLine.section(";", 2, -1);

                QString sEndianness = sLine.section(";", 1, 1);

                if (sEndianness == "BE") {
                    record.endian = XBinary::ENDIAN_BIG;
                } else if (sEndianness == "LE") {
                    record.endian = XBinary::ENDIAN_LITTLE;
                } else {
                    record.endian = XBinary::ENDIAN_UNKNOWN;
                }

                listResult.append(record);
            }
        }

        file.close();
    }

    return listResult;
}

MultiSearch::SIGNATURE_RECORD MultiSearch::createSignature(const QString &sName, const QString &sSignature)
{
    MultiSearch::SIGNATURE_RECORD result = {};

    result.endian = XBinary::ENDIAN_LITTLE;
    result.nNumber = 0;
    result.sName = sName;
    result.sSignature = sSignature;

    return result;
}

void MultiSearch::processSignature(MultiSearch::SIGNATURE_RECORD signatureRecord)
{
    // #ifdef QT_DEBUG
    //     QElapsedTimer timer;
    //     timer.start();
    // #endif
    g_pSemaphore->acquire();

    XBinary binary;

    binary.setReadWriteMutex(&g_mutex);
    binary.setDevice(g_pDevice);

    QList<XBinary::MS_RECORD> listResult =
        binary.multiSearch_signature(&(g_options.memoryMap), 0, binary.getSize(), N_MAX, signatureRecord.sSignature, signatureRecord.sName, g_pPdStruct);

    g_pListRecords->append(listResult);

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
        //ssOptions.bUTF8 = g_options.bUTF8;
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

        qint32 nNumberOfSignatures = g_options.pListSignatureRecords->count();

        XBinary::setPdStructTotal(g_pPdStruct, g_nFreeIndex, nNumberOfSignatures);

        for (qint32 i = 0; (i < nNumberOfSignatures) && (!(g_pPdStruct->bIsStop)); i++) {
            SIGNATURE_RECORD signatureRecord = g_options.pListSignatureRecords->at(i);

            bool bSuccess = false;

            if ((signatureRecord.endian == g_options.endian)) {
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

void MultiSearch::processModel()
{
    QElapsedTimer scanTimer;
    scanTimer.start();

    g_nFreeIndex = XBinary::getFreeIndex(g_pPdStruct);
    XBinary::setPdStructInit(g_pPdStruct, g_nFreeIndex, 0);

    if (g_type == TYPE_STRINGS) {
        qint32 nNumberOfRecords = g_pListRecords->count();
        *g_ppModel = new QStandardItemModel(nNumberOfRecords, __COLUMN_STRING_SIZE);  // TODO Check maximum

        //        XADDR nBaseAddress = g_options.memoryMap.nModuleAddress;

        XBinary::MODE modeAddress = XBinary::getWidthModeFromSize(g_options.memoryMap.nModuleAddress + g_options.memoryMap.nImageSize);
        XBinary::MODE modeOffset = XBinary::getWidthModeFromSize(g_options.memoryMap.nBinarySize);

        XBinary::setPdStructTotal(g_pPdStruct, g_nFreeIndex, nNumberOfRecords);

        (*g_ppModel)->setHeaderData(COLUMN_STRING_NUMBER, Qt::Horizontal, tr("Number"));
        (*g_ppModel)->setHeaderData(COLUMN_STRING_OFFSET, Qt::Horizontal, tr("Offset"));
        (*g_ppModel)->setHeaderData(COLUMN_STRING_ADDRESS, Qt::Horizontal, tr("Address"));
        (*g_ppModel)->setHeaderData(COLUMN_STRING_REGION, Qt::Horizontal, "");
        (*g_ppModel)->setHeaderData(COLUMN_STRING_SIZE, Qt::Horizontal, tr("Size"));
        (*g_ppModel)->setHeaderData(COLUMN_STRING_TYPE, Qt::Horizontal, tr("Type"));
        (*g_ppModel)->setHeaderData(COLUMN_STRING_VALUE, Qt::Horizontal, tr("String"));

        for (qint32 i = 0; (i < nNumberOfRecords) && (!(g_pPdStruct->bIsStop)); i++) {
            XBinary::MS_RECORD record = g_pListRecords->at(i);

            {
                QStandardItem *pTypeNumber = new QStandardItem;
                pTypeNumber->setData(i, Qt::DisplayRole);
                pTypeNumber->setData(record.nOffset, Qt::UserRole + USERROLE_OFFSET);
                pTypeNumber->setData(record.nAddress, Qt::UserRole + USERROLE_ADDRESS);
                pTypeNumber->setData(record.nSize, Qt::UserRole + USERROLE_SIZE);
                pTypeNumber->setData(record.recordType, Qt::UserRole + USERROLE_TYPE);
                (*g_ppModel)->setItem(i, COLUMN_STRING_NUMBER, pTypeNumber);
            }
            {
                QStandardItem *pItem = new QStandardItem;
                pItem->setText(XBinary::valueToHex(modeOffset, record.nOffset));
                (*g_ppModel)->setItem(i, COLUMN_STRING_OFFSET, pItem);
            }
            {
                QStandardItem *pItem = new QStandardItem;

                if (record.nAddress != (XADDR)-1) {
                    pItem->setText(XBinary::valueToHex(modeAddress, record.nAddress));
                }

                (*g_ppModel)->setItem(i, COLUMN_STRING_ADDRESS, pItem);
            }
            {
                QStandardItem *pItem = new QStandardItem;
                pItem->setText(record.sRegion);
                (*g_ppModel)->setItem(i, COLUMN_STRING_REGION, pItem);
            }
            {
                QStandardItem *pTypeSize = new QStandardItem;
                pTypeSize->setText(XBinary::valueToHexEx(record.sString.size()));
                (*g_ppModel)->setItem(i, COLUMN_STRING_SIZE, pTypeSize);
            }
            {
                QStandardItem *pTypeItem = new QStandardItem;
                pTypeItem->setText(XBinary::msRecordTypeIdToString(record.recordType));
                (*g_ppModel)->setItem(i, COLUMN_STRING_TYPE, pTypeItem);
            }
            {
                (*g_ppModel)->setItem(i, COLUMN_STRING_VALUE, new QStandardItem(record.sString));
            }

            XBinary::setPdStructCurrent(g_pPdStruct, g_nFreeIndex, i);
        }

        XOptions::setModelTextAlignment((*g_ppModel), COLUMN_STRING_NUMBER, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment((*g_ppModel), COLUMN_STRING_OFFSET, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment((*g_ppModel), COLUMN_STRING_ADDRESS, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment((*g_ppModel), COLUMN_STRING_REGION, Qt::AlignLeft | Qt::AlignVCenter);
        XOptions::setModelTextAlignment((*g_ppModel), COLUMN_STRING_SIZE, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment((*g_ppModel), COLUMN_STRING_TYPE, Qt::AlignLeft | Qt::AlignVCenter);
        XOptions::setModelTextAlignment((*g_ppModel), COLUMN_STRING_VALUE, Qt::AlignLeft | Qt::AlignVCenter);
    } else if (g_type == TYPE_VALUES) {
        // TODO rewrite
        qint32 nNumberOfRecords = g_pListRecords->count();
        *g_ppModel = new QStandardItemModel(nNumberOfRecords, __COLUMN_VALUE_SIZE);  // TODO Check maximum

        XBinary::MODE modeAddress = XBinary::getWidthModeFromSize(g_options.memoryMap.nModuleAddress + g_options.memoryMap.nImageSize);
        XBinary::MODE modeOffset = XBinary::getWidthModeFromSize(g_options.memoryMap.nBinarySize);

        XBinary::setPdStructTotal(g_pPdStruct, g_nFreeIndex, nNumberOfRecords);

        (*g_ppModel)->setHeaderData(COLUMN_VALUE_NUMBER, Qt::Horizontal, tr("Number"));
        (*g_ppModel)->setHeaderData(COLUMN_VALUE_OFFSET, Qt::Horizontal, tr("Offset"));
        (*g_ppModel)->setHeaderData(COLUMN_VALUE_ADDRESS, Qt::Horizontal, tr("Address"));
        (*g_ppModel)->setHeaderData(COLUMN_VALUE_REGION, Qt::Horizontal, "");
        (*g_ppModel)->setHeaderData(COLUMN_VALUE_VALUE, Qt::Horizontal, tr("Value"));

        for (qint32 i = 0; (i < nNumberOfRecords) && (!(g_pPdStruct->bIsStop)); i++) {
            XBinary::MS_RECORD record = g_pListRecords->at(i);

            {
                QStandardItem *pItem = new QStandardItem;
                pItem->setData(i, Qt::DisplayRole);
                // TODO mb more
                pItem->setData(record.nAddress, Qt::UserRole + USERROLE_ADDRESS);
                pItem->setData(record.nOffset, Qt::UserRole + USERROLE_OFFSET);
                pItem->setData(record.nSize, Qt::UserRole + USERROLE_SIZE);
                pItem->setData(record.recordType, Qt::UserRole + USERROLE_TYPE);
                (*g_ppModel)->setItem(i, COLUMN_VALUE_NUMBER, pItem);
            }
            {
                QStandardItem *pItem = new QStandardItem;
                pItem->setText(XBinary::valueToHex(modeOffset, record.nOffset));
                (*g_ppModel)->setItem(i, COLUMN_VALUE_OFFSET, pItem);
            }
            {
                QStandardItem *pItem = new QStandardItem;

                if (record.nAddress != (XADDR)-1) {
                    pItem->setText(XBinary::valueToHex(modeAddress, record.nAddress));
                }

                (*g_ppModel)->setItem(i, COLUMN_VALUE_ADDRESS, pItem);
            }
            {
                QStandardItem *pItem = new QStandardItem;
                pItem->setText(record.sRegion);
                (*g_ppModel)->setItem(i, COLUMN_VALUE_REGION, pItem);
            }
            {
                QStandardItem *pItem = new QStandardItem;
                pItem->setText(record.sString);
                (*g_ppModel)->setItem(i, COLUMN_VALUE_VALUE, pItem);
            }

            XBinary::setPdStructCurrent(g_pPdStruct, g_nFreeIndex, i);
        }

        XOptions::setModelTextAlignment((*g_ppModel), COLUMN_VALUE_NUMBER, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment((*g_ppModel), COLUMN_VALUE_OFFSET, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment((*g_ppModel), COLUMN_VALUE_ADDRESS, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment((*g_ppModel), COLUMN_VALUE_REGION, Qt::AlignLeft | Qt::AlignVCenter);
        XOptions::setModelTextAlignment((*g_ppModel), COLUMN_VALUE_VALUE, Qt::AlignLeft | Qt::AlignVCenter);
    } else if (g_type == TYPE_SIGNATURES) {
        qint32 nNumberOfRecords = g_pListRecords->count();
        *g_ppModel = new QStandardItemModel(nNumberOfRecords, 3);  // TODO Check maximum

        XBinary::MODE modeAddress = XBinary::getWidthModeFromMemoryMap(&(g_options.memoryMap));
        XBinary::MODE modeOffset = XBinary::getWidthModeFromSize(g_options.memoryMap.nBinarySize);

        XBinary::setPdStructTotal(g_pPdStruct, g_nFreeIndex, nNumberOfRecords);

        (*g_ppModel)->setHeaderData(0, Qt::Horizontal, tr("Offset"));
        (*g_ppModel)->setHeaderData(1, Qt::Horizontal, tr("Address"));
        (*g_ppModel)->setHeaderData(2, Qt::Horizontal, tr("Name"));

        for (qint32 i = 0; (i < nNumberOfRecords) && (!(g_pPdStruct->bIsStop)); i++) {
            XBinary::MS_RECORD record = g_pListRecords->at(i);

            XADDR nAddress = XBinary::offsetToAddress(&(g_options.memoryMap), record.nOffset);

            {
                QStandardItem *pItem = new QStandardItem;
                pItem->setText((XBinary::valueToHex(modeOffset, record.nOffset)));

                pItem->setData(record.nOffset, Qt::UserRole + USERROLE_OFFSET);
                pItem->setData(record.nSize, Qt::UserRole + USERROLE_SIZE);
                pItem->setData(record.sString, Qt::UserRole + USERROLE_STRING);
                pItem->setData(record.sInfo, Qt::UserRole + USERROLE_INFO);
                pItem->setData(nAddress, Qt::UserRole + USERROLE_ADDRESS);

                (*g_ppModel)->setItem(i, 0, pItem);
            }
            {
                QStandardItem *pItem = new QStandardItem;

                if (nAddress != (XADDR)-1) {
                    pItem->setText(XBinary::valueToHex(modeAddress, nAddress));
                }

                (*g_ppModel)->setItem(i, 1, pItem);
            }
            {
                QString sName = record.sInfo;

                if (sName == "") {
                    sName = record.sString;
                }

                (*g_ppModel)->setItem(i, 2, new QStandardItem(sName));

                XBinary::setPdStructCurrent(g_pPdStruct, g_nFreeIndex, i);
            }
        }

        XOptions::setModelTextAlignment((*g_ppModel), 0, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment((*g_ppModel), 1, Qt::AlignRight | Qt::AlignVCenter);
        XOptions::setModelTextAlignment((*g_ppModel), 2, Qt::AlignLeft | Qt::AlignVCenter);
    }

    XBinary::setPdStructFinished(g_pPdStruct, g_nFreeIndex);

    emit completed(scanTimer.elapsed());
}

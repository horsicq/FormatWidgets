/* Copyright (c) 2025 hors<horsicq@gmail.com>
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
#include "xgetdatarecordsprocess.h"

XGetDataRecordsProcess::XGetDataRecordsProcess(QObject *pParent) : XThreadObject(pParent)
{
    m_pDevice = nullptr;
    m_pListDataRecordsRows = nullptr;
    m_pListTitles = nullptr;
    m_pPdStruct = nullptr;
}

void XGetDataRecordsProcess::setData(QIODevice *pDevice, const XBinary::DATA_RECORDS_OPTIONS &dataRecordsOptions, QList<XBinary::DATA_RECORD_ROW> *pListDataRecordsRows,
                                     QList<QString> *pListTitles, XBinary::PDSTRUCT *pPdStruct)
{
    m_pDevice = pDevice;
    m_dataRecordsOptions = dataRecordsOptions;
    m_pListDataRecordsRows = pListDataRecordsRows;
    m_pListTitles = pListTitles;
    m_pPdStruct = pPdStruct;
}

void XGetDataRecordsProcess::process()
{
    XFormats::getDataRecordValues(m_dataRecordsOptions.pMemoryMap->fileType, m_pDevice, m_dataRecordsOptions, m_pListDataRecordsRows, m_pListTitles, false, -1,
                                  m_pPdStruct);
}

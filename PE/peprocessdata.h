/* Copyright (c) 2017-2025 hors<horsicq@gmail.com>
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
#ifndef PEPROCESSDATA_H
#define PEPROCESSDATA_H

#include "pe_defs.h"
#include "processdata.h"
#include "specabstract.h"

class PEProcessData : public ProcessData {
    Q_OBJECT

public:
    explicit PEProcessData(qint32 nType, QStandardItemModel **ppModel, XPE *pPE, qint32 nNumber, qint64 nOffset, qint64 nSize, QVariant varInfo = QVariant());

    virtual void _process();
    virtual void ajustTableView(qint32 nType, QTableView *pTableView);
    virtual void ajustTreeView(qint32 nType, QTreeView *pTreeView);
    void handleCertRecord(QStandardItem *pParent, XPE::CERT_RECORD certRecord);
    virtual void adjustModel(QStandardItemModel *pModel);

private:
    qint32 g_nType;
    XPE *g_pPE;
    QStandardItemModel **g_ppModel;
    qint32 g_nNumber;
    qint64 g_nOffset;
    qint64 g_nSize;
    QVariant g_varInfo;
};

#endif  // PEPROCESSDATA_H

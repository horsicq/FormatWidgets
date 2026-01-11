/* Copyright (c) 2017-2026 hors<horsicq@gmail.com>
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
#ifndef NEPROCESSDATA_H
#define NEPROCESSDATA_H

#include "ne_defs.h"
#include "processdata.h"

class NEProcessData : public ProcessData {
    Q_OBJECT

public:
    explicit NEProcessData(qint32 nType, QStandardItemModel **ppModel, XNE *pNE, qint64 nOffset, qint64 nSize);
    virtual void _process();
    virtual void ajustTableView(qint32 nType, QTableView *pTableView);

private:
    qint32 m_nType;
    XNE *m_pNE;
    QStandardItemModel **m_ppModel;
    qint64 m_nOffset;
    qint64 m_nSize;
};

#endif  // NEPROCESSDATA_H

/* Copyright (c) 2017-2023 hors<horsicq@gmail.com>
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
#ifndef LEPROCESSDATA_H
#define LEPROCESSDATA_H

#include "le_defs.h"
#include "processdata.h"

class LEProcessData : public ProcessData {
    Q_OBJECT

public:
    explicit LEProcessData(int nType, QStandardItemModel **ppModel, XLE *pLE, qint64 nOffset, qint64 nSize);

    virtual void _process();
    virtual void ajustTableView(QWidget *pWidget, QTableView *pTableView);

private:
    qint32 g_nType;
    XLE *g_pLE;
    QStandardItemModel **g_ppModel;
    qint64 g_nOffset;
    qint64 g_nSize;
};

#endif  // LEPROCESSDATA_H

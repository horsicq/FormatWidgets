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
#ifndef XDIALOGPROCESSDATA_H
#define XDIALOGPROCESSDATA_H

#include <QDialog>
#include <QMessageBox>
#include <QThread>

#include "xprocessdata.h"
#include "xdialogprocess.h"

class XDialogProcessData : public XDialogProcess {
    Q_OBJECT

public:
    explicit XDialogProcessData(QWidget *pParent);
    ~XDialogProcessData();
    void setData(QStandardItemModel **ppModel, QList<XFW_DEF::HEADER_RECORD> *pListHeaderRecords, XFW_DEF::CWOPTIONS *pCwOptions);

private:
    XProcessData *g_pProcessData;
    QThread *g_pThread;
};

#endif  // XDIALOGPROCESSDATA_H

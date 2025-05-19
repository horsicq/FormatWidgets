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
#include "xdialogprocessdata.h"

XDialogProcessData::XDialogProcessData(QWidget *pParent) : XDialogProcess(pParent)
{
    g_pProcessData = new XProcessData;
    g_pThread = new QThread;

    g_pProcessData->moveToThread(g_pThread);

    connect(g_pThread, SIGNAL(started()), g_pProcessData, SLOT(process()));
    connect(g_pProcessData, SIGNAL(completed(qint64)), this, SLOT(onCompleted(qint64)));
    connect(g_pProcessData, SIGNAL(errorMessage(QString)), this, SLOT(errorMessageSlot(QString)));

    g_pThread->start();
}

XDialogProcessData::~XDialogProcessData()
{
    stop();
    waitForFinished();

    g_pThread->quit();
    g_pThread->wait();

    delete g_pThread;
    delete g_pProcessData;
}

void XDialogProcessData::setData(QStandardItemModel **ppModel, QList<XFW_DEF::HEADER_RECORD> *pListHeaderRecords, XFW_DEF::CWOPTIONS *pCwOptions)
{
    g_pProcessData->setData(ppModel, pListHeaderRecords, pCwOptions, getPdStruct());
    g_pThread->start();
}

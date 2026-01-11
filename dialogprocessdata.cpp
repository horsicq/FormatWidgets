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
#include "dialogprocessdata.h"

DialogProcessData::DialogProcessData(QWidget *pParent, ProcessData *pProcessData, XOptions *pOptions) : XDialogProcess(pParent)
{
    this->m_pProcessData = pProcessData;

    pProcessData->setPdStruct(getPdStruct());
    pProcessData->setOptions(pOptions);

    m_pThread = new QThread;

    pProcessData->moveToThread(m_pThread);

    connect(m_pThread, SIGNAL(started()), pProcessData, SLOT(process()));
    connect(pProcessData, SIGNAL(completed(qint64)), this, SLOT(onCompleted(qint64)));
    connect(pProcessData, SIGNAL(errorMessage(QString)), this, SLOT(errorMessageSlot(QString)));

    m_pThread->start();
}

DialogProcessData::~DialogProcessData()
{
    stop();
    waitForFinished();

    m_pThread->quit();
    m_pThread->wait();

    delete m_pThread;
}

/* Copyright (c) 2019-2022 hors<horsicq@gmail.com>
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
#include "dialogmultisearchprocess.h"

DialogMultiSearchProcess::DialogMultiSearchProcess(QWidget *pParent) :
    XDialogProcess(pParent)
{
    g_type=MultiSearch::TYPE_STRINGS;

    g_pHandleSearch=new MultiSearch;
    g_pHandleModel=new MultiSearch;
    g_pThreadSearch=new QThread;
    g_pThreadModel=new QThread;

    g_pHandleSearch->moveToThread(g_pThreadSearch);
    g_pHandleModel->moveToThread(g_pThreadModel);

    connect(g_pThreadSearch,SIGNAL(started()),g_pHandleSearch,SLOT(processSearch()));
    connect(g_pHandleSearch,SIGNAL(completed(qint64)),this,SLOT(onCompleted(qint64)));
    connect(g_pHandleSearch,SIGNAL(errorMessage(QString)),this,SLOT(errorMessage(QString)));

    connect(g_pThreadModel,SIGNAL(started()),g_pHandleModel,SLOT(processModel()));
    connect(g_pHandleModel,SIGNAL(completed(qint64)),this,SLOT(onCompleted(qint64)));
    connect(g_pHandleModel,SIGNAL(errorMessage(QString)),this,SLOT(errorMessage(QString)));
}

DialogMultiSearchProcess::~DialogMultiSearchProcess()
{
    stop();
    waitForFinished();

    g_pThreadSearch->quit();
    g_pThreadSearch->wait();

    g_pThreadModel->quit();
    g_pThreadModel->wait();

    delete g_pThreadSearch;
    delete g_pThreadModel;
    delete g_pHandleSearch;
    delete g_pHandleModel;
}

void DialogMultiSearchProcess::processSearch(QIODevice *pDevice,QList<XBinary::MS_RECORD> *pListRecords,MultiSearch::OPTIONS options,MultiSearch::TYPE type)
{
    g_type=type;

    if(type==MultiSearch::TYPE_STRINGS)
    {
        setWindowTitle(tr("Search strings"));
    }
    else if(type==MultiSearch::TYPE_SIGNATURES)
    {
        setWindowTitle(tr("Search signatures"));
    }

    g_pHandleSearch->setSearchData(pDevice,pListRecords,options,type,getPdStruct());
    g_pThreadSearch->start();
}

void DialogMultiSearchProcess::processModel(QList<XBinary::MS_RECORD> *pListRecords,QStandardItemModel **ppModel,MultiSearch::OPTIONS options,MultiSearch::TYPE type)
{
    setWindowTitle(tr("Create view model"));

    g_pHandleModel->setModelData(pListRecords,ppModel,options,type,getPdStruct());
    g_pThreadModel->start();
}

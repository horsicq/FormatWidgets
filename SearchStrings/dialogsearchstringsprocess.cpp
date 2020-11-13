// copyright (c) 2019-2020 hors<horsicq@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#include "dialogsearchstringsprocess.h"
#include "ui_dialogsearchstringsprocess.h"

DialogSearchStringsProcess::DialogSearchStringsProcess(QWidget *pParent) :
    QDialog(pParent),
    ui(new Ui::DialogSearchStringsProcess)
{
    ui->setupUi(this);

    g_pHandleStrings=new SearchStrings;
    g_pHandleModel=new SearchStrings;
    g_pThreadSearch=new QThread;
    g_pThreadModel=new QThread;

    g_pHandleStrings->moveToThread(g_pThreadSearch);
    g_pHandleModel->moveToThread(g_pThreadModel);

    connect(g_pThreadSearch, SIGNAL(started()), g_pHandleStrings, SLOT(processSearch()));
    connect(g_pHandleStrings, SIGNAL(completed(qint64)), this, SLOT(onCompleted(qint64)));
    connect(g_pHandleStrings, SIGNAL(errorMessage(QString)), this, SLOT(errorMessage(QString)));
    connect(g_pHandleStrings, SIGNAL(progressValue(qint32)), this, SLOT(progressValue(qint32)));

    connect(g_pThreadModel, SIGNAL(started()), g_pHandleModel, SLOT(processModel()));
    connect(g_pHandleModel, SIGNAL(completed(qint64)), this, SLOT(onCompleted(qint64)));
    connect(g_pHandleModel, SIGNAL(errorMessage(QString)), this, SLOT(errorMessage(QString)));
    connect(g_pHandleModel, SIGNAL(progressValue(qint32)), this, SLOT(progressValue(qint32)));

    ui->progressBar->setMaximum(100);
    ui->progressBar->setMinimum(0);
}

DialogSearchStringsProcess::~DialogSearchStringsProcess()
{
    g_pHandleStrings->stop();
    g_pHandleModel->stop();

    g_pThreadSearch->quit();
    g_pThreadSearch->wait();

    g_pThreadModel->quit();
    g_pThreadModel->wait();

    delete ui;

    delete g_pThreadSearch;
    delete g_pThreadModel;
    delete g_pHandleStrings;
    delete g_pHandleModel;
}

void DialogSearchStringsProcess::processSearch(QIODevice *pDevice, QList<SearchStrings::RECORD> *pListRecords, SearchStrings::OPTIONS *pOptions)
{
    setWindowTitle(tr("Search strings"));

    g_pHandleStrings->setSearchData(pDevice,pListRecords,pOptions);
    g_pThreadSearch->start();
}

void DialogSearchStringsProcess::processModel(QList<SearchStrings::RECORD> *pListRecords, QStandardItemModel **ppModel, SearchStrings::OPTIONS *pOptions)
{
    setWindowTitle(tr("Create view model"));

    g_pHandleModel->setModelData(pListRecords,ppModel,pOptions);
    g_pThreadModel->start();
}

void DialogSearchStringsProcess::on_pushButtonCancel_clicked()
{
    g_pHandleStrings->stop();
    g_pHandleModel->stop();
}

void DialogSearchStringsProcess::errorMessage(QString sText)
{
    QMessageBox::critical(this,tr("Error"),sText);
}

void DialogSearchStringsProcess::onCompleted(qint64 nElapsed)
{
    Q_UNUSED(nElapsed)

    this->close();
}

void DialogSearchStringsProcess::progressValue(qint32 nValue)
{
    ui->progressBar->setValue(nValue);
}

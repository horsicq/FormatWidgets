// copyright (c) 2019-2021 hors<horsicq@gmail.com>
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
#include "dialogmultisearchprocess.h"
#include "ui_dialogmultisearchprocess.h"

DialogMultiSearchProcess::DialogMultiSearchProcess(QWidget *pParent) :
    QDialog(pParent),
    ui(new Ui::DialogMultiSearchProcess)
{
    ui->setupUi(this);

    g_pHandleSearch=new MultiSearch;
    g_pHandleModel=new MultiSearch;
    g_pThreadSearch=new QThread;
    g_pThreadModel=new QThread;

    g_pHandleSearch->moveToThread(g_pThreadSearch);
    g_pHandleModel->moveToThread(g_pThreadModel);

    connect(g_pThreadSearch, SIGNAL(started()), g_pHandleSearch, SLOT(processSearch()));
    connect(g_pHandleSearch, SIGNAL(completed(qint64)), this, SLOT(onCompleted(qint64)));
    connect(g_pHandleSearch, SIGNAL(errorMessage(QString)), this, SLOT(errorMessage(QString)));
    connect(g_pHandleSearch, SIGNAL(progressValueChanged(qint32)), this, SLOT(onProgressValueChanged(qint32)));

    connect(g_pThreadModel, SIGNAL(started()), g_pHandleModel, SLOT(processModel()));
    connect(g_pHandleModel, SIGNAL(completed(qint64)), this, SLOT(onCompleted(qint64)));
    connect(g_pHandleModel, SIGNAL(errorMessage(QString)), this, SLOT(errorMessage(QString)));
    connect(g_pHandleModel, SIGNAL(progressValueChanged(qint32)), this, SLOT(onProgressValueChanged(qint32)));

    ui->progressBar->setMaximum(100);
    ui->progressBar->setMinimum(0);
}

DialogMultiSearchProcess::~DialogMultiSearchProcess()
{
    g_pHandleSearch->stop();
    g_pHandleModel->stop();

    g_pThreadSearch->quit();
    g_pThreadSearch->wait();

    g_pThreadModel->quit();
    g_pThreadModel->wait();

    delete ui;

    delete g_pThreadSearch;
    delete g_pThreadModel;
    delete g_pHandleSearch;
    delete g_pHandleModel;
}

void DialogMultiSearchProcess::processSearch(QIODevice *pDevice, QList<XBinary::MS_RECORD> *pListRecords, MultiSearch::OPTIONS *pOptions)
{
    setWindowTitle(tr("Search strings"));

    g_pHandleSearch->setSearchData(pDevice,pListRecords,pOptions);
    g_pThreadSearch->start();
}

void DialogMultiSearchProcess::processModel(QList<XBinary::MS_RECORD> *pListRecords, QStandardItemModel **ppModel, MultiSearch::OPTIONS *pOptions)
{
    setWindowTitle(tr("Create view model"));

    g_pHandleModel->setModelData(pListRecords,ppModel,pOptions);
    g_pThreadModel->start();
}

void DialogMultiSearchProcess::on_pushButtonCancel_clicked()
{
    g_pHandleSearch->stop();
    g_pHandleModel->stop();
}

void DialogMultiSearchProcess::errorMessage(QString sText)
{
    QMessageBox::critical(this,tr("Error"),sText);
}

void DialogMultiSearchProcess::onCompleted(qint64 nElapsed)
{
    Q_UNUSED(nElapsed)

    this->close();
}

void DialogMultiSearchProcess::onProgressValueChanged(qint32 nValue)
{
    ui->progressBar->setValue(nValue);
}

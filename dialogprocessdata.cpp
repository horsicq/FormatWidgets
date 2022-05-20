/* Copyright (c) 2017-2022 hors<horsicq@gmail.com>
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
#include "dialogprocessdata.h"
#include "ui_dialogprocessdata.h"

DialogProcessData::DialogProcessData(QWidget *pParent,ProcessData *pProcessData) :
    QDialog(pParent),
    ui(new Ui::DialogProcessData)
{
    ui->setupUi(this);

    this->g_pProcessData=pProcessData;

    g_pThread=new QThread;

    pProcessData->moveToThread(g_pThread);

    connect(g_pThread,SIGNAL(started()),pProcessData,SLOT(process()));
    connect(pProcessData,SIGNAL(completed(qint64)),this,SLOT(onCompleted(qint64)));
    connect(pProcessData,SIGNAL(errorMessage(QString)),this,SLOT(errorMessage(QString)));
    connect(pProcessData,SIGNAL(progressValue(qint32)),this,SLOT(progressValue(qint32)));
    connect(pProcessData,SIGNAL(progressMinimum(qint32)),this,SLOT(progressMinimum(qint32)));
    connect(pProcessData,SIGNAL(progressMaximum(qint32)),this,SLOT(progressMaximum(qint32)));

    g_pThread->start();
}

DialogProcessData::~DialogProcessData()
{
    g_pProcessData->stop();

    g_pThread->quit();
    g_pThread->wait();

    delete ui;
    delete g_pThread;
}

void DialogProcessData::on_pushButtonCancel_clicked()
{
    g_pProcessData->stop();
}

void DialogProcessData::errorMessage(QString sText)
{
    QMessageBox::critical(XOptions::getMainWidget(this),tr("Error"),sText);
}

void DialogProcessData::onCompleted(qint64 nElapsed)
{
    Q_UNUSED(nElapsed)

    this->close();
}

void DialogProcessData::progressValue(qint32 nValue)
{
    ui->progressBar->setValue(nValue);
}

void DialogProcessData::progressMinimum(qint32 nValue)
{
    ui->progressBar->setMinimum(nValue);
}

void DialogProcessData::progressMaximum(qint32 nValue)
{
    ui->progressBar->setMaximum(nValue);
}

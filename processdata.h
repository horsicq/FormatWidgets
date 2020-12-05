// copyright (c) 2017-2020 hors<horsicq@gmail.com>
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
#ifndef PROCESSDATA_H
#define PROCESSDATA_H

#include <QObject>
#include <QTableView>
#include <QTreeView>
#include <QStandardItemModel>
#include <QElapsedTimer>
#include "formatwidget_def.h"
#include "xlineedithex.h"

class ProcessData : public QObject
{
    Q_OBJECT

public:
    explicit ProcessData();
    void stop();
    void setMaximum(quint64 nMaximum);
    void incValue();
    bool isRun();
    virtual void _process()=0;
    virtual void ajustTableView(QWidget *pWidget,QTableView *pTableView)=0;
    virtual void ajustTreeView(QWidget *pWidget,QTreeView *pTreeView);
    static QList<QString> getStructList(const FW_DEF::HEADER_RECORD *pRecords, int nRecordCount);
    static void setHeader(QStandardItemModel *pModel,QList<QString> *pListStrings);

public slots:
    void process();

signals:
    void errorMessage(QString sText);
    void completed(qint64 nElapsed);
    void progressValue(qint32 nValue);
    void progressMinimum(qint32 nValue);
    void progressMaximum(qint32 nValue);

private:
    bool g_bIsStop;
    quint64 g_nMaximum;
    quint64 _nValue;
    quint64 _nProcent;
    qint32 _nCurrentProcent;
};

#endif // PROCESSDATA_H

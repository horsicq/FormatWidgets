/* Copyright (c) 2017-2022 hors<horsicq@gmail.com>
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
#ifndef PROCESSDATA_H
#define PROCESSDATA_H

#include <QElapsedTimer>
#include <QStandardItemModel>
#include <QTableView>
#include <QTreeView>

#include "formatwidget_def.h"
#include "xbinary.h"
#include "xlineedithex.h"
#include "xoptions.h"
#include "staticscan.h"

class ProcessData : public QObject {
    Q_OBJECT

public:
    explicit ProcessData();

    void setPdStruct(XBinary::PDSTRUCT *pPdStruct);
    void setOptions(XOptions *pOptions);
    XOptions *getOptions();
    void setMaximum(quint64 nMaximum);
    void incValue();
    bool isRun();
    XBinary::PDSTRUCT *getPdStruct();
    virtual void _process() = 0;
    virtual void ajustTableView(QWidget *pWidget, QTableView *pTableView) = 0; // TODO remove pWidget use pTableView
    virtual void ajustTreeView(QWidget *pWidget, QTreeView *pTreeView); // TODO remove pWidget use pTableView
    static QList<QString> getStructList(const FW_DEF::HEADER_RECORD *pRecords, int nRecordCount);
    static void setTableHeader(QStandardItemModel *pModel, QList<QString> *pListStrings);
    static void setTreeHeader(QStandardItemModel *pModel, QList<QString> *pListStrings);
    virtual void adjustModel(QStandardItemModel *pModel);

public slots:
    void process();

signals:
    void errorMessage(QString sText);
    void completed(qint64 nElapsed);

private:
    XOptions *g_pOptions;
    XBinary::PDSTRUCT *g_pPdStruct;
    qint32 g_nFreeIndex;
};

#endif  // PROCESSDATA_H

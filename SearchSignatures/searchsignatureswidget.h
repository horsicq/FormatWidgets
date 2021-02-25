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
#ifndef SEARCHSIGNATURESWIDGET_H
#define SEARCHSIGNATURESWIDGET_H

#include "xshortcutswidget.h"
#include <QFile>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QFileDialog>
#include <QMenu>
#include <QApplication>
#include <QClipboard>
#include <QFutureWatcher>
#include <QtConcurrent>
#include "dialogmultisearchprocess.h"
#include "xformats.h"

namespace Ui
{
class SearchSignaturesWidget;
}

class SearchSignaturesWidget : public XShortcutsWidget
{
    Q_OBJECT

public:

    struct OPTIONS
    {
        QString sSignaturesPath;
        bool bMenu_Hex;
    };

    explicit SearchSignaturesWidget(QWidget *pParent=nullptr);
    ~SearchSignaturesWidget();
    void setData(QIODevice *pDevice,XBinary::FT fileType,OPTIONS options,bool bAuto=false,QWidget *pParent=nullptr);
    void setOptions(OPTIONS options);
    OPTIONS getOptions();
    void setSignaturesPath(QString sPath);

    void reload();
    bool getInitStatus();

private slots:
    void on_pushButtonSave_clicked();
    void on_pushButtonSearch_clicked();
    void on_tableViewResult_customContextMenuRequested(const QPoint &pos);
    void _copyAddress();
    void _copyOffset();
    void _copyName();
    void _copySignature();
    void _hex();
    void search();
    void deleteOldModel();
    void loadSignatures(QString sFileName);
    void on_comboBoxFile_currentIndexChanged(int index);

protected:
    virtual void registerShortcuts(bool bState);

signals:
    void showHex(qint64 nOffset,qint64 nSize);

private:
    Ui::SearchSignaturesWidget *ui;
    QIODevice *g_pDevice;
    QWidget *g_pParent;
    QSortFilterProxyModel *g_pFilter;
    XBinary::FT g_fileType;
    QStandardItemModel *g_pModel;
    bool g_bInit;
    QStandardItemModel *g_pOldModel;
    QFutureWatcher<void> g_watcher;
    QList<MultiSearch::SIGNATURE_RECORD> g_listSignatureRecords;
    QString g_sPath;
    OPTIONS g_options;

    QShortcut *g_scCopyName;
    QShortcut *g_scCopySignature;
    QShortcut *g_scCopyAddress;
    QShortcut *g_scCopyOffset;
    QShortcut *g_scHex;
};

#endif // SEARCHSIGNATURESWIDGET_H

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
#ifndef SEARCHSTRINGSWIDGET_H
#define SEARCHSTRINGSWIDGET_H

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
#include "xshortcutswidget.h"

namespace Ui
{
class SearchStringsWidget;
}

class SearchStringsWidget : public XShortcutsWidget
{
    Q_OBJECT

public:

    struct OPTIONS
    {
        qint64 nBaseAddress;
        bool bAnsi; // TODO
        bool bUnicode; // TODO
        qint32 nMinLenght;
        bool bMenu_Hex;
    };

    explicit SearchStringsWidget(QWidget *pParent=nullptr);
    ~SearchStringsWidget();
    void setData(QIODevice *pDevice,SearchStringsWidget::OPTIONS options,bool bAuto=false,QWidget *pParent=nullptr);

    void reload();
    bool getInitStatus();

private slots:
    void on_pushButtonSave_clicked();
    void on_pushButtonSearch_clicked();
    void on_lineEditFilter_textChanged(const QString &sText);
    void filter(QString sString);
    void on_tableViewResult_customContextMenuRequested(const QPoint &pos);
    void _copyString();
    void _copyOffset();
    void _copySize();
    void _hex();
    void search();
    void deleteOldModel();

protected:
    virtual void registerShortcuts(bool bState);

signals:
    void showHex(qint64 nOffset,qint64 nSize);

private:
    Ui::SearchStringsWidget *ui;
    QIODevice *g_pDevice;
    QWidget *g_pParent;
    QSortFilterProxyModel *g_pFilter;
    SearchStringsWidget::OPTIONS g_options;
    QStandardItemModel *g_pModel;
    bool g_bInit;
    QStandardItemModel *g_pOldModel;
    QFutureWatcher<void> g_watcher;
    QShortcut *g_scCopyString;
    QShortcut *g_scCopyOffset;
    QShortcut *g_scCopySize;
    QShortcut *g_scHex;
};

#endif // SEARCHSTRINGSWIDGET_H

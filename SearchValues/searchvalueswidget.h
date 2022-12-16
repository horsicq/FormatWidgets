/* Copyright (c) 2019-2022 hors<horsicq@gmail.com>
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
#ifndef SEARCHVALUESWIDGET_H
#define SEARCHVALUESWIDGET_H

#include <QApplication>
#include <QClipboard>
#include <QFileDialog>
#include <QFutureWatcher>
#include <QMenu>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QtConcurrent>

#include "dialogmultisearchprocess.h"
#include "xshortcutswidget.h"
#include "dialogsearch.h"

namespace Ui {
class SearchValuesWidget;
}

class SearchValuesWidget : public XShortcutsWidget {
    Q_OBJECT

    enum SC {
        __SC_DUMMY = 0,
        // TODO
        __SC_SIZE
    };

public:
    explicit SearchValuesWidget(QWidget *pParent = nullptr);
    ~SearchValuesWidget();

    void setData(QIODevice *pDevice, XBinary::FT fileType);
    QIODevice *getDevice();

    void reload();
    bool getInitStatus();

private slots:
    void on_pushButtonSave_clicked();
    void on_tableViewResult_customContextMenuRequested(const QPoint &pos);
    void _copyString();
    void _copyOffset();
    void _copySize();
    void _hex();
    void search();
    void deleteOldModel();
    void adjust();
    void on_pushButtonSearchString_clicked();
    void on_pushButtonSearchSignature_clicked();
    void on_pushButtonSearchValue_clicked();

private:
    void _search(DialogSearch::SEARCHMODE mode);

protected:
    virtual void registerShortcuts(bool bState);

signals:
    void showHex(qint64 nOffset, qint64 nSize);
    void showDemangle(QString sString);
    void dataChanged();

private:
    Ui::SearchValuesWidget *ui;
    QIODevice *g_pDevice;
    QStandardItemModel *g_pModel;
    bool g_bInit;
    QStandardItemModel *g_pOldModel;
    QFutureWatcher<void> g_watcher;
    QShortcut *shortCuts[__SC_SIZE];
    bool g_bIsReadonly;
};

#endif  // SEARCHVALUESWIDGET_H

/* Copyright (c) 2019-2025 hors<horsicq@gmail.com>
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
#ifndef SEARCHSIGNATURESWIDGET_H
#define SEARCHSIGNATURESWIDGET_H

#include <QApplication>
#include <QClipboard>
#include <QFile>
#include <QFileDialog>
#include <QFutureWatcher>
#include <QMenu>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>

#include "multisearch.h"
#include "xdialogprocess.h"
#include "xformats.h"
#include "xshortcutswidget.h"
#include "xmodel_msrecord.h"

namespace Ui {
class SearchSignaturesWidget;
}

class SearchSignaturesWidget : public XShortcutsWidget {
    Q_OBJECT

public:
    struct OPTIONS {
        XBinary::FT fileType;
        bool bMenu_Hex;
        bool bMenu_Disasm;
        XADDR nStartAddress;
    };

    explicit SearchSignaturesWidget(QWidget *pParent = nullptr);
    ~SearchSignaturesWidget();

    void setData(QIODevice *pDevice, OPTIONS options, bool bAuto = false);
    OPTIONS getOptions();
    void updateSignaturesPath();

    void reload();
    bool getInitStatus();
    virtual void adjustView();
    virtual void reloadData(bool bSaveSelection);

private slots:
    void on_toolButtonSave_clicked();
    void on_toolButtonSearch_clicked();
    void on_tableViewResult_customContextMenuRequested(const QPoint &pos);
    void _hex();
    void _disasm();
    void search();
    void loadSignatures(const QString &sFileName);
    void on_comboBoxFile_currentIndexChanged(int nIndex);
    void on_tableViewSelection(const QItemSelection &itemSelected, const QItemSelection &itemDeselected);
    void on_tableViewResult_clicked(const QModelIndex &index);
    void viewSelection();
    void reloadFileType();
    void on_comboBoxType_currentIndexChanged(int nIndex);
    void on_toolButtonPatch_clicked();

private:
    Ui::SearchSignaturesWidget *ui;
    QIODevice *m_pDevice;
    bool g_bInit;
    // QStandardItemModel *g_pOldModel;
    // QFutureWatcher<void> g_watcher;
    QList<XBinary::SIGNATUREDB_RECORD> g_listSignatureRecords;
    OPTIONS g_options;
    QVector<XBinary::MS_RECORD> g_listRecords;
};

#endif  // SEARCHSIGNATURESWIDGET_H

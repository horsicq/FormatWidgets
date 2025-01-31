/* Copyright (c) 2019-2024 hors<horsicq@gmail.com>
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

#include "dialogmultisearchprocess.h"
#include "xformats.h"
#include "xshortcutswidget.h"
#include "xmodel_msrecord.h"

namespace Ui {
class SearchSignaturesWidget;
}

class SearchSignaturesWidget : public XShortcutsWidget {
    Q_OBJECT

    enum SC {
        SC_COPYNAME = 0,
        SC_COPYSIGNATURE,
        SC_COPYADDRESS,
        SC_COPYOFFSET,
        SC_HEX,
        __SC_SIZE
    };

public:
    struct OPTIONS {
        bool bMenu_Hex;
        QString sUserSignature;  // It is not global
    };

    explicit SearchSignaturesWidget(QWidget *pParent = nullptr);
    ~SearchSignaturesWidget();

    void setData(QIODevice *pDevice, XBinary::FT fileType, OPTIONS options, bool bAuto = false);
    void setOptions(OPTIONS options);
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
    void _copyAddress();
    void _copyOffset();
    void _copyName();
    void _copySignature();
    void _hex();
    void search();
    void loadSignatures(const QString &sFileName);
    void on_comboBoxFile_currentIndexChanged(int nIndex);

protected:
    virtual void registerShortcuts(bool bState);

private:
    Ui::SearchSignaturesWidget *ui;
    QIODevice *g_pDevice;
    QStandardItemModel *g_pModel;
    bool g_bInit;
    // QStandardItemModel *g_pOldModel;
    // QFutureWatcher<void> g_watcher;
    QList<MultiSearch::SIGNATURE_RECORD> g_listSignatureRecords;
    OPTIONS g_options;
    QShortcut *shortCuts[__SC_SIZE];
};

#endif  // SEARCHSIGNATURESWIDGET_H

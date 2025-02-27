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
#ifndef SEARCHVALUESWIDGET_H
#define SEARCHVALUESWIDGET_H

#include <QApplication>
#include <QClipboard>
#include <QFileDialog>
#include <QFutureWatcher>
#include <QMenu>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>

#include "dialogmultisearchprocess.h"
#include "xshortcutswidget.h"
#include "dialogsearch.h"
#include "xmodel_msrecord.h"

namespace Ui {
class SearchValuesWidget;
}

class SearchValuesWidget : public XShortcutsWidget {
    Q_OBJECT

public:
    struct OPTIONS {
        XBinary::FT fileType;
        bool bMenu_Hex;
        bool bMenu_Disasm;
        bool bScan;
        QVariant varValue;
        XBinary::VT valueType;
        XBinary::ENDIAN endian;
    };

    explicit SearchValuesWidget(QWidget *pParent = nullptr);
    ~SearchValuesWidget();

    void setData(QIODevice *pDevice, OPTIONS options);
    QIODevice *getDevice();

    void findValue(QVariant varValue, XBinary::VT valueType, XBinary::ENDIAN endian);
    void findValue(quint64 nValue, XBinary::ENDIAN endian);
    virtual void adjustView();
    virtual void reloadData(bool bSaveSelection);

private slots:
    void on_toolButtonSave_clicked();
    void on_tableViewResult_customContextMenuRequested(const QPoint &pos);
    void search();
    void on_toolButtonSearchString_clicked();
    void on_toolButtonSearchSignature_clicked();
    void on_toolButtonSearchValue_clicked();
    void on_toolButtonSearch_clicked();
    void _hex();
    void _disasm();
    void viewSelection();
    void on_tableViewSelection(const QItemSelection &itemSelected, const QItemSelection &itemDeselected);
    void on_tableViewResult_clicked(const QModelIndex &index);
    void on_comboBoxType_currentIndexChanged(int nIndex);

private:
    void _search(DialogSearch::SEARCHMODE mode);

private:
    Ui::SearchValuesWidget *ui;
    QIODevice *g_pDevice;
    OPTIONS g_options;
    // QStandardItemModel *g_pOldModel;
    // QFutureWatcher<void> g_watcher;
    QVector<XBinary::MS_RECORD> g_listRecords;

    QVariant g_varValue;
    XBinary::VT g_valueType;
    XBinary::ENDIAN g_endian;
};

#endif  // SEARCHVALUESWIDGET_H

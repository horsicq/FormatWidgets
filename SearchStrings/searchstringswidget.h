/* Copyright (c) 2019-2023 hors<horsicq@gmail.com>
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
#ifndef SEARCHSTRINGSWIDGET_H
#define SEARCHSTRINGSWIDGET_H

#include <QApplication>
#include <QClipboard>
#include <QFileDialog>
#include <QFutureWatcher>
#include <QMenu>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QtConcurrent>

#include "dialogeditstring.h"
#include "dialogmultisearchprocess.h"
#include "xshortcutswidget.h"

namespace Ui {
class SearchStringsWidget;
}

class SearchStringsWidget : public XShortcutsWidget {
    Q_OBJECT

    enum SC {
        SC_HEX = 0,
        SC_DEMANGLE,
        SC_EDITSTRING,
        __SC_SIZE
    };

public:
    struct OPTIONS {
        qint64 nBaseAddress;
        bool bAnsi;
        bool bUTF8;
        bool bUnicode;
        bool bCStrings;
        QString sANSICodec;
        qint32 nMinLenght;
        bool bLinks;
        bool bMenu_Hex;
        bool bMenu_Demangle;
        QString sTitle;  // For dialog
    };

    explicit SearchStringsWidget(QWidget *pParent = nullptr);
    ~SearchStringsWidget();

    void setData(QIODevice *pDevice, XBinary::FT fileType, SearchStringsWidget::OPTIONS options, bool bAuto = false);
    void setBackupDevice(QIODevice *pDevice);
    QIODevice *getDevice();
    QIODevice *getBackupDevice();

    void reload();
    bool getInitStatus();
    bool isEdited();
    bool saveBackup();
    void setReadonly(bool bState);
    bool isReadonly();
    void setReadonlyVisible(bool bState);

private slots:
    void on_pushButtonSave_clicked();
    void on_pushButtonSearch_clicked();
    void on_lineEditFilter_textChanged(const QString &sText);
    void filter(const QString &sString);
    void on_tableViewResult_customContextMenuRequested(const QPoint &pos);
    void _hex();
    void _demangle();
    void _editString();
    void search();
    void deleteOldModel();
    void on_checkBoxAnsi_stateChanged(int nArg);
    void on_checkBoxUTF8_stateChanged(int nArg);
    void on_checkBoxUnicode_stateChanged(int nArg);
    void adjust();
    void on_tableViewSelection(const QItemSelection &itemSelected, const QItemSelection &itemDeselected);
    void on_tableViewResult_clicked(const QModelIndex &index);
    void viewSelection();
    void on_checkBoxReadonly_toggled(bool bChecked);

protected:
    virtual void registerShortcuts(bool bState);

signals:
    void showHex(qint64 nOffset, qint64 nSize);
    void showDemangle(const QString &sString);
    void dataChanged(qint64 nDeviceOffset, qint64 nDeviceSize);
    void currentAddressChanged(XADDR nAddress, qint64 nSize);

private:
    Ui::SearchStringsWidget *ui;
    QIODevice *g_pDevice;
    QIODevice *g_pBackupDevice;
    QSortFilterProxyModel *g_pFilter;
    SearchStringsWidget::OPTIONS g_options;
    QStandardItemModel *g_pModel;
    bool g_bInit;
    QStandardItemModel *g_pOldModel;
    QFutureWatcher<void> g_watcher;
    QShortcut *g_shortCuts[__SC_SIZE];
    bool g_bIsReadonly;
};

#endif  // SEARCHSTRINGSWIDGET_H

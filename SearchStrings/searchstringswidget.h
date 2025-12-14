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
#ifndef SEARCHSTRINGSWIDGET_H
#define SEARCHSTRINGSWIDGET_H

#include <QApplication>
#include <QClipboard>
#include <QFileDialog>
#include <QFutureWatcher>
#include <QMenu>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>

#include "dialogeditstring.h"
#include "multisearch.h"
#include "xdialogprocess.h"
#include "xshortcutswidget.h"
#include "xmodel_msrecord.h"

namespace Ui {
class SearchStringsWidget;
}

class SearchStringsWidget : public XShortcutsWidget {
    Q_OBJECT

public:
    struct OPTIONS {
        qint64 nBaseAddress;
        bool bAnsi;
        // bool bUTF8;
        bool bUnicode;
        bool bNullTerminated;
        // QString sANSICodec;
        qint32 nMinLenght;
        bool bLinks;
        QString sMask;
        bool bMenu_Hex;
        bool bMenu_Disasm;
        bool bMenu_Demangle;
        QString sTitle;  // For dialog
    };

    explicit SearchStringsWidget(QWidget *pParent = nullptr);
    ~SearchStringsWidget();

    void setData(QIODevice *pDevice, XBinary::FT fileType, SearchStringsWidget::OPTIONS options, bool bAuto = false);
    QIODevice *getDevice();

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
    void _demangle();
    void _editString();
    void search();
    void on_checkBoxAnsi_stateChanged(int nArg);
    // void on_checkBoxUTF8_stateChanged(int nArg);
    void on_checkBoxUnicode_stateChanged(int nArg);
    void adjust();
    void on_tableViewSelection(const QItemSelection &itemSelected, const QItemSelection &itemDeselected);
    void on_tableViewResult_clicked(const QModelIndex &index);
    void viewSelection();
    void on_comboBoxType_currentIndexChanged(int nIndex);
    void on_lineEditMask_textChanged(const QString &sText);
    void on_checkBoxRegExp_stateChanged(int nArg);

signals:
    void showDemangle(const QString &sString);

private:
    Ui::SearchStringsWidget *ui;
    QIODevice *m_pDevice;
    SearchStringsWidget::OPTIONS m_options;
    QVector<XBinary::MS_RECORD> m_listRecords;
    bool m_bInit;
};

#endif  // SEARCHSTRINGSWIDGET_H

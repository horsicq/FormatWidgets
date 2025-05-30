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
#ifndef NEWIDGET_H
#define NEWIDGET_H

#include "../formatwidget.h"
#include "ne_defs.h"
#include "neprocessdata.h"
#include "nesectionheaderwidget.h"

namespace Ui {
class NEWidget;
}

class NEWidget : public FormatWidget {
    Q_OBJECT

public:
    NEWidget(QWidget *pParent = nullptr);
    NEWidget(QIODevice *pDevice, FW_DEF::OPTIONS options, QWidget *pParent = nullptr);
    ~NEWidget();

    virtual void clear();
    virtual void cleanup();
    virtual void reload();

protected:
    virtual SV _setValue(QVariant vValue, qint32 nStype, qint32 nNdata, qint32 nVtype, qint32 nPosition, qint64 nOffset);
    virtual void setReadonly(bool bState);
    virtual void blockSignals(bool bState);
    virtual void adjustHeaderTable(qint32 nType, QTableWidget *pTableWidget);
    virtual QString typeIdToString(qint32 nType);
    virtual void _showInDisasmWindowAddress(qint64 nAddress);
    virtual void _showInDisasmWindowOffset(qint64 nOffset);
    virtual void _showInMemoryMapWindowOffset(qint64 nOffset);
    virtual void _showInHexWindow(qint64 nOffset, qint64 nSize);
    virtual void _findValue(quint64 nValue, XBinary::ENDIAN endian);
    virtual void _widgetValueChanged(QVariant vValue);

private slots:
    virtual void reloadData(bool bSaveSelection);
    void on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *pItemCurrent, QTreeWidgetItem *pItemPrevious);
    void on_checkBoxReadonly_toggled(bool bChecked);
    void on_toolButtonReload_clicked();
    void enableButton();
    void on_tableWidget_DOS_HEADER_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn);
    void on_tableWidget_OS2_HEADER_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn);
    void on_toolButtonPrev_clicked();
    void on_toolButtonNext_clicked();
    void onTableView_SEGMENTS_currentRowChanged(const QModelIndex &current, const QModelIndex &previous);

private:
    enum CB {
        CB_DOS_HEADER_e_magic = 0,
        CB_OS2_HEADER_ne_magic,
        CB_OS2_HEADER_ne_flags,
        CB_OS2_HEADER_ne_exetype,
        CB_OS2_HEADER_ne_flagsothers,
        __CB_size
    };

    enum INV {
        INV_IMAGE_DOS_HEADER_e_lfanew,
        CB_OS2_HEADER_ne_enttab,
        CB_OS2_HEADER_ne_segtab,
        CB_OS2_HEADER_ne_rsrctab,
        CB_OS2_HEADER_ne_restab,
        CB_OS2_HEADER_ne_modtab,
        CB_OS2_HEADER_ne_imptab,
        CB_OS2_HEADER_ne_nrestab,
        __INV_size
    };

    Ui::NEWidget *ui;

    XLineEditHEX *g_lineEdit_DOS_HEADER[N_NE_DOS_HEADER::__data_size];
    XLineEditHEX *g_lineEdit_OS2_HEADER[N_OS2_HEADER::__data_size];
    XComboBoxEx *g_comboBox[__CB_size];

    InvWidget *g_invWidget[__INV_size];
    SubDevice *g_subDevice[SNE::__TYPE_size];
    QStandardItemModel *g_tvModel[SNE::__TYPE_size];
};

#endif  // NEWIDGET_H

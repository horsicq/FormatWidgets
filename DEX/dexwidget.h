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
#ifndef DEXWIDGET_H
#define DEXWIDGET_H

#include "../formatwidget.h"
#include "dex_defs.h"
#include "dexprocessdata.h"
#include "dexsectionheaderwidget.h"
#include "dialogsectionheader.h"

namespace Ui {
class DEXWidget;
}

class DEXWidget : public FormatWidget {
    Q_OBJECT

public:
    DEXWidget(QWidget *pParent = nullptr);
    DEXWidget(QIODevice *pDevice, FW_DEF::OPTIONS options, QWidget *pParent = nullptr);
    ~DEXWidget();

    virtual void clear();
    virtual void cleanup();
    virtual void reload();

protected:
    virtual SV _setValue(QVariant vValue, int nStype, int nNdata, int nVtype, int nPosition, qint64 nOffset);
    virtual void setReadonly(bool bState);
    virtual void blockSignals(bool bState);
    virtual void adjustHeaderTable(int nType, QTableWidget *pTableWidget);
    virtual QString typeIdToString(int nType);
    virtual void _showInMemoryMapWindowOffset(qint64 nOffset);
    virtual void _showInHexWindow(qint64 nOffset, qint64 nSize);

private slots:
    virtual void reloadData();
    void widgetValueChanged(quint64 nValue);
    void on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *pItemCurrent, QTreeWidgetItem *pItemPrevious);
    void on_checkBoxReadonly_toggled(bool bChecked);
    bool createSectionTable(int nType, QTableWidget *pTableWidget, const FW_DEF::HEADER_RECORD *pHeaderRecord, int nNumberOfRecords);
    void on_pushButtonReload_clicked();
    void enableButton();
    void on_tableWidget_Header_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn);
    void on_lineEditFilterStrings_textChanged(const QString &sString);
    void on_lineEditFilterTypes_textChanged(const QString &sString);
    void on_toolButtonPrev_clicked();
    void on_toolButtonNext_clicked();
    void on_pushButtonSave_STRING_ID_ITEM_clicked();
    void on_pushButtonSave_TYPE_ID_ITEM_clicked();
    void on_pushButtonSave_PROTO_ID_ITEM_clicked();
    void on_pushButtonSave_FIELD_ID_ITEM_clicked();
    void on_pushButtonSave_CLASS_DEF_ITEM_clicked();
    void on_pushButtonSave_METHOD_ID_ITEM_clicked();

private:
    // TODO rename
    enum CB { CB_Dex_Header_magic = 0, CB_Dex_Header_version, CB_Dex_Header_endian_tag, __CB_size };

    enum INV { INV_link = 0, INV_map, INV_string_ids, INV_type_ids, INV_proto_ids, INV_field_ids, INV_method_ids, INV_class_defs, INV_data, __INV_size };

    Ui::DEXWidget *ui;

    XLineEditHEX *g_lineEdit_HEADER[N_DEX_HEADER::__data_size];

    XComboBoxEx *g_comboBox[__CB_size];
    InvWidget *g_invWidget[__INV_size];

    SubDevice *g_subDevice[SDEX::__TYPE_size];
    QStandardItemModel *g_tvModel[SDEX::__TYPE_size];

    QSortFilterProxyModel *g_pFilterStrings;
    QSortFilterProxyModel *g_pFilterTypes;
};

#endif  // DEXWIDGET_H

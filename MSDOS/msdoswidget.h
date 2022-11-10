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
#ifndef MSDOSWIDGET_H
#define MSDOSWIDGET_H

#include "../formatwidget.h"
#include "msdos_defs.h"

namespace Ui {
class MSDOSWidget;
}

class MSDOSWidget : public FormatWidget {
    Q_OBJECT

   public:
    MSDOSWidget(QWidget *pParent = nullptr);
    MSDOSWidget(QIODevice *pDevice, FW_DEF::OPTIONS options,
                QWidget *pParent = nullptr);
    ~MSDOSWidget();

    virtual void clear();
    virtual void cleanup();
    virtual void reload();

   protected:
    virtual SV _setValue(QVariant vValue, int nStype, int nNdata, int nVtype,
                         int nPosition, qint64 nOffset);
    virtual void setReadonly(bool bState);
    virtual void blockSignals(bool bState);
    virtual void adjustHeaderTable(int nType, QTableWidget *pTableWidget);
    virtual QString typeIdToString(int nType);
    virtual void _showInDisasmWindowAddress(qint64 nAddress);
    virtual void _showInDisasmWindowOffset(qint64 nOffset);
    virtual void _showInMemoryMapWindowOffset(qint64 nOffset);
    virtual void _showInHexWindow(qint64 nOffset, qint64 nSize);

   private slots:
    virtual void reloadData();
    void widgetValueChanged(quint64 nValue);
    void on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *pItemCurrent,
                                              QTreeWidgetItem *pItemPrevious);
    void on_checkBoxReadonly_toggled(bool bChecked);
    void on_pushButtonReload_clicked();
    void enableButton();
    void on_tableWidget_DOS_HEADER_currentCellChanged(int nCurrentRow,
                                                      int nCurrentColumn,
                                                      int nPreviousRow,
                                                      int nPreviousColumn);
    void on_toolButtonPrev_clicked();
    void on_toolButtonNext_clicked();
    void on_pushButtonHex_clicked();
    void on_pushButtonDisasm_clicked();
    void on_pushButtonStrings_clicked();
    void on_pushButtonMemoryMap_clicked();
    void on_pushButtonEntropy_clicked();
    void on_pushButtonHeuristicScan_clicked();

   private:
    enum CB { CB_DOS_HEADER_e_magic = 0, __CB_size };

    Ui::MSDOSWidget *ui;

    XLineEditHEX *g_lineEdit_DOS_HEADER[N_DOS_HEADER::__data_size];
    XComboBoxEx *g_comboBox[__CB_size];

    SubDevice *g_subDevice[SMSDOS::__TYPE_size];
};

#endif  // MSDOSWIDGET_H

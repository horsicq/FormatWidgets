/* Copyright (c) 2020-2022 hors<horsicq@gmail.com>
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
#ifndef BINARYWIDGET_H
#define BINARYWIDGET_H

#include "../formatwidget.h"
#include "binary_defs.h"

namespace Ui {
class BinaryWidget;
}

class BinaryWidget : public FormatWidget {
    Q_OBJECT

   public:
    BinaryWidget(QWidget *pParent = nullptr);
    BinaryWidget(QIODevice *pDevice, FW_DEF::OPTIONS options,
                 QWidget *pParent = nullptr);
    ~BinaryWidget();

    virtual void clear();
    virtual void cleanup();
    virtual void reload();

   protected:
    virtual SV _setValue(QVariant vValue, int nStype, int nNdata, int nVtype,
                         int nPosition, qint64 nOffset);
    virtual void setReadonly(bool bState);
    virtual void blockSignals(bool bState);
    virtual void adjustHeaderTable(int nType, QTableWidget *pTableWidget);
    virtual void _showInDisasmWindowAddress(XADDR nAddress);
    virtual void _showInDisasmWindowOffset(qint64 nOffset);
    virtual void _showInMemoryMapWindowOffset(qint64 nOffset);
    virtual void _showInHexWindow(qint64 nOffset, qint64 nSize);

   private slots:
    virtual void reloadData();
    void widgetValueChanged(quint64 nValue);
    void on_checkBoxReadonly_toggled(bool bChecked);
    void on_pushButtonReload_clicked();
    void enableButton();
    void on_toolButtonPrev_clicked();
    void on_toolButtonNext_clicked();
    void on_pushButtonHex_clicked();
    void on_pushButtonDisasm_clicked();
    void on_pushButtonStrings_clicked();
    void on_pushButtonEntropy_clicked();
    void on_pushButtonHeuristicScan_clicked();
    void on_pushButtonMemoryMap_clicked();
    void on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *pItemCurrent,
                                              QTreeWidgetItem *pItemPrevious);

   private:
    Ui::BinaryWidget *ui;
};

#endif  // BINARYWIDGET_H

/* Copyright (c) 2017-2023 hors<horsicq@gmail.com>
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
#ifndef ELFSECTIONHEADERWIDGET_H
#define ELFSECTIONHEADERWIDGET_H

#include "../formatwidget.h"
#include "elf_defs.h"

namespace Ui {
class ELFSectionHeaderWidget;
}

class ELFSectionHeaderWidget : public FormatWidget {
    Q_OBJECT

public:
    ELFSectionHeaderWidget(QWidget *pParent = nullptr);
    ELFSectionHeaderWidget(QIODevice *pDevice, FW_DEF::OPTIONS options, quint32 nNumber, qint64 nOffset, qint32 nType, QWidget *pParent = nullptr);
    ~ELFSectionHeaderWidget();

    virtual void clear();
    virtual void cleanup();
    virtual void reload();
    virtual void setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions);
    void setStringTable(qint64 nStringTableOffset, qint64 nStringTableSize);

protected:
    virtual SV _setValue(QVariant vValue, int nStype, int nNdata, int nVtype, int nPosition, qint64 nOffset);
    virtual void setReadonly(bool bState);
    virtual void blockSignals(bool bState);
    virtual void adjustHeaderTable(int nType, QTableWidget *pTableWidget);

private slots:
    void on_checkBoxReadonly_toggled(bool bChecked);
    void reloadData();
    void widgetValueChanged(quint64 nValue);
    void on_tableWidget_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn);
    void on_pushButtonSaveHeader_clicked();

private:
    Ui::ELFSectionHeaderWidget *ui;
    XLineEditHEX **g_ppLinedEdit;
    qint32 g_nLineEditSize;
    XComboBoxEx **g_ppComboBox;
    qint32 g_nComboBoxSize;
    InvWidget **g_ppInvWidget;
    int g_nInvWidgetSize;
    SubDevice *g_pSubDevice;
    qint64 g_nStringTableOffset;
    qint64 g_nStringTableSize;
};

#endif  // ELFSECTIONHEADERWIDGET_H

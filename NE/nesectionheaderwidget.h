/* Copyright (c) 2017-2024 hors<horsicq@gmail.com>
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
#ifndef NESECTIONHEADERWIDGET_H
#define NESECTIONHEADERWIDGET_H

#include "../formatwidget.h"
#include "ne_defs.h"

namespace Ui {
class NESectionHeaderWidget;
}

class NESectionHeaderWidget : public FormatWidget {
    Q_OBJECT

public:
    enum CB {
        CB_CHARACTERISTICS = 0,
        CB_ALIGH,
        __CB_size
    };
    NESectionHeaderWidget(QWidget *pParent = nullptr);
    NESectionHeaderWidget(QIODevice *pDevice, FW_DEF::OPTIONS options, quint32 nNumber, qint64 nOffset, qint32 nType, QWidget *pParent = nullptr);
    ~NESectionHeaderWidget();

    virtual void clear();
    virtual void reload();

protected:
    virtual SV _setValue(QVariant vValue, qint32 nStype, qint32 nNdata, qint32 nVtype, qint32 nPosition, qint64 nOffset);
    virtual void setReadonly(bool bState);
    virtual void blockSignals(bool bState);
    virtual void adjustHeaderTable(qint32 nType, QTableWidget *pTableWidget);

private slots:
    void on_checkBoxReadonly_toggled(bool bChecked);
    void reloadData(bool bSaveSelection);

    void on_tableWidget_Section_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn);

private:
    enum INV {
        INV_OriginalFirstThunk,
        INV_Name,
        INV_FirstThunk,
        __INV_size
    };

    Ui::NESectionHeaderWidget *ui;
    //    XLineEditHEX *lineEdit_Section[N_mach_sections::__data_size];
    InvWidget *g_invWidget[__INV_size];
    SubDevice *g_pSubDevice;
};

#endif  // NESECTIONHEADERWIDGET_H

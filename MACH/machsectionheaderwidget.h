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
#ifndef MACHSECTIONHEADERWIDGET_H
#define MACHSECTIONHEADERWIDGET_H

#include "../formatwidget.h"
#include "mach_defs.h"

namespace Ui {
class MACHSectionHeaderWidget;
}

class MACHSectionHeaderWidget : public FormatWidget {
    Q_OBJECT

public:
    MACHSectionHeaderWidget(QWidget *pParent = nullptr);
    MACHSectionHeaderWidget(QIODevice *pDevice, FW_DEF::OPTIONS options, quint32 nNumber, qint64 nOffset, qint32 nType, QWidget *pParent = nullptr);
    ~MACHSectionHeaderWidget();

    virtual void clear();
    virtual void cleanup();
    virtual void reload();
    virtual void setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions);

protected:
    virtual SV _setValue(QVariant vValue, qint32 nStype, qint32 nNdata, qint32 nVtype, qint32 nPosition, qint64 nOffset);
    virtual void setReadonly(bool bState);
    virtual void blockSignals(bool bState);
    virtual void adjustHeaderTable(qint32 nType, QTableWidget *pTableWidget);
    virtual void _widgetValueChanged(QVariant vValue);

private slots:
    void on_checkBoxReadonly_toggled(bool bChecked);
    void reloadData(bool bSaveSelection);
    void on_tableWidget_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn);

private:
    Ui::MACHSectionHeaderWidget *ui;
    XLineEditHEX **g_ppLinedEdit;
    qint32 g_nLineEditSize;
    XComboBoxEx **g_ppComboBox;
    qint32 g_nComboBoxSize;
    InvWidget **g_ppInvWidget;
    qint32 g_nInvWidgetSize;
    SubDevice *g_pSubDevice;
};
#endif  // MACHSECTIONHEADERWIDGET_H

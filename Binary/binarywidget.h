/* Copyright (c) 2020-2025 hors<horsicq@gmail.com>
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
    BinaryWidget(QIODevice *pDevice, const FW_DEF::OPTIONS &options, QWidget *pParent = nullptr);
    ~BinaryWidget();

    virtual void clear();
    virtual void cleanup();
    virtual void reload();

protected:
    virtual SV _setValue(QVariant vValue, qint32 nStype, qint32 nNdata, qint32 nVtype, qint32 nPosition, qint64 nOffset);
    virtual void setReadonly(bool bState);
    virtual void blockSignals(bool bState);
    virtual void adjustHeaderTable(qint32 nType, QTableWidget *pTableWidget);
    virtual void _showInDisasmWindowAddress(XADDR nAddress);
    virtual void _showInDisasmWindowOffset(qint64 nOffset);
    virtual void _showInMemoryMapWindowOffset(qint64 nOffset);
    virtual void _showInHexWindow(qint64 nOffset, qint64 nSize);
    virtual void _findValue(quint64 nValue, XBinary::ENDIAN endian);
    virtual void _widgetValueChanged(QVariant vValue);

private slots:
    virtual void reloadData(bool bSaveSelection);
    void on_checkBoxReadonly_toggled(bool bChecked);
    void on_toolButtonReload_clicked();
    void enableButton();
    void on_toolButtonPrev_clicked();
    void on_toolButtonNext_clicked();
    void on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *pItemCurrent, QTreeWidgetItem *pItemPrevious);

private:
    Ui::BinaryWidget *ui;
};

#endif  // BINARYWIDGET_H

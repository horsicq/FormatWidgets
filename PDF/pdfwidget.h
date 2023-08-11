/* Copyright (c) 2022-2023 hors<horsicq@gmail.com>
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
#ifndef PDFWIDGET_H
#define PDFWIDGET_H

#include "../formatwidget.h"
#include "pdf_defs.h"

namespace Ui {
class PDFWidget;
}

class PDFWidget : public FormatWidget {
    Q_OBJECT

public:
    PDFWidget(QWidget *pParent = nullptr);
    PDFWidget(QIODevice *pDevice, FW_DEF::OPTIONS options, QWidget *pParent = nullptr);
    ~PDFWidget();

    virtual void clear();
    virtual void cleanup();
    virtual void reload();

protected:
    virtual SV _setValue(QVariant vValue, int nStype, int nNdata, int nVtype, int nPosition, qint64 nOffset);
    virtual void setReadonly(bool bState);
    virtual void blockSignals(bool bState);
    virtual void reloadData();
    virtual void _showInMemoryMapWindowOffset(qint64 nOffset);
    virtual void _showInHexWindow(qint64 nOffset, qint64 nSize);
    virtual void _findValue(quint64 nValue, bool bIsBigEndian);

private slots:
    void on_checkBoxReadonly_toggled(bool bChecked);

    void on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *pCurrent, QTreeWidgetItem *pPrevious);

private:
    Ui::PDFWidget *ui;
};

#endif  // PDFWIDGET_H

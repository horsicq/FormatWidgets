/* Copyright (c) 2019-2024 hors<horsicq@gmail.com>
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
#ifndef INVWIDGET_H
#define INVWIDGET_H

#include <QHBoxLayout>
#include <QPushButton>

#include "xbinary.h"
#include "xshortcutswidget.h"

// TODO changed signal XHexEdit
class InvWidget : public XShortcutsWidget {
    Q_OBJECT

public:
    enum TYPE {
        TYPE_HEX,
        TYPE_DISASM
    };

    explicit InvWidget(QWidget *pParent, TYPE type);
    ~InvWidget();

    void setOffsetAndSize(XBinary *pBinary, qint64 nOffset, qint64 nSize, bool bNotNull = false);
    void setAddressAndSize(XBinary *pBinary, XADDR nAddress, qint64 nSize, bool bNotNull = false);
    void _setEnabled(bool bState);

private slots:
    void showHexSlot();
    void showDisasmSlot();

signals:
    void showHex(qint64 nOffset, qint64 nSize);
    void showDisasm(XADDR nAddress);

protected:
    virtual void registerShortcuts(bool bState);

private:
    qint64 g_nAddress;
    qint64 g_nOffset;
    qint64 g_nSize;
    QPushButton *g_pHexPushButton;
    QPushButton *g_pDisasmPushButton;
};

#define PInvWidget InvWidget *

#endif  // INVWIDGET_H

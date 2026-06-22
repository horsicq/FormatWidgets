/* Copyright (c) 2025-2026 hors<horsicq@gmail.com>
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

#ifndef XFWIDGET_TABLE_H
#define XFWIDGET_TABLE_H

#include <QWidget>
#include "xftableview.h"
#include "xbinary.h"

namespace Ui {
class XFWidget_Table;
}

class XFWidget_Table : public QWidget {
    Q_OBJECT

public:
    explicit XFWidget_Table(QWidget *pParent = nullptr);
    virtual ~XFWidget_Table();

    void setData(XBinary *pXBinary, const XBinary::XFHEADER &xfHeader);
    void clear();

    void setReadonly(bool bIsReadonly);

    XFTableView *getTableView();

signals:
    void fieldSelected(qint32 nFieldIndex, QVariant value, const XBinary::XFRECORD &xfRecord);
    void fieldDoubleClicked(qint32 nFieldIndex, QVariant value, const XBinary::XFRECORD &xfRecord);

private slots:
    void onSaveClicked();
    void onShowOffsetsToggled(bool bChecked);
    void onShowPresentationToggled(bool bChecked);

private:
    Ui::XFWidget_Table *ui;
    XBinary *m_pXBinary;
    QString m_sCurrentTag;
    bool m_bIsReadonly;
};

#endif  // XFWIDGET_TABLE_H

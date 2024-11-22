/* Copyright (c) 2024 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef MACH_HEADERWIDGET_H
#define MACH_HEADERWIDGET_H

#include "formatwidget.h"

namespace Ui {
class mach_headerWidget;
}

class mach_headerWidget : public FormatWidget
{
    Q_OBJECT

public:
    explicit mach_headerWidget(QWidget *pParent = nullptr);
    ~mach_headerWidget();
    virtual void reloadData(bool bSaveSelection);

protected:
    virtual SV _setValue(QVariant vValue, qint32 nPosition);

private slots:
    void on_tableWidgetMain_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void on_tableWidgetMain_customContextMenuRequested(const QPoint &pos);

private:
    Ui::mach_headerWidget *ui;
};

#endif // MACH_HEADERWIDGET_H

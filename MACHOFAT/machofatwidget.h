/* Copyright (c) 2017-2025 hors<horsicq@gmail.com>
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
#ifndef MACHOFATWIDGET_H
#define MACHOFATWIDGET_H

#include "../MACH/machwidget.h"

namespace Ui {
class MACHOFATWidget;
}

class MACHOFATWidget : public FormatWidget {
    Q_OBJECT

public:
    explicit MACHOFATWidget(QWidget *pParent = nullptr);
    ~MACHOFATWidget();

    virtual void clear();
    virtual void cleanup();
    virtual void reload();

protected:
    virtual SV _setValue(QVariant vValue, qint32 nStype, qint32 nNdata, qint32 nVtype, qint32 nPosition, qint64 nOffset);
    virtual void setReadonly(bool bState);
    virtual void blockSignals(bool bState);

private slots:
    void reloadData(bool bSaveSelection);
    void on_comboBoxFilePart_currentIndexChanged(int nIndex);

private:
    Ui::MACHOFATWidget *ui;
    QList<SubDevice *> g_listDevices;
};

#endif  // MACHOFATWIDGET_H

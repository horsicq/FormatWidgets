// copyright (c) 2019 hors<horsicq@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#ifndef BINARYWIDGET_H
#define BINARYWIDGET_H

#include "../formatwidget.h"
#include "binary_defs.h"

namespace Ui {
class BinaryWidget;
}

class BinaryWidget : public FormatWidget
{
    Q_OBJECT

public:
    BinaryWidget(QWidget *parent = nullptr);
    BinaryWidget(QIODevice *pDevice, OPTIONS *pOptions, QWidget *parent = nullptr);
    ~BinaryWidget();
    virtual void clear();
    virtual void setData(QIODevice *pDevice,OPTIONS *pOptions);
protected:
    virtual bool _setValue(QVariant vValue,int nStype,int nNdata,int nVtype,int nPosition);
    virtual void setReadonly(bool bState);
    virtual void blockSignals(bool bState);
    virtual void adjustHeaderTable(int type,QTableWidget *pTableWidget);
private slots:
    virtual void reloadData();
    void widgetValueChanged(quint64 nValue);
    void on_checkBoxReadonly_toggled(bool checked);
private:

    Ui::BinaryWidget *ui;

    bool bInit;
};

#endif // BINARYWIDGET_H

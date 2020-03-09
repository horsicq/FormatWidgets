// copyright (c) 2017-2020 hors<horsicq@gmail.com>
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
#ifndef IMPORTHEADERWIDGET_H
#define IMPORTHEADERWIDGET_H

#include "../formatwidget.h"
#include "pe_defs.h"

namespace Ui
{
class ImportHeaderWidget;
}

class ImportHeaderWidget : public FormatWidget
{
    Q_OBJECT

public:
    enum CB
    {
        CB_CHARACTERISTICS=0,
        CB_ALIGH,
        __CB_size
    };
    ImportHeaderWidget(QWidget *parent=nullptr);
    ImportHeaderWidget(QIODevice *pDevice, FW_DEF::OPTIONS *pOptions, quint32 nNumber, QWidget *parent=nullptr);
    ~ImportHeaderWidget();
    virtual void clear();
    virtual void setData(QIODevice *pDevice, FW_DEF::OPTIONS *pOptions, quint32 nNumber);
    virtual void reload();

protected:
    virtual bool _setValue(QVariant vValue,int nStype,int nNdata,int nVtype,int nPosition);
    virtual void setReadonly(bool bState);
    virtual void blockSignals(bool bState);
    virtual void adjustHeaderTable(int type,QTableWidget *pTableWidget);

private slots:
    void on_checkBoxReadonly_toggled(bool checked);
    void reloadData();

    void on_tableWidget_IMPORT_HEADER_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

private:
    enum INV
    {
        INV_OriginalFirstThunk,
        INV_Name,
        INV_FirstThunk,
        __INV_size
    };

    Ui::ImportHeaderWidget *ui;
    XLineEditHEX *lineEdit_IMPORT_HEADER[N_IMAGE_SECTION_HEADER::__data_size+1];
    bool bInit;
    InvWidget *invWidget[__INV_size];
    SubDevice *pSubDevice;
};

#endif // IMPORTHEADERWIDGET_H

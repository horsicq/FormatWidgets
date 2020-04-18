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
#ifndef PROGRAMHEADERWIDGET_H
#define PROGRAMHEADERWIDGET_H

#include "../formatwidget.h"
#include "elf_defs.h"

namespace Ui {
class ProgramHeaderWidget;
}

class ProgramHeaderWidget : public FormatWidget
{
    Q_OBJECT

public:
    enum CB
    {
        CB_TYPE=0,
        CB_FLAGS,
        __CB_size
    };
    explicit ProgramHeaderWidget(QWidget *parent=nullptr);
    ~ProgramHeaderWidget();
    virtual void clear();
    virtual void reset();
    virtual void setData(QIODevice *pDevice, FW_DEF::OPTIONS *pOptions, quint32 __nNumber, qint64 nOffset);
    virtual void reload();

protected:
    virtual bool _setValue(QVariant vValue,int nStype,int nNdata,int nVtype,int nPosition,qint64 nOffset);
    virtual void setReadonly(bool bState);
    virtual void blockSignals(bool bState);
    virtual void adjustHeaderTable(int type,QTableWidget *pTableWidget);

private slots:
    void on_checkBoxReadonly_toggled(bool checked);
    void reloadData();
    void widgetValueChanged(quint64 nValue);

    void on_tableWidget_Elf_Phdr_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

private:
    enum INV
    {
        INV_VirtualAddress,
        INV_PointerToRawData,
        __INV_size
    };

    Ui::ProgramHeaderWidget *ui;
    XLineEditHEX *lineEdit_Elf_Phdr[N_Elf_Phdr32::__data_size];
    XComboBoxEx *comboBox[__CB_size];
    bool bInit;
    InvWidget *invWidget[__INV_size];
    SubDevice *pSubDevice;
};

#endif // PROGRAMHEADERWIDGET_H

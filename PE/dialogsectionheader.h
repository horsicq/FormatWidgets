// copyright (c) 2017-2019 hors<horsicq@gmail.com>
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
#ifndef DIALOGSECTIONHEADER_H
#define DIALOGSECTIONHEADER_H

#include "../dialogformat.h"
#include "pe_defs.h"


#include <QDialog>

namespace Ui {
class DialogSectionHeader;
}

class DialogSectionHeader : public DialogFormat
{
    Q_OBJECT

public:
    enum CB
    {
        CB_CHARACTERISTICS=0,
        CB_ALIGH,
        __CB_size
    };
    explicit DialogSectionHeader(QIODevice *pDevice, OPTIONS *pOptions, quint32 nNumber, QWidget *parent = nullptr);
    ~DialogSectionHeader();

protected:
    virtual bool _setValue(QVariant vValue,int nStype,int nNdata,int nVtype,int nPosition);
    virtual void setReadonly(bool bState);
    virtual void blockSignals(bool bState);
private slots:
    void on_checkBoxReadonly_toggled(bool checked);
    void reloadData();
    void widgetValueChanged(quint64 nValue);
private:
    Ui::DialogSectionHeader *ui;
    QLineEditHEX *lineEdit_IMAGE_SECTION_HEADER[N_IMAGE_SECTION_HEADER::__data_size+1];
    QComboBoxEx *comboBox[__CB_size];
    quint32 nNumber;
    bool bInit;
};

#endif // DIALOGSECTIONHEADER_H

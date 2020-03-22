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
#include "programheaderwidget.h"
#include "ui_programheaderwidget.h"

ProgramHeaderWidget::ProgramHeaderWidget(QWidget *parent) :
    FormatWidget(parent),
    ui(new Ui::ProgramHeaderWidget)
{
    ui->setupUi(this);
}

ProgramHeaderWidget::~ProgramHeaderWidget()
{
    delete ui;
}

void ProgramHeaderWidget::clear()
{

}

void ProgramHeaderWidget::setData(QIODevice *pDevice, FW_DEF::OPTIONS *pOptions, quint32 nNumber)
{
    FormatWidget::setData(pDevice,pOptions,nNumber);
}

void ProgramHeaderWidget::reload()
{
    clear();

    ui->checkBoxReadonly->setEnabled(!isReadonly());

    reloadData();
}

bool ProgramHeaderWidget::_setValue(QVariant vValue, int nStype, int nNdata, int nVtype, int nPosition)
{
    return false;
}

void ProgramHeaderWidget::setReadonly(bool bState)
{

}

void ProgramHeaderWidget::blockSignals(bool bState)
{

}

void ProgramHeaderWidget::adjustHeaderTable(int type, QTableWidget *pTableWidget)
{
    int nSymbolWidth=getSymbolWidth();

    pTableWidget->setColumnWidth(HEADER_COLUMN_OFFSET,nSymbolWidth*4);
    pTableWidget->setColumnWidth(HEADER_COLUMN_TYPE,nSymbolWidth*6);

    switch(type)
    {
        case SELF::TYPE_Elf_Phdr:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME,nSymbolWidth*16);
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE,nSymbolWidth*8);
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO,nSymbolWidth*16);
            break;
    }
}

void ProgramHeaderWidget::on_checkBoxReadonly_toggled(bool checked)
{

}

void ProgramHeaderWidget::reloadData()
{

}

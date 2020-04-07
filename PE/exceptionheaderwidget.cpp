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
#include "exceptionheaderwidget.h"
#include "ui_exceptionheaderwidget.h"

ExceptionHeaderWidget::ExceptionHeaderWidget(QWidget *parent):
    FormatWidget(parent),
    ui(new Ui::ExceptionHeaderWidget)
{
    ui->setupUi(this);
}

ExceptionHeaderWidget::ExceptionHeaderWidget(QIODevice *pDevice, FW_DEF::OPTIONS *pOptions, quint32 nNumber, qint64 nOffset, QWidget *parent):
    FormatWidget(pDevice,pOptions,nNumber,nOffset,parent),
    ui(new Ui::ExceptionHeaderWidget)
{
    ui->setupUi(this);

    setData(pDevice,pOptions,nNumber,nOffset);
}

ExceptionHeaderWidget::~ExceptionHeaderWidget()
{
    delete ui;
}

void ExceptionHeaderWidget::clear()
{
    bInit=false;

    memset(lineEdit_Exception,0,sizeof lineEdit_Exception);
    memset(invWidget,0,sizeof invWidget);

    pSubDevice=nullptr;

    ui->checkBoxReadonly->setChecked(true);
}

void ExceptionHeaderWidget::setData(QIODevice *pDevice, FW_DEF::OPTIONS *pOptions, quint32 nNumber, qint64 nOffset)
{
    FormatWidget::setData(pDevice,pOptions,nNumber,nOffset);
}

void ExceptionHeaderWidget::reload()
{

}

bool ExceptionHeaderWidget::_setValue(QVariant vValue, int nStype, int nNdata, int nVtype, int nPosition, qint64 nOffset)
{
    return false;
}

void ExceptionHeaderWidget::setReadonly(bool bState)
{

}

void ExceptionHeaderWidget::blockSignals(bool bState)
{

}

void ExceptionHeaderWidget::adjustHeaderTable(int type, QTableWidget *pTableWidget)
{

}

void ExceptionHeaderWidget::on_checkBoxReadonly_toggled(bool checked)
{

}

void ExceptionHeaderWidget::reloadData()
{

}

void ExceptionHeaderWidget::on_tableWidget_Exceptions_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{

}


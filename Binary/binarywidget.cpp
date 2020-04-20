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
#include "binarywidget.h"
#include "ui_binarywidget.h"

BinaryWidget::BinaryWidget(QWidget *parent) :
    FormatWidget(parent),
    ui(new Ui::BinaryWidget)
{
    ui->setupUi(this);
}

BinaryWidget::BinaryWidget(QIODevice *pDevice, FW_DEF::OPTIONS *pOptions, QWidget *parent) :
    FormatWidget(pDevice,pOptions,0,0,parent),
    ui(new Ui::BinaryWidget)
{
    ui->setupUi(this);

    setData(pDevice,pOptions,0,0);
    reload();
}

BinaryWidget::~BinaryWidget()
{
    delete ui;
}

void BinaryWidget::clear()
{
    reset();

    ui->checkBoxReadonly->setChecked(true);
}

void BinaryWidget::cleanup()
{

}

void BinaryWidget::reset()
{
    bInit=false;
}

void BinaryWidget::reload()
{
    clear();

    ui->checkBoxReadonly->setEnabled(!isReadonly());

    XBinary binary(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

    if(binary.isValid())
    {
        reloadData();
    }
}

bool BinaryWidget::_setValue(QVariant vValue, int nStype, int nNdata, int nVtype, int nPosition, qint64 nOffset)
{
    Q_UNUSED(vValue)
    Q_UNUSED(nStype)
    Q_UNUSED(nNdata)
    Q_UNUSED(nVtype)
    Q_UNUSED(nPosition)
    Q_UNUSED(nOffset)

    bool bResult=false;

    blockSignals(true);

//    quint64 nValue=vValue.toULongLong();

    if(getDevice()->isWritable())
    {

    }

    blockSignals(false);

    return bResult;
}

void BinaryWidget::setReadonly(bool bState)
{
    ui->widgetHex->setReadonly(bState);
}

void BinaryWidget::blockSignals(bool bState)
{
    Q_UNUSED(bState)
}

void BinaryWidget::adjustHeaderTable(int type, QTableWidget *pTableWidget)
{
    Q_UNUSED(type)
    Q_UNUSED(pTableWidget)
//    int nSymbolWidth=getSymbolWidth();
}

void BinaryWidget::reloadData()
{
    XBinary binary(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

    if(binary.isValid())
    {
        if(!bInit)
        {
            ui->widgetHex->setData(getDevice(),getOptions());
            ui->widgetHex->setEdited(isEdited());
            connect(ui->widgetHex,SIGNAL(editState(bool)),this,SLOT(setEdited(bool)));

            bInit=true;
        }
        ui->widgetHex->reload();

        setReadonly(ui->checkBoxReadonly->isChecked());
    }
}

void BinaryWidget::widgetValueChanged(quint64 nValue)
{
    Q_UNUSED(nValue)
//    QWidget *pWidget=qobject_cast<QWidget *>(sender());
//    int nStype=pWidget->property("STYPE").toInt();
//    int nNdata=pWidget->property("NDATA").toInt();
}

void BinaryWidget::on_checkBoxReadonly_toggled(bool checked)
{
    setReadonly(checked);
}

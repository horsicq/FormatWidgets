/* Copyright (c) 2017-2022 hors<horsicq@gmail.com>
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
#include "binarywidget.h"
#include "ui_binarywidget.h"

BinaryWidget::BinaryWidget(QWidget *pParent) :
    FormatWidget(pParent),
    ui(new Ui::BinaryWidget)
{
    ui->setupUi(this);

//    connect(ui->widgetStrings,SIGNAL(showHex(qint64,qint64)),this,SLOT(showInHexWindow(qint64,qint64)));
}

BinaryWidget::BinaryWidget(QIODevice *pDevice,FW_DEF::OPTIONS options,QWidget *pParent) :
    BinaryWidget(pParent)
{
    ui->setupUi(this);

    BinaryWidget::setData(pDevice,options,0,0,0);
    BinaryWidget::reload();
}

BinaryWidget::~BinaryWidget()
{
    delete ui;
}

void BinaryWidget::clear()
{
    BinaryWidget::reset();

    ui->checkBoxReadonly->setChecked(true);
}

void BinaryWidget::cleanup()
{

}

void BinaryWidget::reload()
{
    // TODO Hex
    BinaryWidget::clear();

    ui->checkBoxReadonly->setEnabled(!isReadonly());

    XBinary binary(getDevice(),getOptions().bIsImage,getOptions().nImageBase);

    if(binary.isValid())
    {
        setFileType(binary.getFileType());

        BinaryWidget::reloadData();
    }
}

FormatWidget::SV BinaryWidget::_setValue(QVariant vValue,int nStype,int nNdata,int nVtype,int nPosition,qint64 nOffset)
{
    Q_UNUSED(vValue)
    Q_UNUSED(nStype)
    Q_UNUSED(nNdata)
    Q_UNUSED(nVtype)
    Q_UNUSED(nPosition)
    Q_UNUSED(nOffset)

    SV result=SV_NONE;

    blockSignals(true);

//    quint64 nValue=vValue.toULongLong();

    if(getDevice()->isWritable())
    {

    }

    blockSignals(false);

    return result;
}

void BinaryWidget::setReadonly(bool bState)
{
    ui->widgetHex->setReadonly(bState);
}

void BinaryWidget::blockSignals(bool bState)
{
    Q_UNUSED(bState)
}

void BinaryWidget::adjustHeaderTable(int nType,QTableWidget *pTableWidget)
{
    Q_UNUSED(nType)
    Q_UNUSED(pTableWidget)
//    qint32 nSymbolWidth=getSymbolWidth();
}

void BinaryWidget::reloadData()
{
    XBinary binary(getDevice(),getOptions().bIsImage,getOptions().nImageBase);

    if(binary.isValid())
    {
        ui->widgetHex->setData(getDevice(),getOptions(),getBackupDevice());
//        ui->widgetHex->setBackupDevice(getBackupDevice());
        ui->widgetHex->reload();

        BinaryWidget::setReadonly(ui->checkBoxReadonly->isChecked());
    }
}

void BinaryWidget::widgetValueChanged(quint64 nValue)
{
    Q_UNUSED(nValue)
//    QWidget *pWidget=qobject_cast<QWidget *>(sender());
//    int nStype=pWidget->property("STYPE").toInt();
//    int nNdata=pWidget->property("NDATA").toInt();
}

void BinaryWidget::on_checkBoxReadonly_toggled(bool bChecked)
{
    setReadonly(bChecked);
}

void BinaryWidget::on_pushButtonReload_clicked()
{
    ui->pushButtonReload->setEnabled(false);
    reload();

    QTimer::singleShot(1000,this,SLOT(enableButton()));
}

void BinaryWidget::enableButton()
{
    ui->pushButtonReload->setEnabled(true);
}
